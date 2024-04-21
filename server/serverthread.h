#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QFile>

struct ObjectPrivate;
class ServerThread : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit ServerThread(QObject *parent = nullptr);
    ~ServerThread();
    void run()override;
    void SetSocket(QTcpSocket *socket);
    void LoadMusicFiles(const QString& path);
    void sendFile();

private:
    QTcpSocket *socket;
    ObjectPrivate *p;

    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 sendSize; //已发送的文件大小

    QFile file;

signals:
    void sendData(const QByteArray &data);
};

#endif // SERVERTHREAD_H
