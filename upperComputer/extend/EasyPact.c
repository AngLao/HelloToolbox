#include "EasyPact.h" 

uint8_t REC_HEADER = 0xAA;
uint8_t check_model = sum_model;
uint8_t transfer_model = high_first_out;
/**
 * @brief 解析帧数据
 * @param new_data 新的数据
 * @param frame 存储解析后的帧数据
 * @return 解析成功返回 0，否则返回 1
 */
uint8_t easy_parse_data(frame_t** frame,const uint8_t new_data)
{
    static parse_state_t	    parse_state = HEADER;
    static uint8_t 			 	data_index  = 0;
    static frame_t 			 	current_frame ;  // 静态变量存储当前帧的状态和数据
    static uint8_t* 			p_frame = (uint8_t*)&current_frame ;  // 一个用于操作帧数据的指针,使用uint8_t类型逐位操作

    uint8_t parse_fail  = 1;

    switch (parse_state) {
    case HEADER: {
        // 等待帧头的第一个字节
        if (new_data == REC_HEADER) {
            *(p_frame+HEADER) = new_data;
            parse_state = ADDRESS;
        }
        break;
    }
    case ADDRESS: {
        // 等待地址字节
        *(p_frame+ADDRESS) = new_data;
        parse_state = ID;
        break;
    }
    case ID: {
        // 等待 ID 字节
        *(p_frame+ID) = new_data;
        parse_state = DATA_LENGTH;
        break;
    }
    case DATA_LENGTH: {
        // 等待数据长度字节
        *(p_frame+DATA_LENGTH) = new_data;
        if (*(p_frame+DATA_LENGTH) > MAX_DATA_LENGTH) {
            //数据长度超出范围，重置解析状态
            parse_state = HEADER;
        } else {
            //接收索引归零
            data_index = 0;
            //没有要接收的数据直接接收检验码
            parse_state = (*(p_frame+DATA_LENGTH) > 0) ? DATA : CHECK_HIGH;
        }
        break;
    }
    case DATA: {
        // 等待数据字节
        *(p_frame+DATA+data_index) = new_data;
        data_index++;
        //接受数据长度符合待收数据长度
        if (data_index == *(p_frame+DATA_LENGTH)) {
            // 数据接收完毕，进入下一步状态
            parse_state = CHECK_HIGH;
        }
        break;
    }
    case CHECK_HIGH: {
        // 等待校验码高位字节 数据后第一位即为校验码高字节
        *(p_frame+DATA+data_index) = new_data  ;

        parse_state = CHECK_LOW;
        break;
    }
    case CHECK_LOW: {
        // 等待校验码低位字节 数据后第二位即为校验码高字节
        *(p_frame+DATA+data_index+1) = new_data;
        uint16_t checksum = calculate_check(&current_frame,*(p_frame+DATA_LENGTH)+4);
        uint16_t this_checksum = ((uint16_t)*(p_frame+DATA+data_index)<<8)+*(p_frame+DATA+data_index+1);
        if (this_checksum == checksum){
            // 校验码匹配，解析成功
            *frame =  &current_frame;
            parse_fail = 0;
        }
        parse_state = HEADER;
        break;
    }
    }

    return parse_fail;
}

/**
 * @brief  从帧中解析出帧名称
 * @param  p_Frame 指向帧结构体的指针
 * @return uint16_t 帧名称
 */
uint16_t easy_return_name(frame_t* p_Frame)
{
    // 从帧中读取ID和地址，组成帧名称
    uint16_t name =p_Frame->address << 8  ;
    name |=p_Frame->id  ;
    return name;
}

/**
 * @brief  从帧中解析出帧名称
 * @param  p_Frame 指向帧结构体的指针
 * @return uint8_t 帧总长度
 */
uint8_t easy_return_buflen(frame_t *p_Frame)
{

    uint8_t* p = (uint8_t*)p_Frame;
    return (*(p+DATA_LENGTH)+6) ;
}

/**
 * @brief 在数据帧中添加指定长度的数据
 * @param p_Frame 数据帧指针
 * @param data 要添加的数据
 * @param length 要添加的数据长度，单位为字节
 * @return 添加数据的结果，0表示成功，1表示数据长度超过最大限制
 */
uint8_t easy_add_data(frame_t *p_Frame, uint32_t data, uint8_t length)
{
    uint8_t* p = (uint8_t*)p_Frame;
    uint8_t old_length = *(p+DATA_LENGTH);
    //数据长度超过最大限制
    if((old_length+length) >MAX_DATA_LENGTH )
        return 1;
    //增加真实数据长度
    *(p+DATA_LENGTH) += length;
    //打包真实数据
    for(uint8_t i =0; i<length; i++){
        if(transfer_model == high_first_out){
            switch (length) {
            case sizeof(int):
                *(p+DATA+old_length+i) = data>>(8*(3-i));
                break;
            case sizeof(short):
                *(p+DATA+old_length+i) = data>>(8*(1-i));
                break;
            case sizeof(char):
                *(p+DATA+old_length+i) = data>>(8*i);
                break;
            }
        }else{
            *(p+DATA+old_length+i) = data>>(8*i);
        }
    }
    return 0;
}

