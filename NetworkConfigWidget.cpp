#include "NetworkConfigWidget.h"
#include "ui_NetworkConfigWidget.h"
#include <QtXml>
#include <QFile>
#include <QDebug>
#include <map>

struct NetworkConfigWidgetPrivate
{
    QDomDocument doc;
    std::map<QLineEdit*,QDomElement> NodeMap;
};

NetworkConfigWidget::NetworkConfigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkConfigWidget),p(new NetworkConfigWidgetPrivate)
{
    ui->setupUi(this);

    QFile file("C:\\Users\\86151\\Desktop\\test.xml"); //本地配置xml文件
    if(file.open(QIODevice::ReadOnly) == false)
    {
        qDebug()<<"文件打开失败!\n";
        return;
    }
    auto Content = file.readAll();
    file.close();

    //xml文本对象
    QString errorString;
    int row,column;

    if(p->doc.setContent(Content,&errorString,&row,&column) == false)
    {
        qDebug()<<"xml格式错误："<<errorString<<":"<<row<<":"<<column;
        return;
    }
    qDebug()<<"xml读取成功!\n";

    //xml根节点元素
    QDomElement rootNode = p->doc.documentElement();
    qDebug()<<"根节点名称："<<rootNode.nodeName()<<endl;

    ui->lineEdit->setText(rootNode.firstChildElement("ip").text());
    ui->lineEdit_2->setText(rootNode.firstChildElement("port").text());

    p->NodeMap[ui->lineEdit] = rootNode.firstChildElement("ip");
    p->NodeMap[ui->lineEdit_2] = rootNode.firstChildElement("port");
    connect(ui->QPushButton_ok,&QPushButton::clicked,this,
            &NetworkConfigWidget::PushButton_ok_clicked);
}

NetworkConfigWidget::~NetworkConfigWidget()
{
    delete ui;
    delete p;
}

QString NetworkConfigWidget::GetIP()
{
    return ui->lineEdit->text();
}

int NetworkConfigWidget::GetPort()
{
    return ui->lineEdit_2->text().toUShort();
}

void NetworkConfigWidget::PushButton_ok_clicked()
{
    auto ipNode = p->NodeMap.find(ui->lineEdit)->second;
    ipNode.firstChild().setNodeValue(ui->lineEdit->text());

    auto portNode = p->NodeMap.find(ui->lineEdit_2)->second;
    portNode.firstChild().setNodeValue(ui->lineEdit_2->text());


    QFile file("C:\\Users\\86151\\Desktop\\test.xml");
    if(file.open(QIODevice::WriteOnly) == false)
    {
        qDebug()<<"文件打开失败!\n";
        return;
    }

    file.write(p->doc.toByteArray());
    file.close();

    this->close();
    emit PBOK_Press();
}
