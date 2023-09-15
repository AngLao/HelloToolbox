#ifndef FRMDEVICE_H
#define FRMDEVICE_H

#include <QWidget>

namespace Ui {
class frmDevice;
}

class frmDevice : public QWidget
{
    Q_OBJECT

public:
    explicit frmDevice(QWidget *parent = 0);
    ~frmDevice();

    void addFrmDevice(QStringList* mQStringlis);

    void setTop(  QString QStringList);
    void setBottom(  QString QStringList);
    void setLabelValue(  QStringList *QStringList);

private:
    Ui::frmDevice *ui;
};

#endif // FRMDEVICE_H
