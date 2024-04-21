#include "widget.h"
#include "ui_widget.h"
#include <QIcon>
#include <QFile>
#include <QDebug> //输出到终端 std::cout
#include <QDir> //文件夹对象
#include <QMenu>
#include <QEvent>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QString>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QDataStream>
#include <QMediaPlaylist>
#include <QPropertyAnimation>
#include <QColor>
#include <QNetworkAccessManager>
#include "NetworkConfigWidget.h"
#include "test.h"


enum JSONFLAG
{
    MUSICLIST,
    MUSICFILE,
    MUSICPOST,
    MUSICLRC
};

struct WidgetPrivate
{
    QNetworkAccessManager *http_acc;
    bool isplay=false;
    QMediaPlayer *player;

    QString accessToken; //权限字符串
    QNetworkRequest request; //http请求对象
    QNetworkAccessManager manger; //http应答管理对象
    QNetworkReply *reply; //http应答指针

    //搜索歌单列表
    std::map<int,QString> searchmusictime;
    std::map<int,QString> searchmusicname;
    std::map<int,QString> searchmusicurl;

    int songrow = 0;
    QString url;

    QString name;//从歌单里读客户端请求的音乐名
    std::vector<QString> RequestVec;//歌名塞到vec里发给服务器处理


    QString Name; //歌名
    QString Singer; //歌手

    QString filePath; //路径
    QString postPath; //海报路径
    QString lrcPath; //海报路径

