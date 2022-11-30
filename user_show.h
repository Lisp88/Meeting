#ifndef user_show_h
#define user_show_h
#include <QWidget>
#include<QPaintEvent>
#include <QTimer>
#include <QTime>
//#include "kernel.h"
namespace Ui {
    class user_show;
}

class Kernel;
class room_dialog;
class user_show : public QWidget
{
    Q_OBJECT

public:
    explicit user_show(QWidget *parent = nullptr);
    ~user_show();
signals:
    void signal_user_clicked(int user_id, QString name);
public slots:
    void slot_setInfo( int id , QString name );
    void slot_setImage(QImage& img );
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void slot_check_timeout();
private:
    Ui::user_show *ui;
    friend class Kernel;
    friend class room_dialog;
    int m_id;
    int m_icon;
    QString m_name;

    QImage m_img;
    //默认背景
    QImage m_default_img;
    QTime m_last_time;
    QTimer m_timer;
};

#endif // user_show_h
