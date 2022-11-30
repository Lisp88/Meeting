#ifndef VIDEO_READ_H
#define VIDEO_READ_H

#include <QObject>

#include "video_common.h"
#include"QMessageBox"
#include<QTimer>
#include<QBuffer>

class Video_Read;

class Video_Thread : public work_thread{
    Q_OBJECT
public:
    void setMp_video_read(Video_Read *new_mp_video_read);
public slots:
    void slot_thread_read_more();
private:
    Video_Read *mp_video_read;
};


//采集摄像头视频
class Video_Read : public QObject
{
    Q_OBJECT
public:
    explicit Video_Read(QObject *parent = nullptr);
    ~Video_Read();
    enum funnypic_type{fp_tuer = 1, fp_hat};
signals:
    void SIG_videoFrame( QImage img );
    void SIG_videoFrameData( QByteArray bt );
public slots:
    void start();
    void pause();
    void slot_readMore();
    void set_pic_id(int id);
private:
    //摄像头对象
    cv::VideoCapture cap;
    QTimer * m_timer;
    int m_funny_type;
    std::vector<Rect> last_faces;
    QImage m_tuer;
    QImage m_hat;

    QSharedPointer<Video_Thread> sp_video_read;
};

#endif // VIDEO_READ_H
