#ifndef LRC_H
#define LRC_H

#include <QWidget>

namespace Ui {
class lrc;
}

class lrc : public QWidget
{
    Q_OBJECT

public:
    explicit lrc(QWidget *parent = nullptr);
    ~lrc();

private:
    Ui::lrc *ui;
};

#endif // LRC_H
