#ifndef SINGIN_H
#define SINGIN_H

#include <QWidget>

namespace Ui {
class Singin;
}
struct WidgetPrivate;
class Singin : public QWidget
{
    Q_OBJECT

public:
    explicit Singin(QWidget *parent = nullptr);
    void InserIntotXML(const QString &account);
    ~Singin();

private slots:
    void on_pushButton_config_clicked();

private:
    Ui::Singin *ui;
    WidgetPrivate *p;
};

#endif // SINGIN_H
