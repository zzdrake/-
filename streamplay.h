#ifndef STREAMPLAY_H
#define STREAMPLAY_H

#include <QWidget>
#include <QTcpSocket>
#include <QMediaPlayer>

namespace Ui {
class StreamPlay;
}

class StreamPlay : public QWidget
{
    Q_OBJECT

public:
    explicit StreamPlay(QWidget *parent = nullptr);
    ~StreamPlay();

private:
    Ui::StreamPlay *ui;
    QTcpSocket* socket;
    QMediaPlayer p;
};

#endif // STREAMPLAY_H
