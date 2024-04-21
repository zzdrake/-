#include "lrc.h"
#include "ui_lrc.h"

lrc::lrc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lrc)
{
    ui->setupUi(this);
}

lrc::~lrc()
{
    delete ui;
}
