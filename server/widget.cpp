#include "widget.h"
#include "ui_widget.h"
#include <QTcpServer>
#include <QDir>
#include <QListWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QThread>
#include <vector>

struct MusicInfo
{
    QString name;
    QString Singer; //歌手
    QString filePath;//文件路径
    QString postPath;//海报路径
    QString lrcPath;  //歌词路径

    QByteArray Lrc; //歌词
    QByteArray image; //图片数据
    QByteArray data;
};

enum JSONFLAG
{
    MUSICLIST,
    MUSICFILE,
    MUSICPOST,
    MUSICLRC
};

struct WidgetPrivate
{
    std::vector<MusicInfo> MusicInfoVec;
    QByteArray MusicVecToJson()
    {
        QJsonObject obj;
        obj["flag"] = MUSICLIST;
        QJsonArray array;
        for(auto ite =MusicInfoVec.begin();ite != MusicInfoVec.end();ite++)
        {
            QJsonObject obj;
            obj["name"]=ite->name;
            array.append(obj);
        }
        obj["data"] = array;
        return QJsonDocument(obj).toJson();
    }
};

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),p(new WidgetPrivate)
{
    ui->setupUi(this);
    //元对象系统，在父类指针被释放时会释放子类指针
    QTcpServer *s = new QTcpServer(this);
    s->listen(QHostAddress::Any,8080); //bind + listen
    LoadMusicFile("C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\music");
    connect(s,&QTcpServer::newConnection,this,[s,this]()
    {
        ui->textEdit->append("检测到新的客户端连接");
        QTcpSocket*socket=s->nextPendingConnection();
        this->socket = socket;
        socket->write(p->MusicVecToJson());
        qDebug()<<"write歌单"<<endl;
        qDebug()<<socket->isOpen()<<endl;

        connect(socket,&QTcpSocket::readyRead,this,
                [this]()
        {
            SendJsonToClient();
        });
    });
}

Widget::~Widget()
{
    delete ui;
    delete p;
}

void Widget::LoadMusicFile(const QString &path)
{
    QDir dir(path);
    if(dir.exists())
    {
        auto FileList =dir.entryInfoList(QDir::Dirs| QDir::Files|QDir::NoDotAndDotDot);
        //dir.entryInfoList() 是 QDir 类的成员函数，用于获取指定目录中的文件和目录信息，并返回一个 QFileInfoList 对象，该对象包含了所有文件和目录的详细信息。
        for (auto ite=FileList.begin();ite!=FileList.end();ite++)
        {
            if(ite->isDir()==true)
            {
                qDebug()<<"要打开的是文件夹，文件夹的名字是"<<ite->fileName()<<endl;
                LoadMusicFile(ite->path()+"\\"+ite->fileName());
            }
            if((*ite).isFile()==true)
            {
                MusicInfo m;
                if(ite->suffix()=="mp3")
                {
                    m.name=ite->baseName();
                    m.Singer=dir.dirName();
                    m.filePath=ite->path()+"/"+ite->fileName();
                    QFile file(ite->path()+"/"+ite->fileName());
                    qDebug()<<"正在读取该文件的内容m.filePath："<<m.filePath;
                    if (file.open(QIODevice::ReadOnly)==false)
                    {
                        qDebug() << "Failed to open file:" << file.errorString();
                        continue;
                    }
                    else
                    {

                        m.data = file.readAll();

                        qDebug()<<"加载歌曲的名字是"<<ite->fileName();
                        qDebug()<<"加载歌曲的大小是"<<m.data.size();
                        file.close();
                    }



                    m.lrcPath =ite->path()+"/"+ite->baseName()+".lrc";
                    QFile lrcfile(m.lrcPath);
                    qDebug()<<"正在读取该文件的内容m.lrcPath："<<m.lrcPath;
                    if (lrcfile.open(QIODevice::ReadOnly)==false)
                    {
                        qDebug() << "Failed to open file:" << lrcfile.errorString();
                        continue;
                    }
                    else
                    {
                        m.Lrc = lrcfile.readAll();
                        qDebug()<<"加载歌词的大小是"<<m.Lrc.size();
                        lrcfile.close();
                    }


                    m.postPath =ite->path()+"/"+ite->baseName()+".jpg";
                    QFile postfile(m.postPath);
                    qDebug()<<"正在读取该文件的内容m.postPath："<<m.postPath;
                    if (postfile.open(QIODevice::ReadOnly)==false)
                    {
                        qDebug() << "Failed to open file:" << postfile.errorString();
                        continue;
                    }
                    else
                    {
                        m.image = postfile.readAll();
                        qDebug()<<"加载海报的大小是"<<m.image.size();
                        postfile.close();
                    }
                    p->WidgetPrivate::MusicInfoVec.push_back(m);
                }
            }
        }
    }
    else
    {
        qDebug()<<"该音乐文件夹不存在\n";
    }
}

