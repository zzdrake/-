#include "widget.h"
#include <QApplication>
#include "httpserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    httpServer server;
    if(!server.listen(QHostAddress::Any,8081))
    {
        qDebug()<<"Server could not start.";
    }
    else {
        qDebug()<<"Server started.";
    }
    return a.exec();
}
