#include "streamplay.h"
#include "ui_streamplay.h"

StreamPlay::StreamPlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamPlay)
{
    ui->setupUi(this);

    ui->last->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->mode->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->next->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");
    ui->play->setStyleSheet("QPushButton{border:none}" "QPushButton{background-color:transparent}");

    //设置按钮的初始状态
    QIcon last(":/image/last.png");
    QPixmap lastmap = last.pixmap(ui->last->size());
    ui->last->setIcon(QIcon(lastmap));
    ui->last->setIconSize(ui->last->size());

    QIcon next(":/image/next.png");
    QPixmap nextmap = next.pixmap(ui->next->size());
    ui->next->setIcon(QIcon(nextmap));
    ui->next->setIconSize(ui->next->size());

    QIcon order(":/image/order.png");
    QPixmap ordermap = order.pixmap(ui->mode->size());
    ui->mode->setIcon(QIcon(ordermap));
    ui->mode->setIconSize(ui->mode->size());

    QIcon play(":/image/play.png");
    QPixmap playmap = play.pixmap(ui->play->size());
    ui->play->setIcon(QIcon(playmap));
    ui->play->setIconSize(ui->play->size());
}

StreamPlay::~StreamPlay()
{
    delete ui;
}
