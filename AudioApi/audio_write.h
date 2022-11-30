#ifndef AUDIO_WRITE_H
#define AUDIO_WRITE_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
#include "audio_common.h"
class Audio_Write : public QObject
{
    Q_OBJECT
public:
    explicit Audio_Write(QObject *parent = nullptr);
    ~Audio_Write();


signals:

public slots:
    void slot_audio_out(QByteArray ba);

private:
    QAudioFormat m_format;

    QIODevice* m_buffer_out;
    QAudioOutput* m_audio_out;
    SpeexBits bits_dec;
    void *Dec_State;
};

#endif // AUDIO_WRITE_H
