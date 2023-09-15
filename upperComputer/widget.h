#ifndef WIDGET_H
#define WIDGET_H

#include <QFile>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget(){};

private:

    class qAgreement *pqAgreement;

    void startWindowInit(void);
    void qssInit(QString filePath);
    //自定义界面组件句柄
    QStackedWidget* m_pStackedWidget;
};
#endif // WIDGET_H