    QByteArray data;
    QByteArray lrc_data;
    QByteArray post_data;
};

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),w(new WidgetPrivate),
    currentImageIndex(0),
    imageTimeer(new QTimer(this)),
    imagePath({":/image/1.png",":/image/2.png",":/image/3.png"})
{
    ui->setupUi(this);
    //设置窗口标题
    this->setObjectName("MainWidget");

    //消除原widget边框
    this->setWindowFlag(Qt::FramelessWindowHint);

    this->setWindowIcon(QIcon(":/image/IOS.png"));
    ui->pushButton_player->setIcon(QIcon(":/image/play.png"));

    QPixmap logo(":/image/netmusic.png");
    ui->label_logo->setPixmap(logo);
    ConfigWidget = new NetworkConfigWidget();

    //设置按钮的初始状态
    QIcon last(":/image/last.png");
    QPixmap lastmap = last.pixmap(ui->pushButton_last->size());
    ui->pushButton_last->setIcon(QIcon(lastmap));
    ui->pushButton_last->setIconSize(ui->pushButton_last->size());

    QIcon next(":/image/next.png");
    QPixmap nextmap = next.pixmap(ui->pushButton_next->size());
    ui->pushButton_next->setIcon(QIcon(nextmap));
    ui->pushButton_next->setIconSize(ui->pushButton_next->size());

    QIcon order(":/image/order.png");
    QPixmap ordermap = order.pixmap(ui->pushButton_mode->size());
    ui->pushButton_mode->setIcon(QIcon(ordermap));
    ui->pushButton_mode->setIconSize(ui->pushButton_mode->size());

    QIcon play(":/image/play.png");
    QPixmap playmap = play.pixmap(ui->pushButton_player->size());
    ui->pushButton_player->setIcon(QIcon(playmap));
    ui->pushButton_player->setIconSize(ui->pushButton_player->size());

    ui->pushButton_close->setIcon(QIcon(":/image/cha.png"));
    ui->pushButton_min->setIcon(QIcon(":/image/mini.png"));
    ui->pushButton_max->setIcon(QIcon(":/image/expand.png"));
    ui->pushButton_left->setIcon(QIcon(":/image/left.png"));
    ui->pushButton_right->setIcon(QIcon(":/image/right.png"));
    ui->pushButton_micro->setIcon(QIcon(":/image/search.png"));
    ui->label_headp->setPixmap(QPixmap(":/image/ball.png").scaled(ui->label_headp->width(),
                                                                  ui->label_headp->height()));
    ui->pushButton_color->setIcon(QIcon(":/image/clothes.png"));
    ui->pushButton_volume->setIcon(QIcon(":/image/volume.png"));

    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget_like->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget_like->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->widget_onlinePlay->setStyleSheet("background-image: url(:/image/Onlie.jpg); background-repeat: no-repeat; background-position: center; background-size: cover;");

    //设置音量初始值
    p.setVolume(50);
    ui->verticalSlider->hide();
    volumPressed=false;

    // 加载图标
    QIcon icon(":/image/vip.png");

    // 获取图标的 QPixmap 对象
    QPixmap pixmap = icon.pixmap(ui->pushButton_vip->size());  // 使用按钮的大小作为 QPixmap 的大小

    // 设置按钮的背景
    ui->pushButton_vip->setIcon(QIcon(pixmap));
    ui->pushButton_vip->setIconSize(ui->pushButton_vip->size());

    ui->label_posta->setPixmap(QPixmap(":/image/A.png").scaled(ui->label_posta->width(),
                                                               ui->label_posta->height()));
    originalLabelSize = ui->label_posta->size();
    connect(imageTimeer,&QTimer::timeout,this,&Widget::updataImage);
    imageTimeer->start(3000);

    ui->label_postb->setPixmap(QPixmap(":/image/B.png").scaled(ui->label_postb->width(),
                                                               ui->label_postb->height()));
    ui->label_postc->setPixmap(QPixmap(":/image/C.png").scaled(ui->label_postc->width(),
                                                               ui->label_postc->height()));
    ui->label_postd->setPixmap(QPixmap(":/image/D.png").scaled(ui->label_postd->width(),
                                                               ui->label_postd->height()));
    ui->label_poste->setPixmap(QPixmap(":/image/E.png").scaled(ui->label_poste->width(),
                                                               ui->label_poste->height()));
    //设置listwidget透明
    ui->listWidget->setStyleSheet("QListWidget{border:none}" "QListWidget{background-color:transparent}");
    ui->label_LRC->setStyleSheet("QLabel{border:none}" "QLabel{background-color:transparent}");
    ui->pushButton_connect->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->pushButton_ipconfig->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->label->setStyleSheet("QLabel{border:none}" "QLabel{background-color:transparent}");
    ui->pushButton_volume->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->listWidget_like->setStyleSheet("QListWidget{border:none}" "QListWidget{background-color:transparent}");
    ui->label_2->setStyleSheet("QLabel{border:none}" "QLabel{background-color:transparent}");
    ui->pushButton_love->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->pushButton_love->setIcon(QIcon(":/image/loverded.png"));
    ui->pushButton_lrc->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->pushButton_lrc->setIcon(QIcon(":/image/lrc.png"));
    ui->pushButton_play->setIcon(QIcon(":/image/pauseOnline.png"));
    ui->pushButton_play->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->pushButton_play->setIconSize(ui->pushButton_play->size());
    ui->pushButton_ai->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->pushButton_chat->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->pushButton_stream->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");

    //设置封面专辑初始状态
    QPixmap post(":/image/electronics.png");
    ui->label_post->setPixmap(post);

    //元对象系统中的对象名
    ui->pushButton_player->setObjectName("BtnPlay");

    //取代多if，状态注册机，状态不变
    PlayTypeMap[0] = QIcon(":/image/order.png");
    PlayTypeMap[1] = QIcon(":/image/loop.png");
    PlayTypeMap[2] = QIcon(":/image/rand.png");

    typeCount = 0;
    ui->pushButton_mode->setIcon(PlayTypeMap[typeCount]);

//    LoadMusicFiles("C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\music");

    //xml配置
    connect(ui->pushButton_ipconfig,&QPushButton::clicked,this,&Widget::NetworkConfig);

    connect(ui->pushButton_connect,&QPushButton::clicked,this,&Widget::connectToServer);

    connect(ConfigWidget,&NetworkConfigWidget::PBOK_Press,this,&Widget::connectToServer);

    socket = new QTcpSocket(this);

    //设置媒体源 url表示网络路径和本地路径的集合 异步操作
//p.setMedia(QUrl::fromLocalFile("C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\music\\夜曲.mp3"));
//  p.play();


    //建立信号发送者、信号、信号接收者、槽
    //connect(sender,signal,receiver,slot);四个参数都是指针，信号和槽都是函数指针
    connect(&p,&QMediaPlayer::durationChanged,this,&Widget::durationchanged);

    //connect函数将信号和槽进行绑定
    connect(ui->pushButton_player,&QPushButton::clicked,this,&Widget::PlayMusic);

    //当前播放进度变化的信号
    connect(&p,&QMediaPlayer::positionChanged,this,&Widget::Positionchanged);

    connect(&p,&QMediaPlayer::stateChanged,this,&Widget::PlayStateChanged);

    connect(ui->horizontalSlider,&QSlider::sliderMoved,&p,&QMediaPlayer::setPosition);

    connect(&timer,&QTimer::timeout,this,&Widget::SetLRC);

    connect(ui->listWidget,&QListWidget::doubleClicked,this,&Widget::DoubleClickedListWidget);

    connect(ui->listWidget_stream,&QListWidget::doubleClicked,this,&Widget::DoubleClickedStreamWidgset);

    //下一首按钮
    connect(ui->pushButton_next,&QPushButton::clicked,this,&Widget::NextBtnClicked);

    //上一首按钮
    connect(ui->pushButton_last,&QPushButton::clicked,this,&Widget::PrevBtnClicked);

    //切换播放模式
    connect(ui->pushButton_mode,&QPushButton::clicked,this,&Widget::PlayTypeBtnClicked);

    //关闭窗口
    connect(ui->pushButton_close,&QPushButton::clicked,this,&Widget::close);

    //最小化窗口
    connect(ui->pushButton_min,&QPushButton::clicked,this,&Widget::showMinimized);

    w->http_acc=new QNetworkAccessManager;
    w->player = new QMediaPlayer;

    //在线播放音乐
    connect(w->http_acc,SIGNAL(finished(QNetworkReply*)),this,SLOT(recvAck(QNetworkReply*)));
    connect(ui->listWidget_like, &QListWidget::itemClicked, [=](QListWidgetItem *item) {
        w->songrow = ui->listWidget_like->currentRow();
        OnlinePlay();
    });

    //打开右键菜单
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget,&QListWidget::customContextMenuRequested,this,&Widget::ListcustomContextMenuRequested);

    //鉴权
    //设置url
    w->request.setUrl(QUrl("https://aip.baidubce.com/oauth/2.0/token"
                           "?grant_type=client_credentials&"
                           "client_id=IaK0lhk65aOvsiG1ICMyeK5h&"
                           "client_secret=mYqGpBUa8jxjeCN9UEoCrYShbzmoZd50"));

    //设置header
    w->request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    w->request.setRawHeader("Accept","application/json");

    //获取权限字符串
    GetAccessToken();

    connect(ui->pushButton_ai,&QPushButton::clicked,this,&Widget::Generate);
}

