#include "accountwidget.h"
#include "ui_accountwidget.h"

AccountWidget::AccountWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountWidget)
{
    ui->setupUi(this);
}

void AccountWidget::SetName(const QString &name)
{
    ui->label_name->setText(name);
}

void AccountWidget::SetAccount(const QString &account)
{
    ui->label_account->setText(account);
}

void AccountWidget::SetPicture(const QPixmap &pixmap)
{
    ui->label_picture->setPixmap(pixmap.scaled(ui->label_picture->width(),ui->label_picture->height()));
}

QString AccountWidget::GetAccount()
{
    return ui->label_account->text();
}

AccountWidget::~AccountWidget()
{
    delete ui;
}
