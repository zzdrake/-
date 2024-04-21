#include "singin.h"
#include "ui_singin.h"
#include <QMessageBox>
#include <QtXml>
#include <QSqlError>
#include <QSqlQuery>
#include <QRandomGenerator>

struct WidgetPrivate
{
     QSqlDatabase db;
};

Singin::Singin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Singin),p(new WidgetPrivate)
{
    ui->setupUi(this);

    p->db = QSqlDatabase::addDatabase("QMYSQL");
    p->db.setPort(3306);
    p->db.setHostName("192.168.15.128");
    p->db.setUserName("root");
    p->db.setPassword("123456");
    p->db.setDatabaseName("MusicPlayer_user");
}

void Singin::InserIntotXML(const QString &account)
{
    QFile file("C:\\Users\\86151\\Desktop\\Login.xml");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open XML file for reading and writing!");
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        QMessageBox::critical(this, "Error", "Failed to parse XML content!");
        return;
    }

    file.close();

    QDomElement root = doc.documentElement();

    // 创建新用户节点
    QDomElement userNode = doc.createElement("用户");
    QDomElement accountNode = doc.createElement("账号");
    QDomText accountText = doc.createTextNode(account);
    accountNode.appendChild(accountText);
    userNode.appendChild(accountNode);

    //密码节点
    QDomElement PasswdNode = doc.createElement("密码");
    QDomText passwd = doc.createTextNode(ui->lineEdit_setPwd->text());
    PasswdNode.appendChild(passwd);
    userNode.appendChild(PasswdNode);

    //创建名字节点
    QDomElement NameNode = doc.createElement("名字");
    QDomText name = doc.createTextNode(ui->lineEdit_Name->text());
    NameNode.appendChild(name);
    userNode.appendChild(NameNode);

    // 将新用户节点添加到根节点下
    root.appendChild(userNode);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::critical(this, "Error", "Failed to open XML file for writing!");
        return;
    }

    QTextStream out(&file);
    doc.save(out, 4); // Save XML document with indentation of 4 spaces

    file.close();
}


Singin::~Singin()
{
    delete ui;
}

void Singin::on_pushButton_config_clicked()
{
    QRandomGenerator::securelySeeded();

    int randomAccount = QRandomGenerator::global()->bounded(100000,999999); //生成随机的六位数账号

    QString randomString = QString::number(randomAccount);

    InserIntotXML(randomString);

    QMessageBox::information(this,"您注册的账号为："," "+randomString);
}
