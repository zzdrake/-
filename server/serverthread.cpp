#include "serverthread.h"
#include <QHostAddress>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

struct MusicInfo
{
    QString Name;
    QString Singer; //歌手
    QString filePath;//文件路径
    QString postPath;//海报路径
    QString Lrc; //歌词
};

struct ObjectPrivate
{
    QByteArray MusicVecToJson()
    {
        QJsonObject obj;
        obj["flag"] = 1;
        QJsonArray array;
        for(auto ite = MusicVec.begin();ite != MusicVec.end(); ite++)
        {
            array.append(ite->Name);
        }
        obj["data"] = array;
        return QJsonDocument(obj).toJson();
    }
    std::vector<MusicInfo> MusicVec; //歌曲列表
};

ServerThread::ServerThread(QObject *parent) : QObject(parent),p(new ObjectPrivate){}

ServerThread::~ServerThread()
{
    delete p;
}

void ServerThread::run()
{
    LoadMusicFiles("C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\music");
    QString ip = socket->peerAddress().toString();
    QString port = QString::number(socket->peerPort());
    QString text = QString("来自ip:") + ip + "端口:" + port;
    qDebug()<<text<<endl;
    emit sendData(p->MusicVecToJson());
}

void ServerThread::SetSocket(QTcpSocket *socket)
{
    this->socket = socket;
}

void ServerThread::LoadMusicFiles(const QString &path)
{
    QDir dir(path);
    if(dir.exists())
    {
        //获取该文件夹下所有的文件
        auto FileList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        for(auto ite = FileList.begin(); ite != FileList.end();ite++)
        {
            if(ite->isDir() == true)
            {
                LoadMusicFiles(ite->path() + "\\" + ite->fileName());
            }
            if((*ite).isFile() == true)
            {
                MusicInfo m;
                if(ite->suffix() == "mp3")
                {
                    m.Name = ite->baseName(); //文件名字
                    m.Singer = dir.dirName();

                    m.filePath = ite->path() + "/" + ite->fileName();
                    p->MusicVec.push_back(m);
                }
            }
        }
        qDebug() << "添加成功" << endl;
    }
    else
    {
        qDebug() << "该音乐文件夹不存在！" <<endl;
    }
}

void ServerThread::sendFile()
{

}

