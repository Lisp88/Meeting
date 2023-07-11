#include "facedetect.h"
#include "objdetect/objdetect.hpp"
#include<QCoreApplication>
#include<qDebug>

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name_face = "Capture - Face";

FaceDetect::FaceDetect()
{

}

void FaceDetect::FaceDetectInit()
{
    // 将 xml 文件放在 exe 同级的目录下面
     QString face_cascade_name = QCoreApplication::applicationDirPath()
     +"/haarcascade_frontalface_default.xml";

     QString eyes_cascade_name = QCoreApplication::applicationDirPath()
      +"/haarcascade_eye.xml";

     // 根据路径加载 xml 文件
     qDebug() << face_cascade_name;
     //-- 1. Load the cascade
     if( !face_cascade.load( face_cascade_name.toStdString() ) )
     {
         qDebug()<< "--(!)Error loading face " ;
         return;
     }
     qDebug() << eyes_cascade_name;
     if( !eyes_cascade.load( eyes_cascade_name.toStdString() ) )
     {
         qDebug()<<"--(!)Error loading eyes " ;
         return;
     }
}

void FaceDetect::detectAndDisplay(Mat &frame, std::vector<Rect> &faces)
{
    Mat frame_gray;
    //首先 , 得到无颜色的图片用于做识别
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- 多尺寸检测人脸
    //第三个参数 每个图像比例下图像大小减少多少的参数。
    //第四个参数 指定每个候选矩形应该为邻居保留多少个像素。
    //第五个参数 参数与函数 cvHaarDetectObjects 中的旧级联具有相同的含义。是新版本还是
    //旧版本
    //第六个参数 最小可能对象大小。小于该值的对象将被忽略。
    //第七个参数 最大可能对象大小。大于该值的对象将被忽略
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 6, 0, Size(100,100) , Size(400, 400) );

    //imshow( "capture_face", frame );
    //为了防止误识别, 再识别眼睛
    for( auto ite = faces.begin() ; ite != faces.end() ; )
    {
        Rect rct = *ite;
        Mat faceROI = frame_gray( rct );
        std::vector<Rect> eyes;
        //-- 在每张人脸上检测双眼
        eyes_cascade.detectMultiScale( faceROI, eyes , 1.1, 2, 0 , Size(20, 20) );
        //正常 鼻子的大小 应该不超过脸的 1/3
        if( eyes.size() != 2 )
        {
            ite = faces.erase(ite);
        }else
        {
            if( rct.height*0.5 < eyes[0].y ){
                ite = faces.erase(ite);
                continue;
            }
            ++ite;
        }
    }
    //绘制识别的人脸矩形
    for( auto ite = faces.begin() ; ite != faces.end() ; ++ite )
    {
        Rect rct = *ite;
        Point center( rct.x + rct.width*0.5, rct.y + rct.height*0.5 );
        ellipse( frame, center, Size( rct.width*0.5, rct.height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    }
    //-- 显示结果图像
    //imshow( window_name_face, frame );
}
