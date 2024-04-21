#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include <QFile>
#include <QTextCodec>

namespace Ui {
class Widget;
}

struct WidgetPrivate;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void LoadMusicFile(const QString &path);

    QByteArray FileToJson(const char* packet,int JSONFLAG);

    void SendJsonToClient();

private:
    Ui::Widget *ui;
    WidgetPrivate* p;

    QTcpServer *s;
    QTcpSocket *socket;
    QTcpSocket *StSocket;
};
#endif // WIDGET_H
