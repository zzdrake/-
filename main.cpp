#include "widget.h"
//#include "Login.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QMenu>

int main(int argc, char *argv[])
{
    //QT应用基础类
    QApplication a(argc, argv);
//    Login l;
//    l.show();
    Widget w;
    w.show();


    //事件循环
    return a.exec();
}