Widget::~Widget()
{
    delete ui;
    delete w;
    socket->deleteLater();
}

long long StringToMsec(const QString& timeStr)
{
    auto LeftList = timeStr.split(":");
    if(LeftList.size() != 2)
        return 0;
    auto Min = LeftList[0].toInt();
    auto RightList = LeftList[1].split(".");
    if(RightList.size() != 2)
        return 0;

    int sec = RightList[0].toInt();
    int msec = RightList[1].toInt();
    //最终转换成毫秒
    return msec + 1000 * sec + 1000 * 60 * Min;
}

void Widget::ReadLRC(const QString &filepath)
{
    //先清空上次存放的数据
    LRCMap.clear();
    QFile file(filepath);
    if(file.open(QIODevice::ReadOnly) == true)
    {
        QTextStream input(&file);
        //是否已到文件末尾
        while(input.atEnd() != true)
        {
            auto content = input.readLine();
//            qDebug()<<content<<endl;
            auto LineList = content.split("]");
            if(LineList.size() != 2)
               continue;
            auto TimeStr = LineList[0].remove('[');
            auto TotalTime = StringToMsec(TimeStr);
            QString LRC = LineList[1];

            //时间做key，歌词做value
            LRCMap[TotalTime] = LRC;
        }
        file.close();
    }
    else
    {
        qDebug()<<"打开歌词文件失败!\n";
        return;
    }
}

