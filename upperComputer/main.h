#ifndef MAIN_H
#define MAIN_H

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
    void startWindowInit(void);

    QStackedWidget* pStackedWidget;
};
#endif // WIDGET_H
