#include "httpserver.h"
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QTcpSocket>

httpServer::httpServer(QObject *parent):QTcpServer (parent)
{
}

QString httpServer::LoadFromLocalFile(const QString &path, const QString &searchString)
{
    QDir dir(path);
    if (dir.exists()) {
        QStringList subFolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &subFolder : subFolders) {
            QString subFolderPath = path + "/" + subFolder;
            QString foundPath = LoadFromLocalFile(subFolderPath, searchString);
            if (!foundPath.isEmpty()) {
                return foundPath;
            }
        }

        auto fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
        for (auto ite = fileList.begin(); ite != fileList.end(); ite++) {
            if ((*ite).isFile() == true) {//如果是文件
                if (ite->suffix() == "mp3" && ite->fileName().contains(searchString)) {//后缀是mp3且文件名包含传入字符串
                    return ite->filePath();
                }
            }
        }
    } else {
        qDebug() << "该文件夹不存在" << endl;
    }

    return ""; // 如果未找到匹配的MP3文件，则返回空字符串
}


void httpServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket;
    qDebug()<<"流式传输服务器已启动"<<endl;
    if(!socket->setSocketDescriptor(socketDescriptor))
    {
        qWarning()<<"Failed to set socket descriptor";
        return;
    }
    qDebug()<<"流式传输已经连接"<<endl;
    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
        if (socket->canReadLine()) {
            QString path = "C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\music";
            QByteArray requestLine = socket->readLine();
            // 简单解析请求行，例如 "GET /audio.mp3 HTTP/1.1"
            QStringList tokens = QString(requestLine).split(QRegExp("[ \r\n][ \r\n]*"));
            qDebug()<<"正则表达式已收到"<<endl;
            if (tokens.size() >= 3 && tokens[0] == "GET") {
                qDebug() << tokens[1].split("/")[1].split(".")[0];
                auto Utfcode = tokens[1].split("/")[1].split(".")[0];
                QString fileName = QUrl::fromEncoded(Utfcode.toUtf8()).toString();
                qDebug() << fileName;
                QString filePath = LoadFromLocalFile(path,fileName);
                qDebug()<<"文件路径："<<filePath;
                QFile file(filePath);
                if (file.open(QIODevice::ReadOnly)) {
                    QByteArray header = "HTTP/1.1 200 OK\r\n"
                                        "Content-Type: audio/mpeg\r\n"
                                        "Connection: close\r\n\r\n";
                    socket->write(header + file.readAll()); // 发送头部及文件内容
                    socket->waitForBytesWritten();
                    socket->disconnectFromHost();
                    qDebug()<<"消息头已发送，断开连接"<<endl;
                } else {
                    QByteArray header = "HTTP/1.1 404 Not Found\r\n"
                                        "Content-Type: text/plain\r\n"
                                        "Connection: close\r\n\r\n"
                                        "File Not Found";
                    socket->write(header); // 文件未找到，发送404
                    socket->disconnectFromHost();
                }
            }
        }
    });
}
