
#ifndef PARAMETERCONFIGURATION_H
#define PARAMETERCONFIGURATION_H


#include <QObject>

#include "qboxlayout.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qslider.h"
#include "qspinbox.h"
#include "qwidget.h"

#include "extend/EasyPact.h"

class ParameterConfiguration : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterConfiguration(QWidget *parent = nullptr);

    QWidget* widget();
    QWidget* addCtrBlock();
    QWidget* addConfBlock();


signals:
    void sendPackData(const char *data , const int DataLen);;
private:
    QWidget* debugViewWidget;
    uint8_t TargetHeader;
signals:

};

#endif // PARAMETERCONFIGURATION_H
