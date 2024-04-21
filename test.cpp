#include "test.h"
#include "ui_test.h"


test::test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);

    request.setUrl(QUrl("https://aip.baidubce.com/oauth/2.0/token"
                           "?grant_type=client_credentials&"
                           "client_id=IaK0lhk65aOvsiG1ICMyeK5h&"
                           "client_secret=mYqGpBUa8jxjeCN9UEoCrYShbzmoZd50"));

    //设置header
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Accept","application/json");

    //获取权限字符串
    GetToken();

    connect(ui->pushButton_send,&QPushButton::clicked,this,&test::Talk);
}

test::~test()
{
    delete ui;
}

void test::GetToken()
{
    reply = manger.post(request,"");
    connect(reply,&QNetworkReply::finished,this,&test::Token);
}

void test::Token()
{
    if(reply->error() == QNetworkReply::NoError)
       {
   //        auto content = p->reply->readAll();
   //        qDebug() << content << endl;
           accessToken = QJsonDocument::fromJson(reply->readAll()).object()["access_token"].toString();
           qDebug() << accessToken <<endl;
       }
       else {
           qDebug() << reply->errorString()<<endl;
       }
       disconnect(reply,&QNetworkReply::finished,this,&test::Token);
       reply->deleteLater();
}

void test::Talk()
{
    request.setUrl(QUrl("https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop"
                           "/chat/completions?access_token=" + accessToken));
    QJsonObject obj;
    QJsonArray array;
    QJsonObject role;
    role["role"] = "user";
    role["content"] = ui->textEdit_ask->toPlainText();
    array.append(role);
    obj["messages"] = array;


    reply = manger.post(request,QJsonDocument(obj).toJson());
    connect(reply,&QNetworkReply::finished,this,&test::TalkReply);
}

void test::TalkReply()
{
    if(reply->error() == QNetworkReply::NoError)
    {
        auto content = reply->readAll();
        ui->textEdit_reply->setText(QJsonDocument::fromJson(content).object()["result"].toString());
        qDebug() << accessToken <<endl;
    }
    else {
        qDebug() << reply->errorString()<<endl;
    }
    disconnect(reply,&QNetworkReply::finished,this,&test::TalkReply);
    reply->deleteLater();
}