QByteArray Widget::FileToJson(const char *packet, int JSONFLAG)
{
    QJsonObject obj;
       obj["flag"]= JSONFLAG;
       if(obj["flag"]== MUSICFILE)
       {
           obj["data"] = packet;
       }
       if(obj["flag"]== MUSICLRC)
       {
           obj["Lrc"] = packet;
       }
       if(obj["flag"]== MUSICPOST)
       {
           obj["image"] = packet;
       }
       return  QJsonDocument(obj).toJson();
}

QByteArray gb2312ToUtf8(const QByteArray &gb2312ByteArray) {
    // 获取GB2312编码的QTextCodec对象
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    if (!codec) {
        // 如果编码不支持，则返回空的QByteArray或进行错误处理
        qWarning("GB2312 codec not found");
        return QByteArray();
    }

    // 将GB2312编码的QByteArray转换为Unicode字符串
    QString unicodeString = codec->toUnicode(gb2312ByteArray);

    // 将Unicode字符串转换为UTF-8编码的QByteArray
    QByteArray utf8ByteArray = unicodeString.toUtf8();

    return utf8ByteArray;
}

void Widget::SendJsonToClient()
{
    qDebug()<<"收到请求,正在向客户端传输歌曲"<<endl;
    auto content=socket->readAll();
    auto obj=QJsonDocument::fromJson(content).object();
    auto c=obj["flag"].toString();
    qDebug()<<"想下载的歌："<<c<<endl;
    for (auto ite=p->MusicInfoVec.begin();ite!=p->MusicInfoVec.end();ite++)
    {
        if(c==ite->name)
        {
            qDebug()<<"服务器传的歌："<<ite->name<<endl;
            qDebug()<<"服务器传的歌曲大小："<<ite->data.size()<<endl;
            int startPosition = 0;
            int packetSize = 2048;//一包的大小1Kb
            int packnum=0;
            while (startPosition < ite->data.size())
            {
                QThread::msleep(20);
                QByteArray packet = ite->data.mid(startPosition, packetSize).toBase64();
                //从startPosition位置开始，提取packetSize长度的字节数据，存储在新的QByteArray对象packet中
                qDebug()<<"packetSize"<<packet.size();

                auto filecontent = FileToJson(packet,MUSICFILE);
                filecontent.insert(filecontent.size(),4096 - filecontent.size(),'\n');
                socket->write(filecontent,4096);
                socket->waitForBytesWritten(4096);

                qDebug()<<"filecontentSize:"<<filecontent.size();
                packnum++;
                qDebug()<<"向客户端发送文件,第"<<packnum<<"包"<<endl;
                startPosition += packetSize;
            }

            QThread::msleep(50);
            qDebug()<<"服务器传的歌词文件大小："<<ite->Lrc.size()<<endl;
            int lrc_startPosition = 0;
            int lrc_packetSize = 2048;//一包的大小1Kb
            int lrc_packnum=0;
            while (lrc_startPosition < ite->Lrc.size())
            {
                QThread::msleep(20);
                QByteArray lrc_packet = ite->Lrc.mid(lrc_startPosition, lrc_packetSize);
                //歌词转码
                QByteArray result = gb2312ToUtf8(lrc_packet);
                qDebug()<<"lrc_packetSize"<<lrc_packet.size();
                auto lrccontent = FileToJson(result,MUSICLRC);
                lrccontent.insert(lrccontent.size(),4096 - lrccontent.size(),'\n');

                socket->write(lrccontent,4096);//传的是QByteArray
                socket->waitForBytesWritten(4096);

                qDebug()<<"lrccontentSize"<<lrccontent.size();
                lrc_packnum++;
                qDebug()<<"向客户端发送歌词文件,第"<<lrc_packnum<<"包"<<endl;
                lrc_startPosition += lrc_packetSize;
            }

            QThread::msleep(50);
            qDebug()<<"服务器传的海报文件大小："<<ite->image.size()<<endl;
            int post_startPosition = 0;
            int post_packetSize = 2048;//一包的大小1Kb
            int post_packnum=0;
            while (post_startPosition < ite->image.size())
            {
                QThread::msleep(20);
                QByteArray post_packet = ite->image.mid(post_startPosition, post_packetSize).toBase64();
                qDebug()<<"packetSize"<<post_packet.size();

                auto postcontent = FileToJson(post_packet,MUSICPOST);
                postcontent.insert(postcontent.size(),4096 - postcontent.size(),'\n');

                socket->write(postcontent,4096);
                socket->waitForBytesWritten(4096);

                qDebug()<<"postcontentSize"<<postcontent.size();
                post_packnum++;
                qDebug()<<"向客户端发送海报文件,第"<<post_packnum<<"包"<<endl;
                post_startPosition += post_packetSize;
            }
        }
    }
}
