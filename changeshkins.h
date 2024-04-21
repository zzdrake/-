#ifndef CHANGESHKINS_H
#define CHANGESHKINS_H

#include <QWidget>

namespace Ui {
class changeshkins;
}

class changeshkins : public QWidget
{
    Q_OBJECT

public:
    explicit changeshkins(QWidget *parent = nullptr);
    ~changeshkins();

protected:
    void leaveEvent(QEvent *event);
private slots:


    void on_pushButton_red_clicked();

    void on_pushButton_blue_clicked();

    void on_pushButton_pink_clicked();

    void on_pushButton_orange_clicked();

    void on_pushButton_yellow_clicked();

    void on_pushButton_green_clicked();

private:
    Ui::changeshkins *ui;
};

#endif // CHANGESHKINS_H
