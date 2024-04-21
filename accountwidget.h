#ifndef ACCOUNTWIDGET_H
#define ACCOUNTWIDGET_H

#include <QWidget>

namespace Ui {
class AccountWidget;
}

class AccountWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AccountWidget(QWidget *parent = nullptr);
    void SetName(const QString& name);
    void SetAccount(const QString& account);
    void SetPicture(const QPixmap& pixmap);
    QString GetAccount();
    ~AccountWidget();


private:
    Ui::AccountWidget *ui;
};

#endif // ACCOUNTWIDGET_H
