#ifndef DESK_READ_H
#define DESK_READ_H

#include <QObject>
#include <QTimer>
#include "video_common.h"
#include <QScreen>
#include <QImage>
#include <QPixmap>
#include <QApplication>
#include <QBuffer>
#include <QDesktopWidget>

class Desk_Read;

class Desk_Thread : public work_thread{
    Q_OBJECT
public:
    void setMp_desk_read(Desk_Read *newMp_desk_read);
public slots:
    void slot_thread_read_more();
private:
    Desk_Read *mp_desk_read;
};


class Desk_Read : public QObject
{
    Q_OBJECT
public:
    explicit Desk_Read(QObject *parent = nullptr);
    ~Desk_Read();
signals:
    void signal_screen(QImage img);//多线程，从引用改为拷贝
    void signal_screen_data(QByteArray ba);
public slots:
    void start();
    void pause();
    void slot_get_screen();
private:
    QTimer* m_timer;
    QSharedPointer<Desk_Thread> sp_desk_thread;
};

#endif // DESK_READ_H