void Widget::OnlinePlay()
{
    w->isplay=true;
    w->player->setMedia(QUrl(w->searchmusicurl[w->songrow]));
    qDebug()<<w->searchmusicurl[w->songrow]<<endl;
    w->url=w->searchmusicurl[w->songrow];
    w->player->play();
    ui->pushButton_play->setIcon(QIcon(":/image/pauseOnline.png"));
}

void Widget::GetAccessToken()
{
    w->reply = w->manger.post(w->request,"");
    connect(w->reply,&QNetworkReply::finished,this,&Widget::AccessToken);
}

void Widget::PlayMusic()
{
    //多媒体的播放状态
    if(p.state() != QMediaPlayer::PlayingState)
    {
        p.play();
        //定时器在播放时启动，500毫秒检查一次是否该换歌词了
        timer.start(500);
        ui->pushButton_player->setIcon(QIcon(":/image/pause.png"));
    }
    else{
        p.pause();
        //暂停时定时器关闭
        timer.stop();
        ui->pushButton_player->setIcon(QIcon(":/image/play.png"));
    }
}


void Widget::durationchanged()
{
    long long durantion = p.duration();
    //设置label的文本
    //sprintful();在不确定参数时使用这种写法
    QString time = QString("%1:%2").arg(durantion/1000/60).arg(durantion/1000%60);
    ui->label_completeTime->setText(time);

    ui->horizontalSlider->setRange(0,durantion);
}

void Widget::Positionchanged()
{
    long long positon = p.position();
    QString time = QString("%1:%2").arg(positon/1000/60).arg(positon/1000%60);
    ui->label_currentTime->setText(time);
    ui->horizontalSlider->setValue(positon);
}

void Widget::SetLRC()
{
    auto Time = p.position();
    for(auto ite = LRCMap.begin(); ite != LRCMap.end();)
    {
        auto PrevIte = ite++;

        //ite指向最后一个时要注意不要指针越界
        if(ite != LRCMap.end() && Time >= PrevIte->first && Time < ite->first)
        {
            ui->label_LRC->setText(PrevIte->second);
        }

        //显示最后一句歌词
        if(ite == LRCMap.end() && Time >= PrevIte->first)
        {
            ui->label_LRC->setText(PrevIte->second);
        }
    }
}

void Widget::LoadMusicFiles(const QString& path)
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
                if(ite->suffix() == "mp3")
                {
                    //把节点和路径进行捆绑
                    QListWidgetItem *item = new QListWidgetItem;
                    item->setText(ite->baseName());
                    //节点有一个userRole的位置给用户做自定义数据存储
                    //把路径作为该位置填进去
                    item->setData(Qt::UserRole,ite->path() + "/" + ite->fileName());
                    ui->listWidget->addItem(item);
                }
            }
        }
        ui->listWidget->setCurrentRow(0);
        qDebug()<< "添加成功" << endl;
    }
    else
    {
        qDebug()<< "该音乐文件夹不存在！" <<endl;
    }

}

