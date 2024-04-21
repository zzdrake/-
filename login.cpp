#include "login.h"
#include "ui_login.h"
#include "widget.h"
#include <QLineEdit>
#include <QAction> //行为对象
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox> //消息对话框
#include <QtXml>
#include <QCompleter>
#include <QListWidget>
#include <map>
#include <QEvent>
#include "accountwidget.h"
#include "singin.h"

struct LoginPrivate
{
    QDomDocument doc;
    std::map<QString,QDomElement> AccountList;
    QListWidget* listW;
};

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login),p(new LoginPrivate)
{
    ui->setupUi(this);
    //设置界面固定大小
    this->setFixedSize(650,450);

    //Qt对象树机制:父界面释放，会同时将子界面指针释放
    user = new QLineEdit(this);
    user->move(150,205);
    user->setMinimumWidth(325);
    user->setMinimumHeight(50);
    user->setFont(QFont("华文隶书",15,1));

    passwd = new QLineEdit(this);
    passwd->move(150,260);
    passwd->setMinimumWidth(325);
    passwd->setMinimumHeight(50);
    passwd->setFont(QFont("华文隶书",10,1));
    passwd->setEchoMode(QLineEdit::Password);

    QAction* qq = new QAction(user);
    qq->setIcon(QIcon(":/image/qq.png"));
    user->addAction(qq,QLineEdit::LeadingPosition);

    QAction* pwd = new QAction(user);
    pwd->setIcon(QIcon(":/image/lock.png"));
    passwd->addAction(pwd,QLineEdit::LeadingPosition);

    QCheckBox *autoLogin = new QCheckBox(this);
    autoLogin->setText("自动登录");
    autoLogin->move(150,315);

    QCheckBox *RemPasswd = new QCheckBox(this);
    RemPasswd->setText("记住密码");
    RemPasswd->move(260,315);

    QPushButton *loginBtn = new QPushButton(this);
    loginBtn->setText("登录");
    loginBtn->move(150,350);
    loginBtn->setMinimumSize(325,50);
    loginBtn->setFont(QFont("仿宋",12,10));
    loginBtn->setStyleSheet("QPushButton{background-color:rgb(53,218,255);color:#ffffff;font-size:20;border-radius:0px;}"
                            " QPushButton:hover{background-color:rgba(53,218,255,0.7);");

    QPushButton *registerBtn = new QPushButton(this);
    registerBtn->setText("注册账号");
    registerBtn->move(25,400);
    registerBtn->setMinimumSize(50,25);
    registerBtn->setFont(QFont("仿宋",12,10));
    registerBtn->setStyleSheet("background: transparent; border: none;");
    registerBtn->setStyleSheet("QPushButton:hover{color:rgba(5,142,200,0.7);}");

    connect(registerBtn,&QPushButton::clicked,this,&Login::SingIn);

    connect(loginBtn,&QPushButton::clicked,this,&Login::LogIn);

    readXML();

    p->listW = new QListWidget(this);
    connect(p->listW,&QListWidget::itemClicked,this,&Login::ItemClicked);

    connect(user,&QLineEdit::textChanged,this,&Login::TextChanged);

    //将所有的控件设置到事件过滤器中
    qApp->installEventFilter(this);
    p->listW->removeEventFilter(this);
}

Login::~Login()
{
    delete ui;
    delete p;
}

void Login::LogIn()
{
    if(Login::IsSuccessed() == true)
    {
        QMessageBox::information(this,"提示","登录成功");

        Widget *w = new Widget;
        w->show();


        emit LoginFinish();
        this->close();
    }
    else
    {
        QMessageBox::critical(this,"警告","账号或密码错误请重新输入");
        user->clear();
        passwd->clear();
    }
}

void Login::TextChanged()
{
    p->listW->clear();
    p->listW->move(user->x(),user->y() + user->height());
    for(auto ite = p->AccountList.begin(); ite != p->AccountList.end(); ite++)
    {
        //如果账号包含输入的文本
        if(ite->first.contains(user->text()) == true)
        {
            AccountWidget *a = new AccountWidget(p->listW);
            a->SetName(ite->second.firstChildElement("姓名").text());
            a->SetAccount(ite->first);
            a->SetPicture(QPixmap(ite->second.firstChildElement("头像路径").text()));
            QListWidgetItem *item = new QListWidgetItem(p->listW);
            item->setSizeHint(QSize(a->width(),a->height()));
            p->listW->setItemWidget(item,a);
        }
    }

    p->listW->move(user->x(),user->y() + user->height());
    p->listW->setFixedWidth(user->width());
    p->listW->show();
}

void Login::ItemClicked(QListWidgetItem *item)
{
    QWidget *w = p->listW->itemWidget(item);
    AccountWidget *a = dynamic_cast<AccountWidget*>(w);
    user->setText(a->GetAccount());
}

void Login::SingIn()
{
    Singin *s = new Singin;
    s->show();
}

bool Login::IsSuccessed()
{
    QFile file("C:\\Users\\86151\\Desktop\\Login.xml");
    if(file.open(QIODevice::ReadOnly) == false)
    {
        qDebug()<<"文件打开失败\n";
        return false;
    }
    auto content = file.readAll();
    file.close();

    QString errorString;
    int row,column;
    if(p->doc.setContent(content,&errorString,&row,&column) == false)
    {
        qDebug()<<"xml格式错误"<<errorString<<":"<<row<<" "<<column<<endl;
        return false;
    }
    auto root = p->doc.documentElement();
    for (auto child = root.firstChild();child.isNull() == false;child = child.nextSibling())
    {
        if(child.firstChildElement("账号").text() == user->text() && child.firstChildElement("密码").text() == passwd->text())
        {
            this->name = child.firstChildElement("昵称").text();
            this->AvatarPath = child.firstChildElement("头像路径").text();
            return true;
        }
    }
    return false;
}

bool Login::eventFilter(QObject *watched, QEvent *event) //watched表示当前界面中的焦点是谁
{
    if(event->type() == QEvent::MouseButtonPress && watched != user)
    {
        QTimer::singleShot(100,p->listW,&QWidget::hide);
        //清空焦点，失焦
        user->clearFocus();
    }
    return QObject::eventFilter(watched,event);
}

void Login::readXML()
{
    QFile file("C:\\Users\\86151\\Desktop\\Login.xml");
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

    auto root = p->doc.documentElement();
    for(auto childNode = root.firstChild(); childNode.isNull() == false; childNode = childNode.nextSibling())
    {
        p->AccountList[childNode.firstChildElement("账号").text()] = childNode.toElement();
    }
}

