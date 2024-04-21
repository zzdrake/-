#ifndef WIDGET_H
#define WIDGET_H
#include <QMediaPlayer>
#include <QWidget>
#include <map>
#include <QTimer>
#include <QIcon>
#include <QKeyEvent>
#include <QLabel>
#include <QTcpSocket>
#include <QTextCodec>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include "NetworkConfigWidget.h"
#include "changeshkins.h"

namespace Ui {
class Widget;
}

struct WidgetPrivate;
class Widget : public QWidget
{
    //元对象系统宏（没有定义元对象系统旧用不了信号和槽函数）
    //引入Qt信号和槽机制的一个宏
    Q_OBJECT

public:
    //默认参数初始化
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void ReadLRC(const QString& filepath);
    void OnlinePlay();
    void GetAccessToken();

//槽函数
public slots:
    void PlayMusic();
    void durationchanged();
    void Positionchanged();
    void SetLRC();
    void LoadMusicFiles(const QString &path);
    void LoadMusicListFromServer(const QString &path);
    void muslistRecvJsonPktFmSvr(const QJsonObject& obj, const QString &path);
    void musFileRecvJsonPcktFmSvr(const QJsonObject& obj,const QString &path);
    void lrcRecvJsonPcktFmSvr(const QJsonObject& obj, const QString &path);
    void pstRecvJsonPckFmSvr(const QJsonObject& obj, const QString &path);

    void PlayStateChanged();

    void DoubleClickedListWidget();
    void DoubleClickedStreamWidgset();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void NextBtnClicked();
    void PrevBtnClicked();
    void PlayTypeBtnClicked();
    void ShowMusicInfo();

    void ListcustomContextMenuRequested();

    void AccessToken();

    void Generate();

    void GenerateReply();

private slots:
    void on_verticalSlider_sliderMoved(int position);

    void on_pushButton_close_clicked();

    void on_pushButton_min_clicked();

    void NetworkConfig();

    void connectToServer();

    void on_pushButton_color_clicked();

    void on_pushButton_volume_clicked();

    void setVolum();

    void on_pushButton_love_clicked();

    void on_pushButton_micro_clicked();

    void recvAck(QNetworkReply *r);

    void updataImage();


    void on_pushButton_play_clicked();

    void on_pushButton_chat_clicked();

private:
    Ui::Widget *ui;
    QMediaPlayer p;
    WidgetPrivate *w;
    std::map<long long,QString> LRCMap;
    QTimer timer;
    QString Dirpath;
    int typeCount;
    std::map<int,QIcon> PlayTypeMap;
    QTcpSocket *socket;
    QPoint dragStartPosition; //添加窗口拖拽功能
    NetworkConfigWidget *ConfigWidget;
    changeshkins *changeSkin = nullptr;


    bool volumPressed=false;  //音量键未被按下
    bool isAreadyLike=false;   //是否喜欢
    int currentImageIndex; //当前图片下标
    QStringList imagePath; //图片路径
    QSize originalLabelSize; //原始图片尺寸
    QTimer *imageTimeer; //图片轮换定时器
};

#endif // WIDGET_H