void Widget::DoubleClickedListWidget()
{
    QString MusicPath = ui->listWidget->currentItem()->data(Qt::UserRole).toString();
    auto MusicName = MusicPath.split(".")[0];
    QFileInfo fileInfo(MusicPath);
    QDir dir(fileInfo.path());
    auto FileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for(auto ite = FileList.begin(); ite != FileList.end();ite++)
    {
        if(ite->suffix() == "png" || ite->suffix() == "jpg")
        {
            QPixmap map(ite->path() + "/" + ite->fileName());
            auto scaled = map.scaled(ui->label_post->width(),ui->label_post->height(),
                                     Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
            ui->label_post->setPixmap(scaled);
        }
    }
    p.setMedia(QUrl::fromLocalFile(MusicPath));
    ReadLRC(MusicName + ".lrc");
    PlayMusic();
}

void Widget::DoubleClickedStreamWidgset()
{
    QListWidgetItem *selectedItem = ui->listWidget_stream->selectedItems().first();
    if (selectedItem) {
        QString MusicName = selectedItem->text();
        qDebug() << "选中的播放音乐名为： " << MusicName;

        p.stop();
        qDebug() << "停止当前播放音乐";

        QString url = QString("http://localhost:8081/%1.mp3").arg(MusicName);
        qDebug() << "设置播放音乐的 URL: " << url;
        p.setMedia(QUrl(url));

        qDebug() << "尝试播放流式音乐";
        p.play();
    } else {
        qDebug() << "无音乐被选中";
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 记录鼠标点击位置相对于窗口左上角的偏移
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        // 移动窗口到当前鼠标位置
        move(event->globalPos() - dragStartPosition);
        event->accept();
    }
}

void Widget::NextBtnClicked()
{
    int nextRow = 0;
    switch(typeCount)
    {
    case 0:
       nextRow = (ui->listWidget->currentRow()+1) % ui->listWidget->count();
       break;
    case 1:
       nextRow = ui->listWidget->currentRow();
       break;
    case 2:
       nextRow = qrand() % ui->listWidget->count();
       break;
    }

    ui->listWidget->setCurrentRow(nextRow);
    DoubleClickedListWidget();
}

void Widget::PrevBtnClicked()
{
    int prevRow = 0;
    switch(typeCount)
    {
    case 0:
       prevRow = (ui->listWidget->currentRow() - 1 + ui->listWidget->count()) % ui->listWidget->count();
       break;
    case 1:
       prevRow = ui->listWidget->currentRow();
       break;
    case 2:
       prevRow = qrand() % ui->listWidget->count();
       break;
    }

    ui->listWidget->setCurrentRow(prevRow);
    DoubleClickedListWidget();
}

void Widget::PlayTypeBtnClicked()
{
    typeCount = (typeCount + 1) % 3;
    ui->pushButton_mode->setIcon(PlayTypeMap[typeCount]);
}

void Widget::ShowMusicInfo()
{
    QString MusicPath1=ui->listWidget->currentItem()->data(Qt::UserRole).toString();
    auto Musicname1=MusicPath1.split(".")[0];
    auto Musicname2=Musicname1.split("/")[1];
    w->name = Musicname2;
    QJsonObject obj2;
    obj2["flag"]=Musicname2;

    bool exist = QFile::exists(MusicPath1);
    if(exist == true)//如果本地有了 就不发请求
    {
        qDebug()<<"本地已有歌曲文件,不发送请求"<<endl;
        return;
    }
    else
    {
        socket->write(QJsonDocument(obj2).toJson());
        qDebug()<<"客户端给服务器发想要的歌名 成功";
        qDebug()<<"保存路径:"<<MusicPath1<<endl;
    }
}

void Widget::recvAck(QNetworkReply *r)
{
    int count=0;
    w->searchmusictime.clear();
    w->searchmusicname.clear();
    w->searchmusicurl.clear();
    QByteArray data;
    int musicId;
    int music_duration;
    int fee;
    QString musicname;
    QString singername;
    data = r->readAll();
    QJsonParseError err;
    QJsonDocument json=QJsonDocument::fromJson(data,&err);
    if(err.error!=QJsonParseError::NoError)
    {
        qDebug()<<"err"<<err.errorString();
        return;
    }
    QJsonObject object=json.object();
    QStringList keys=object.keys();
    if(keys.contains("result"))
    {
        QJsonObject result=object["result"].toObject();
        QStringList rkeys=result.keys();
        if(result.contains("songs"))
        {
            QJsonArray arr=result["songs"].toArray();
            for(auto i:arr)
            {
                QJsonObject object_1=i.toObject();
                if(object_1["fee"].toInt()==1)   //如果是VIP歌曲就继续找
                    continue;
                musicId=object_1["id"].toInt();
                music_duration=object_1["duration"].toInt();
                musicname=object_1["name"].toString();

                QStringList artistsKeys = object.keys();
                if(artistsKeys.contains("artists"))
                {
                    QJsonArray artistsArray = object["artists"].toArray();
                    for(auto j : artistsArray)
                    {
                        QJsonObject object_2 = j.toObject();
                        singername = object_2["name"].toString();         // 歌手名
                    }
                }
                w->searchmusictime[count]=QString::number(music_duration);//音乐总时长
                w->searchmusicname[count]=musicname;//音乐名字
                w->searchmusicurl[count]="https://music.163.com/song/media/outer/url?id="+QString::number(musicId);//音乐URL
                count++;
            }
        }
    }
    for(int i=0;i<w->searchmusicname.size();i++)
    {
        ui->listWidget_like->addItem(w->searchmusicname[i]);
    }
}

void Widget::updataImage()
{
    if (currentImageIndex < imagePath.size())
    {
        // 使用 scaled 方法，将图片缩放到 QLabel 的大小
        ui->label_posta->setPixmap(QPixmap(imagePath[currentImageIndex]).scaled(ui->label_posta->width(),
                                                                                ui->label_posta->height()));

        currentImageIndex = (currentImageIndex + 1) % imagePath.size();
    }
}

void Widget::ListcustomContextMenuRequested()
{
    //Qt菜单类
   QMenu menu;
   QAction*Download=new QAction("下载音乐");
   QAction*Downloadpost=new QAction("下载海报");
   QAction*Downloadlrc=new QAction("下载歌词");
   QAction*remove=new QAction("删除音乐");
   QAction*info=new QAction("音乐信息");
   menu.addAction(Download);
   menu.addAction(Downloadpost);
   menu.addAction(Downloadlrc);
   menu.addAction(remove);
   menu.addAction(info);
   //右键菜单显示在鼠标所在位置
   connect(Download,&QAction::triggered,this,&Widget::ShowMusicInfo);

   menu.exec(QCursor::pos());
}

void Widget::AccessToken()
{
    if(w->reply->error() == QNetworkReply::NoError)
        {
            w->accessToken = QJsonDocument::fromJson(w->reply->readAll()).object()["access_token"].toString();
            qDebug() << w->accessToken <<endl;
        }
        else {
            qDebug() << w->reply->errorString()<<endl;
        }
        disconnect(w->reply,&QNetworkReply::finished,this,&Widget::AccessToken);
        w->reply->deleteLater();
}

void Widget::Generate()
{
    w->request.setUrl(QUrl("https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/text2image/sd_xl?access_token="+w->accessToken));
    QJsonObject obj;
    obj["prompt"] = ui->textEdit->toPlainText();
    obj["size"] = "768x768";
    obj["n"] = 1;
    obj["steps"] = 20;
    obj["style"] = "Anime";
    qDebug()<<obj<<endl;
    w->reply = w->manger.post(w->request,QJsonDocument(obj).toJson());
    qDebug()<<"请求发送成功!\n";
    connect(w->reply,&QNetworkReply::finished,this,&Widget::GenerateReply);
}

void Widget::GenerateReply()
{
    if(w->reply->error() == QNetworkReply::NoError)
    {
        auto content = w->reply->readAll();
//        ui->textEdit_access->setText(QJsonDocument::fromJson(content).object()["result"].toString());
        auto array = QJsonDocument::fromJson(content).object()["data"].toArray();
        if(array.size() == 0)
        {
            qDebug() << "图片获取失败" << endl;
        }
        else {
            QByteArray data = QByteArray::fromBase64(array[0].toObject()["b64_image"].toString().toLocal8Bit());
            QFile file("./generate.jpg");
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(data);
                file.close();
                qDebug()<<"文件写入成功!\n";

                ui->label_headp->setPixmap(QPixmap("./generate.jpg").scaled(ui->label_headp->width(),
                                                                              ui->label_headp->height()));
            }
            else {
                qDebug() << "文件写入失败" <<endl;
            }

        }
    }
    else {
        qDebug() << w->reply->errorString()<<endl;
    }
    disconnect(w->reply,&QNetworkReply::finished,this,&Widget::GenerateReply);
    w->reply->deleteLater();
}

