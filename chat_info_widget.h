#ifndef CHAT_INFO_WIDGET_H
#define CHAT_INFO_WIDGET_H

#include <QWidget>
#include "QIcon"


namespace Ui {
class chat_info_widget;
}

class chat_info_widget : public QWidget
{
    Q_OBJECT

public:
    explicit chat_info_widget(QWidget *parent = nullptr);
    ~chat_info_widget();

    void set_info(int icon_id, QString name, QString text);
private:
    Ui::chat_info_widget *ui;
};

#endif // CHAT_INFO_WIDGET_H
