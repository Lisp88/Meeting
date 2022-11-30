#ifndef AUDIO_COMMON_H
#define AUDIO_COMMON_H


//使用QT媒体库，和音频API
#include <QAudioInput>
#include <QAudioOutput>

#include <QAudioFormat>//音频格式
#include <QAudioDeviceInfo>//音频设备信息

//编码解码库 头文件
#include <speex/include/speex.h>
extern "C" {
#include <WebRtc_Vad/webrtc_vad.h>
}

#define USE_SPEEX   1
//设置压缩质量
#define SPEEX_QUALITY (8)
//VAD
#define USE_VAD     1

#endif // AUDIO_COMMON_H
