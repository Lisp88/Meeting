#include "audio_write.h"
#include "QMessageBox"
#include "QDebug"
Audio_Write::Audio_Write(QObject *parent) : QObject(parent)
{
    //初始化设备
    m_format;            //音频采样格式，初始化设备
    m_format.setSampleRate(8000);    //采样率 8000
    m_format.setChannelCount(1);     //声道数 1
    m_format.setSampleSize(16);      //采样位宽16
    m_format.setCodec("audio/pcm");  //音频格式 pcm
    m_format.setByteOrder(QAudioFormat::LittleEndian);   //设置字节序 小端存储
    m_format.setSampleType(QAudioFormat::UnSignedInt);   //采样类型 无符号整形
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();    //默认输入设备
    if (!info.isFormatSupported(m_format)) {//按照该格式打开音频设备
        QMessageBox::information(NULL , "提示", "打开音频设备失败");
        m_format = info.nearestFormat(m_format);
    }
    m_audio_out = new QAudioOutput(m_format, this);//音频输出设备绑定音频格式
    m_buffer_out = m_audio_out->start();//缓冲区绑定音频输出对象
    //buffer中有什么数据 audio_out就输出什么数据

    //解码初始化
    speex_bits_init(&bits_dec);
    Dec_State = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));
}

Audio_Write::~Audio_Write()
{
    delete m_audio_out;
}

void Audio_Write::slot_audio_out(QByteArray ba)
{
#ifndef USE_SPEEX
    m_buffer_out->write(ba.data(), 640);
#else
    char bytes[800] = {0};
    int i = 0;
    float output_frame1[320] = {0};
    char buf[800] = {0};
    memcpy(bytes,ba.data(),ba.length() / 2);
    //解压缩数据 106 62
    //speex_bits_reset(&bits_dec);
    speex_bits_read_from(&bits_dec,bytes,ba.length() / 2);
    int error = speex_decode(Dec_State,&bits_dec,output_frame1);
    //将解压后数据转换为声卡识别格式
    //大端转为小端 从高地址截断
    short num = 0;
    for (i = 0;i < 160;i++)
    {
        num = output_frame1[i];
        buf[2 * i] = num;
        buf[2 * i + 1] = num >> 8;
        //qDebug() << "float out" << num << output_frame1[i];
    }
    memcpy(bytes,ba.data() + ba.length() / 2,ba.length() / 2);
    //解压缩数据
    //speex_bits_reset(&bits_dec);
    speex_bits_read_from(&bits_dec,bytes,ba.length() / 2);
    error = speex_decode(Dec_State,&bits_dec,output_frame1);
    //将解压后数据转换为声卡识别格式
    //大端
    for (i = 0;i < 160;i++)
    {
        num = output_frame1[i];
        buf[2 * i + 320] = num;
        buf[2 * i + 1 + 320] = num >> 8;
    }
    m_buffer_out->write(buf,640);
    return;
#endif
}
