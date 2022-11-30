#include "user_show.h"
#include "ui_user_show.h"
#include <QDebug>
user_show::user_show(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::user_show),m_id(0)
{
    ui->setupUi(this);
    qDebug()<<"create user show";
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slot_check_timeout()));
    m_timer.start(1000);
    m_default_img.load(":/resource/images/black.jpg");
    slot_setImage(m_default_img);
}

user_show::~user_show()
{
    delete ui;
}

void user_show::slot_setInfo(int id, QString name)
{
    m_id = id;
    m_name = name;

    ui->lb_name->setText( QString("用户[%1]").arg(m_name) );
}

void user_show::slot_setImage(QImage& img)
{
    m_last_time = QTime::currentTime();
    m_img = img;

    repaint();// 会触发绘图事件，立即触发重绘
}

#include<QPainter>
void user_show::paintEvent(QPaintEvent *event)
{
    //画背景
    QPainter painter( this );  //在这个控件上画, 需要这个对象指针
    painter.setBrush( Qt::black ); //设置黑色画刷
    painter.drawRect( -1 , 0, this->width() +1 , this->height() );//画一个矩形 , 会按照画刷填充

    //画图 (人物或桌面)
    //画图片
    if( m_img.size().height()<= 0 ) return;
    // 加载图片用 QImage , 画图使用 QPixmap
    // 图片缩放 scaled


    QPixmap pixmap = QPixmap::fromImage(m_img.scaled( QSize( this->width(), this->height() - ui->lb_name->height()), Qt::KeepAspectRatio ));
    //画的位置
    int x = this->width() - pixmap.width();
    int y = this->height() - pixmap.height() - ui->lb_name->height();
    x = x /2;
    y = ui->lb_name->height() + y/2;

    painter.drawPixmap( QPoint(x,y) , pixmap );
    painter.end();

    event->accept();
}

void user_show::mousePressEvent(QMouseEvent *event)
{
    Q_EMIT signal_user_clicked(m_id, m_name);
    event->accept();
}

void user_show::slot_check_timeout()
{
    if(m_last_time.secsTo(QTime::currentTime()) > 3){
        //重绘默认图片
        slot_setImage(m_default_img);
    }

}