/**
 * @brief 用于对指定数据帧进行校验，并将校验结果添加到数据帧的尾部。
 * @param p_Frame 指向待校验的数据帧指针。
 * @return 校验结果，为0表示校验成功。
 */
uint8_t easy_add_check(frame_t *p_Frame)
{
    uint8_t* p = (uint8_t*)p_Frame;    // 将指针类型转换为 uint8_t 指针类型
    uint8_t frame_len = *(p+DATA_LENGTH) + 4 ;    // 计算数据帧总长度
    uint16_t check = calculate_check(p_Frame , frame_len);    // 对数据帧进行校验

    /*指针即为0*/
    *(p+frame_len) = (check>>8);       // 将校验结果高位存储到数据帧尾部
    *(p+frame_len+1)  = check;         // 将校验结果低位存储到数据帧尾部

    return 0;    // 返回校验结果，为0表示校验成功
}


/**
 * @brief 用于设置指定数据帧的 HEADER 字段值。
 * @param p_Frame 指向待设置 HEADER 的数据帧指针。
 * @param header 指定的 HEADER 值。
 */
void easy_set_header(frame_t *p_Frame, uint8_t header)
{
    uint8_t* p = (uint8_t*)p_Frame;    // 将指针类型转换为 uint8_t 指针类型
    *(p+HEADER) = header;              // 将 HEADER 字段设置为指定的 HEADER 值
}

/**
 * @brief 用于设置指定数据帧的 ADDRESS 字段值。
 * @param p_Frame 指向待设置 ADDRESS 的数据帧指针。
 * @param address 指定的 ADDRESS 值。
 */
void easy_set_address(frame_t *p_Frame, uint8_t address)
{
    uint8_t* p = (uint8_t*)p_Frame;    // 将指针类型转换为 uint8_t 指针类型
    *(p+ADDRESS) = address;            // 将 ADDRESS 字段设置为指定的 ADDRESS 值
}

/**
 * @brief 用于设置指定数据帧的 ID 字段值。
 * @param p_Frame 指向待设置 ID 的数据帧指针。
 * @param id 指定的 ID 值。
 */
void easy_set_id(frame_t *p_Frame, uint8_t id)
{
    uint8_t* p = (uint8_t*)p_Frame;  // 将指针类型转换为 uint8_t 指针类型
    *(p+ID) = id;                    // 将 ID 字段设置为指定的 ID 值
}

/**
 * @brief 用于将指定数据帧中数据部分的所有字节都清零。
 * @param p_Frame 指向待清空数据的数据帧指针。
 */
void easy_wipe_data(frame_t *p_Frame)
{
    uint8_t* p = (uint8_t*)p_Frame;
    *(p+DATA_LENGTH) = 0;
}


/**
 * @brief 根据check_model选择相应的校验算法进行校验
 * @param p_Frame 待校验的数据帧指针
 * @param length 待校验的数据长度
 * @return 校验结果，根据check_model不同可能为校验和或CRC校验码
*/
uint16_t calculate_check(frame_t *p_Frame, uint8_t length)
{
    uint16_t res = 0;
    if(check_model == sum_model) // 如果使用校验和模式
        res = calculate_sum(p_Frame,length);
    else if(check_model == crc16_model) // 如果使用CRC16模式
        res = calculate_crc(p_Frame,length);
    return res;
}

/**
 * @brief 计算数据帧的校验和
 * @param p_Frame 待校验的数据帧指针
 * @param length 待校验的数据长度
 * @return 校验和
*/
uint16_t calculate_sum(frame_t *p_Frame, uint8_t length)
{
    uint8_t sum = 0;
    // 计算校验和
    for (int i = 0; i < length; i++) {
        sum += *((uint8_t*)p_Frame+i);
    }
    return sum;
}

/**
 * @brief 计算帧的CRC校验码
 * @param p_Frame 指向帧结构体的指针
 * @param length 帧长度（不包括校验码）
 * @return uint16_t CRC校验码
 */
uint16_t calculate_crc(frame_t *p_Frame, uint8_t length) {
#define POLY 0x1021
    uint16_t crc = 0xFFFF;

    // 遍历整个帧，计算CRC校验码
    for (int i = 0; i < length ; i++) {
        crc ^= ((uint16_t)*((uint8_t*)p_Frame+i) << 8);
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ POLY;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}
