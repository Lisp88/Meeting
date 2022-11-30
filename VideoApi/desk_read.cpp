#include "desk_read.h"

Desk_Read::Desk_Read(QObject *parent) : QObject(parent), m_timer(new QTimer), sp_desk_thread(new Desk_Thread)
{
    sp_desk_thread->setMp_desk_read(this);
    connect(m_timer, SIGNAL(timeout()), sp_desk_thread.get(), SLOT(slot_thread_read_more()));
}

Desk_Read::~Desk_Read()
{
    if(m_timer){
        delete m_timer;
        m_timer = nullptr;
    }
}

void Desk_Read::start()
{
    m_timer->start(1000/FRAME_RATE);
}

void Desk_Read::pause()
{
    m_timer->stop();
}

void Desk_Read::slot_get_screen()
{
    QScreen* src = QApplication::primaryScreen();
    QPixmap map = src->grabWindow(QApplication::desktop()->winId());

    QImage image = map.toImage();
    image = image.scaled(1600, 900, Qt::KeepAspectRatio);

    Q_EMIT signal_screen(image);

    QByteArray ba;
    QBuffer qbuf(&ba);
    image.save(&qbuf, "JPEG", IMAGE_QUALITY);

    Q_EMIT signal_screen_data(ba);
}

void Desk_Thread::setMp_desk_read(Desk_Read *newMp_desk_read)
{
    mp_desk_read = newMp_desk_read;
}

void Desk_Thread::slot_thread_read_more()
{
    mp_desk_read->slot_get_screen();
}
