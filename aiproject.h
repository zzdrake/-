#ifndef AIPROJECT_H
#define AIPROJECT_H

#include <QWidget>

namespace Ui {
class AIProject;
}

class AIProject : public QWidget
{
    Q_OBJECT

public:
    explicit AIProject(QWidget *parent = nullptr);
    ~AIProject();

private:
    Ui::AIProject *ui;
};

#endif // AIPROJECT_H
