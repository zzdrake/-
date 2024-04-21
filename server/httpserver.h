#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include <QTcpServer>

class httpServer:public QTcpServer
{
    Q_OBJECT
public:
    explicit httpServer(QObject *parent = nullptr);
    QString LoadFromLocalFile(const QString &path,const QString &searchString);
protected:
    void incomingConnection(qintptr socketDescriptor)override;
};

#endif // HTTPSERVER_H