void Widget::PlayStateChanged()
{
    if(p.state() == QMediaPlayer::StoppedState && p.position() == p.duration())
        NextBtnClicked();
}

void Widget::on_verticalSlider_sliderMoved(int position)
{
    int red = position;
    int blue = 0;
    int green = 0;

    QString color = QString("background-color: reb(%1,%2,%3").arg(red).arg(green).arg(blue);

    ui->label->setStyleSheet(color);
}

void Widget::on_pushButton_close_clicked(){}

void Widget::on_pushButton_min_clicked(){}

void Widget::NetworkConfig()
{
    ConfigWidget->show();
}

void Widget::connectToServer()
{
    socket->disconnectFromHost();
    socket->connectToHost(QHostAddress(ConfigWidget->GetIP()),ConfigWidget->GetPort());
    socket->setSocketOption(QTcpSocket::ReceiveBufferSizeSocketOption,4096);

    QString SavePath = "C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\DownloadMusic";

    connect(socket,&QTcpSocket::connected,[](){
        qDebug() << "连接成功！\n";
    });

    //readyread信号接收到表明数据到达
    connect(socket,&QTcpSocket::readyRead,[this,SavePath]()
    {
        LoadMusicListFromServer(SavePath);
    });
}

void Widget::LoadMusicListFromServer(const QString &path)
{
    auto content = socket->read(4096);
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(content,&error);
    qDebug()<<"error:"<<error.errorString()<<endl;
    QJsonObject obj = doc.object();

    if(obj.isEmpty()==true)
    {
        qDebug()<<"obj为空";
    }
    else if(obj["flag"].toInt()==MUSICLIST)
    {
        muslistRecvJsonPktFmSvr(obj,path);
        return;
    }

    switch (obj["flag"].toInt())
    {
    case MUSICLIST://歌单
    {
        muslistRecvJsonPktFmSvr(obj,path);
    }
        break;
    case MUSICFILE://歌曲
    {
        musFileRecvJsonPcktFmSvr(obj,path);
    }
        break;
    case MUSICLRC://歌词
    {
        lrcRecvJsonPcktFmSvr(obj,path);
    }
        break;
    case MUSICPOST://海报
    {
        pstRecvJsonPckFmSvr(obj,path);
    }
        break;
    }

    content.clear();
}

