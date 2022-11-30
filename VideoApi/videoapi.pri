HEADERS += \
    $$PWD/desk_read.h \
    $$PWD/facedetect.h \
    $$PWD/video_common.h \
    $$PWD/video_read.h

SOURCES += \
    $$PWD/desk_read.cpp \
    $$PWD/facedetect.cpp \
    $$PWD/video_read.cpp


#使用 opencv 4.2.0
INCLUDEPATH+=C:/Qt/opencv-release/include/opencv2\
    C:/Qt/opencv-release/include

LIBS+=C:\Qt\opencv-release\lib\libopencv_calib3d420.dll.a\
    C:\Qt\opencv-release\lib\libopencv_core420.dll.a\
    C:\Qt\opencv-release\lib\libopencv_features2d420.dll.a\
    C:\Qt\opencv-release\lib\libopencv_flann420.dll.a\
    C:\Qt\opencv-release\lib\libopencv_highgui420.dll.a\
    C:\Qt\opencv-release\lib\libopencv_imgproc420.dll.a\
    C:\Qt\opencv-release\lib\libopencv_ml420.dll.a\
    C:\Qt\opencv-release\lib\libopencv_objdetect420.dll.a\
    C:\Qt\opencv-release\lib\libopencv_video420.dll.a\
    C:\Qt\opencv-release\lib\libopencv_videoio420.dll.a
