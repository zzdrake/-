#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include <QNetworkRequest>
#include <QNetworkAccessManager> //应答管理对象
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QKeyEvent>
#include <QFile>

namespace Ui {
class test;
}

class test : public QWidget
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = nullptr);
    ~test();

    void GetToken();

public slots:
    void Token();

    void Talk();

    void TalkReply();

private:
    Ui::test *ui;
    QString accessToken; //权限字符串
    QNetworkRequest request; //http请求对象
    QNetworkAccessManager manger; //http应答管理对象
    QNetworkReply *reply; //http应答指针

};

#endif // TEST_H
