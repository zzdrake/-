#include "aiproject.h"
#include "ui_aiproject.h"

AIProject::AIProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AIProject)
{
    ui->setupUi(this);
}

AIProject::~AIProject()
{
    delete ui;
}
