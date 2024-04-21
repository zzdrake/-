#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidgetItem>

namespace Ui {
class Login;
}

struct LoginPrivate;
class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    void readXML();

public slots:
    void LogIn();
    void TextChanged();
    void ItemClicked(QListWidgetItem *item);
    void SingIn();
    bool IsSuccessed();

signals:
    void LoginFinish();

protected:
    //事件过滤器
    bool eventFilter(QObject *watched,QEvent *event);
private:
    Ui::Login *ui;
    QLineEdit *user;
    QLineEdit *passwd;
    LoginPrivate *p;
    QString name;
    QString AvatarPath;
};

#endif // LOGIN_H
