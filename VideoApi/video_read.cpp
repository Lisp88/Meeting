#include "video_read.h"
#include "facedetect.h"
Video_Read::Video_Read(QObject *parent) : QObject(parent), m_funny_type(0), m_tuer(":/resource/images/tuer.png"), m_hat(":/resource/images/hat.png"),
    sp_video_read(new Video_Thread)
{
    m_timer = new QTimer;
    sp_video_read->setMp_video_read(this);
    connect( m_timer , SIGNAL( timeout()) ,
             sp_video_read.get() , SLOT(slot_thread_read_more()));
    FaceDetect::FaceDetectInit();
}



Video_Read::~Video_Read()
{
    m_timer->stop();
    delete m_timer;
}

void Video_Read::start()
{
    //打开摄像头
    cap.open(0);//打开默认摄像头
    if(!cap.isOpened ()){
        QMessageBox::information(NULL,tr("提示"),tr("视频没有打开"));
        return;
    }
    //打开定时器，1s除以帧率
    m_timer->start( 1000/FRAME_RATE );
}

void Video_Read::pause()
{
    //关闭摄像头
    if( cap.isOpened())
        cap.release();
    //关闭定时器
    m_timer->stop();
}

void Video_Read::set_pic_id(int id){
    m_funny_type = id;
}
#include <QPainter>
#include <QDebug>
void Video_Read::slot_readMore()
{
    //获取摄像头
    Mat frame;
    if( !cap. read (frame) )
    {
        return;
    }
    //将 opencv 采集的 BGR 的图片类型转化为 RGB24 的类型
    cvtColor(frame,frame,CV_BGR2RGB);

    std::vector<Rect> faces;
    if(m_funny_type){
        FaceDetect::detectAndDisplay(frame, faces);
    }

    //定义 QImage 对象, 用于发送数据以及图片显示
    QImage image ((unsigned const char*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888);
    //转化为大小更小的图片  等比例缩放
    image = image.scaled( IMAGE_WIDTH,IMAGE_HEIGHT, Qt::KeepAspectRatio );

    //贴图
    QImage* p_img = nullptr;
    switch(m_funny_type){
        case fp_tuer:
            p_img = &m_tuer; break;
        case fp_hat:
            p_img = &m_hat; break;
        default: break;
    }
    if(faces.size()) last_faces = faces;

    //将道具绘制到图片上
    if( p_img )
        if( m_funny_type == fp_tuer || m_funny_type == fp_hat )
        {
            *p_img = p_img->scaled(120,90, Qt::KeepAspectRatio);
            //QPainter 使用
            QPainter paint( &image );
            //遍历所有人脸的矩形, 画道具
            for( int i = 0 ; i < last_faces.size(); ++i)
            {
                Rect rct = last_faces[i];
                int x = rct.x - rct.width*0.5 - p_img -> width()*0.5;
                //20 是图片的尺寸偏移
                int y = rct.y - 2*p_img -> height()-20;
                QPoint p (x , y);
                paint.drawImage( p , * p_img );
            }
        }
    //发送图片  //信号形式发送
    Q_EMIT SIG_videoFrame( image );
    //qDebug()<<"signal image";

    //图片转换为 QByteArray 进行发送
    //压缩图片从 RGB24 格式压缩到 JPEG 格式, 发送出去
    QByteArray ba;
    QBuffer qbuf(&ba); // QBuffer 与 QByteArray 字节数组联立联系
    image.save( &qbuf , "JPEG" , IMAGE_QUALITY ); //将图片的数据写入 ba，第三个参数为质量，可以实现压缩效果(ffmpeg太难)
//    发送图片数据
    Q_EMIT SIG_videoFrameData( ba );
}

void Video_Thread::setMp_video_read(Video_Read *new_mp_video_read)
{
    mp_video_read = new_mp_video_read;
}

void Video_Thread::slot_thread_read_more()
{
    mp_video_read->slot_readMore();
}