void Widget::muslistRecvJsonPktFmSvr(const QJsonObject &obj, const QString &path)
{
    qDebug()<<"case MUSICLIST:objflag:"<<obj["flag"];
    auto array=obj["data"].toArray();
    for (int i=0;i<array.size();i++)
    {
        auto dataobj=array[i].toObject();
        QListWidgetItem*item=new QListWidgetItem;
        item->setText(dataobj["name"].toString());
        w->RequestVec.push_back(dataobj["name"].toString());
        item->setData(Qt::UserRole,path+"/"+dataobj["name"].toString()+".mp3");
        ui->listWidget->addItem(item);
    }

    for (int i=0;i<array.size();i++)
    {
        auto dataobj=array[i].toObject();
        QListWidgetItem*item2=new QListWidgetItem;
        item2->setText(dataobj["name"].toString());
        w->RequestVec.push_back(dataobj["name"].toString());
        item2->setData(Qt::UserRole,path+"/"+dataobj["name"].toString()+".mp3");
        ui->listWidget_stream->addItem(item2);
    }
}

void Widget::musFileRecvJsonPcktFmSvr(const QJsonObject &obj, const QString &path)
{
    static int count = 1;
    qDebug()<<"case MUSICFILE:objflag:"<<obj["flag"];
    QByteArray base64Data = obj["data"].toVariant().toByteArray();

    auto copyfile = QByteArray::fromBase64(base64Data);
    qDebug()<<"接收第"<<count++<<" size : "<<copyfile.size();

    QFile file(path+"/"+w->name+".mp3");
    if (file.open(QIODevice::Append)==false)
    {
        qDebug() << "Failed to open file:" << file.errorString();
        return;
    }
    else
    {
        qDebug() <<"写入文件:"<<w->name+".mp3";
        qint64 len = file.write(copyfile);
        qDebug()<<"歌曲写入大小:"<<len;
        file.flush();
        file.close();
    }
}

std::string utf8Togb2312(std::string utf8_str)
{
    QTextCodec* utf8Codec = QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

    QString strUnicode = utf8Codec->toUnicode(utf8_str.data());        //无编码
    QByteArray gb2312= gb2312Codec->fromUnicode(strUnicode);       //无编码转换

    return std::string(gb2312.data());
}

