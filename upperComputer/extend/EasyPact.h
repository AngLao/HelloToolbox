#ifndef __EasyPact_H
#define __EasyPact_H


#ifdef __cplusplus
extern "C" {

#endif

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;

#define MAX_DATA_LENGTH             32

extern uint8_t REC_HEADER ;
extern uint8_t check_model ;
extern uint8_t transfer_model ;
//校验方法选择
#define sum_model 					0
#define crc16_model                 1
#define niming_model                2

//数据传输模式选择   低位先出? 高位先出?
#define low_first_out               1
#define high_first_out              2


//----------发送方方法声明----------
typedef struct {
    uint8_t header;         // 帧头
    uint8_t address;        // 地址
    uint8_t id;             // ID
    uint8_t data_length;    // 数据长度
    uint8_t data[MAX_DATA_LENGTH];  // 数据
    // 帧校验添加到数据末尾
} frame_t;


//抽象层方法
void easy_wipe_data(frame_t *p_Frame);
void easy_set_id(frame_t *p_Frame, uint8_t id);
void easy_set_header(frame_t *p_Frame, uint8_t header);
void easy_set_address(frame_t *p_Frame, uint8_t address);
uint8_t easy_add_check(frame_t *p_Frame);
uint8_t easy_add_data(frame_t *p_Frame, uint32_t data, uint8_t length);

//----------校验方法声明----------
uint16_t calculate_check(frame_t *p_Frame, uint8_t length);
uint16_t calculate_sum(frame_t *p_Frame, uint8_t length) ;
uint16_t calculate_crc(frame_t *p_Frame, uint8_t length);


//----------接收方方法声明----------
typedef enum {
    HEADER = 0,
    ADDRESS,
    ID,
    DATA_LENGTH,
    DATA,
    CHECK_HIGH,
    CHECK_LOW
} parse_state_t;

uint8_t  easy_return_buflen(frame_t* p_Frame);
uint16_t easy_return_name(frame_t* p_Frame);
uint8_t easy_parse_data(frame_t** frame,const uint8_t new_data);


#ifdef __cplusplus
}
#endif

#endif
