#ifndef AUDIO_READ_H
#define AUDIO_READ_H

#include <QObject>
#include "audio_common.h"
#include "QDebug"
#include "QTimer"
#include "QByteArray"

class Audio_Read : public QObject
{
    Q_OBJECT
public:
    explicit Audio_Read(QObject *parent = nullptr);
    ~Audio_Read();
    enum state{_stop, _record, _pause};

    //开始采集
    void start();
    //暂停采集
    void pause();

signals:
    //定时采集数据，信号触发
    void signal_audio_frame(QByteArray& ba);

public slots:
    //定时超时
    void slot_read_more();

private:
    QAudioFormat m_format;
    QTimer *m_timer;
    QAudioInput* m_audio_in;
    QIODevice* m_buffer_in;

    int m_record_state;

    SpeexBits bits_enc;
    void *Enc_State;
    //静音检测
    VadInst* handle;
};

#endif // AUDIO_READ_H
