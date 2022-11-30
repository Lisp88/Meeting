#ifndef VIDEO_COMMON_H
#define VIDEO_COMMON_H

#include"highgui/highgui.hpp"
#include"imgproc/imgproc.hpp"
#include"core/core.hpp"
#include"opencv2\imgproc\types_c.h"
#include "work_thread.h"
#include <QSharedPointer>

using namespace cv;

//帧率  1秒15帧
#define FRAME_RATE (15)

#define IMAGE_WIDTH (320)
#define IMAGE_HEIGHT (240)

#define IMAGE_QUALITY (60)

#endif // VIDEO_COMMON_H
