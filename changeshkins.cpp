#include "changeshkins.h"
#include "ui_changeshkins.h"
#include "loadfileqss.h"

changeshkins::changeshkins(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::changeshkins)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground); //设置样式表
    this->setStyleSheet("QWidget{border:1px; background:rgb(255,255,255); border-radius:8px;}"
                        "QWidget:hover{background:rgb(255,255,255);}");
    ui->pushButton_red->setStyleSheet("QPushButton{background:rgb(236,65,65); border:0px;}");
    ui->pushButton_blue->setStyleSheet("QPushButton{background:rgb(57,175,234);border:0px;}");
    ui->pushButton_pink->setStyleSheet("QPushButton{background:rgb(255,122,158);border:0px;}");
    ui->pushButton_orange->setStyleSheet("QPushButton{background:rgb(226,171,18);border:0px;}");
    ui->pushButton_yellow->setStyleSheet("QPushButton{background:rgb(249,238,90);border:0px;}");
    ui->pushButton_green->setStyleSheet("QPushButton{background:rgb(106,204,25);border:0px;}");
}

changeshkins::~changeshkins()
{
    delete ui;
}

void changeshkins::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    this->close();
}

void changeshkins::on_pushButton_red_clicked()
{
     LoadFileQss::setStyle("C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\FileColorQSS\\Red.qss"); //加载qss 样式表，用静态方式
}

void changeshkins::on_pushButton_blue_clicked()
{
     LoadFileQss::setStyle("C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\FileColorQSS\\Blue.qss"); //加载qss 样式表，用静态方式
}

void changeshkins::on_pushButton_pink_clicked()
{
     LoadFileQss::setStyle("C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\FileColorQSS\\Pink.qss"); //加载qss 样式表，用静态方式
}

void changeshkins::on_pushButton_orange_clicked()
{
     LoadFileQss::setStyle("C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\FileColorQSS\\Orange.qss"); //加载qss 样式表，用静态方式
}

void changeshkins::on_pushButton_yellow_clicked()
{
      LoadFileQss::setStyle("C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\FileColorQSS\\Yellow.qss"); //加载qss 样式表，用静态方式
}

void changeshkins::on_pushButton_green_clicked()
{
      LoadFileQss::setStyle("C:\\Users\\86151\\Desktop\\252QT\\musicPlayer\\FileColorQSS\\Green.qss"); //加载qss 样式表，用静态方式
}