QByteArray utf8ToGb2312(const QByteArray &utf8ByteArray)
{
    // 获取GB2312编码的QTextCodec对象
      QTextCodec *codec = QTextCodec::codecForName("GB2312");
      if (!codec) {
          // 如果编码不支持，则返回空的QByteArray或进行错误处理
          qWarning("GB2312 codec not found");
          return QByteArray();
      }

      // 将UTF-8编码的QByteArray转换为Unicode字符串
      QString unicodeString = codec->toUnicode(utf8ByteArray);

      // 将Unicode字符串转换为GB2312编码的QByteArray
      QByteArray gb2312ByteArray = codec->fromUnicode(unicodeString);

      return gb2312ByteArray;
}

void Widget::lrcRecvJsonPcktFmSvr(const QJsonObject &obj, const QString &path)
{
    static int lrcPacketCount = 1;
    qDebug()<<" case MUSICLRC:objflag:"<<obj["flag"];
    QByteArray lrcData = obj["Lrc"].toVariant().toString().toLocal8Bit();
    QByteArray result = utf8ToGb2312(lrcData);
    qDebug()<<"接收第"<<lrcPacketCount++<<" size : "<<lrcData.size();

    QFile lrcfile(path+"/"+w->name+".lrc");
    if (lrcfile.open(QIODevice::Append)==false)
    {
        qDebug() << "Failed to open file:" << lrcfile.errorString();
        return;
    }
    else
    {
        qDebug() <<"写入文件:"<<w->name+".lrc";

        qint64 lrclen = lrcfile.write(result);

        qDebug()<<"歌词写入大小:"<<lrclen;
        lrcfile.flush();
        lrcfile.close();
    }
    ReadLRC(path+"/"+w->name+".lrc");
}

void Widget::pstRecvJsonPckFmSvr(const QJsonObject &obj, const QString &path)
{
    static int postPacketCount = 1;
    qDebug()<<"case MUSICPOST:objflag:"<<obj["flag"];
    QByteArray postData = obj["image"].toVariant().toByteArray();
    auto copypost = QByteArray::fromBase64(postData);
    qDebug()<<"接收第"<<postPacketCount++<<" size : "<<copypost.size();

    QFile postfile(path+"/"+w->name+".jpg");
    if (postfile.open(QIODevice::Append)==false)
    {
        qDebug() << "Failed to open file:" << postfile.errorString();
        return;
    }
    else
    {
        qDebug() <<"写入文件:"<<w->name+".jpg";
        qint64 postlen = postfile.write(copypost);
        qDebug()<<"海报写入大小:"<<postlen;
        postfile.flush();
        postfile.close();
    }
}

void Widget::on_pushButton_color_clicked()
{
    if(changeSkin)
    {
        if(changeSkin->isHidden())
        {
            changeSkin->show();
        }
        else {
            changeSkin->hide();
        }

    }
    else {
        changeSkin = new changeshkins(this);
        changeSkin->setGeometry(578,60,409,317); //这里要调整好
        changeSkin->raise();  //提示显示层数
        changeSkin->show();
    }
}

void Widget::on_pushButton_volume_clicked()
{
    if(!volumPressed)
    {
        ui->verticalSlider->show();
        volumPressed=true;
    }
    else
    {
        ui->verticalSlider->hide();
        volumPressed=false;
    }
}

void Widget::setVolum()
{
    p.setVolume(ui->verticalSlider->value());
}

void Widget::on_pushButton_love_clicked()
{
    ui->pushButton_love->setIcon(QIcon(":/image/loverd.png"));
}

void Widget::on_pushButton_micro_clicked()
{
    QNetworkRequest request;
    request.setUrl(QString("http://iwxyi.com:3000/search?keywords=%0").arg(ui->lineEdit->text()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36");
    w->http_acc->get(request);
    ui->listWidget_like->clear();
}

void Widget::on_pushButton_play_clicked()
{
    //多媒体的播放状态
    if(w->player->state() != QMediaPlayer::PlayingState)
    {
        w->player->play();
        ui->pushButton_play->setIcon(QIcon(":/image/pauseOnline.png"));
    }
    else{
        w->player->pause();
        ui->pushButton_play->setIcon(QIcon(":/image/playOnline.png"));
    }
}

void Widget::on_pushButton_chat_clicked()
{
    test* t = new test;
    t->show();
}
