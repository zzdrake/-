#ifndef NetworkConfigWidgetWIDGET_H
#define NetworkConfigWidgetWIDGET_H

#include <QWidget>

namespace Ui {
class NetworkConfigWidget;
}

struct NetworkConfigWidgetPrivate;

class NetworkConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkConfigWidget(QWidget *parent = nullptr);
    ~NetworkConfigWidget();
    QString GetIP();
    int GetPort();

signals:
    void PBOK_Press();

public slots:
    void PushButton_ok_clicked();

private:
    Ui::NetworkConfigWidget *ui;
    NetworkConfigWidgetPrivate *p;
};

#endif // WIDGET_H
