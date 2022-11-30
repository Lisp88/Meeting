#include "audio_read.h"
#include "QMessageBox"
Audio_Read::Audio_Read(QObject *parent) : QObject(parent), m_record_state(state::_stop), m_timer(nullptr), m_audio_in(nullptr), m_buffer_in(nullptr)
{
    //声卡设置
    //m_format;            //音频采样格式
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
    m_record_state = state::_stop;

    //speex 初始化
    speex_bits_init(&bits_enc);
    Enc_State = speex_encoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));

    //设置质量为 8(15kbps)
    int tmp = SPEEX_QUALITY;
    speex_encoder_ctl(Enc_State,SPEEX_SET_QUALITY,&tmp);

    //静音检测初始化
#ifdef USE_VAD
    if(WebRtcVad_Create(&handle)){
        qDebug()<<"vad create fail";
    }
    if(WebRtcVad_Init(handle)){
        qDebug()<<"vad init fail";
    }
#endif
}

Audio_Read::~Audio_Read()
{
    if(m_audio_in){
        m_audio_in->stop();
        delete m_audio_in;
        m_audio_in = nullptr;
    }
    if(m_timer){
        m_timer->stop();
        delete m_timer;
    }
#ifdef USE_VAD
 WebRtcVad_Free(handle);
#endif
}

void Audio_Read::start()
{
    if (m_record_state == state::_record)
        return;
    m_audio_in = new QAudioInput(m_format, this);
    m_buffer_in = m_audio_in->start();//声音采集开始，绑定缓存区和音频设备
    m_timer = new QTimer(this);//每隔一段时间从缓冲区提取数据

    connect(m_timer, &QTimer::timeout, this, &Audio_Read::slot_read_more);
    m_timer->start(20);//20ms
    m_record_state = state::_record;
    qDebug()<<"audio read start";
}

#include <QDebug>
//从缓冲区中读够640Byte
void Audio_Read::slot_read_more()
{
#ifdef USE_SPEEX
    QByteArray m_buffer(4096,0);//字节数组
    qint64 len = m_audio_in->bytesReady();

    if (len < 640){
        //qDebug()<<"size not enough 640";
        return;
    }//字节足够640之后 进行转码 小于 质量不行
    //qDebug()<<"compress pre size :"<<len;

    m_buffer_in->read(m_buffer.data(), 640);//字节读入缓冲区
    QByteArray frame;
    //frame.append(m_buffer.data(),640);
    //编码器编码
    char bytes[800] = {0};
    int i = 0;
    float input_frame1[320];
    char buf[800] = {0};
    char* pInData = (char*)m_buffer.data();
    memcpy( buf , pInData , 640);

    //静音检测后再编码
#ifdef USE_VAD
    int count = 0;
    for (int i = 0; i < 640; i += 320){
        char* temp = buf + i;
        //vad返回值0静音
        if(WebRtcVad_Process(handle, 8000, (int16_t*)temp, 160) == 0){
            memset(temp, 0, 320);
            ++count;
        }
    }
    if(count >= 2) return ;
#endif

    //speek 宅带宽模式 8khz 每次编码320字节  大端存储（网络传输字节序）
    //PCM 采集为小端
    int nbytes = 0;
    {
        short num = 0;
        for (i = 0;i < 160;i++)
        {
            num = (uint8_t)buf[2 * i] | (((uint8_t)buf[2 * i + 1]) << 8);
            input_frame1[i] = num;
            //num = m_buffer[2 * i] | ((short)(m_buffer[2 * i + 1]) << 8);
            //qDebug() << "float in" << num << input_frame1[i];
        }
        //压缩数据 640B -> 76B
        speex_bits_reset(&bits_enc);
        speex_encode(Enc_State,input_frame1,&bits_enc);
        nbytes = speex_bits_write(&bits_enc,bytes,800);
        frame.append(bytes,nbytes);
        //大端
        for (i = 0;i < 160;i++)
        {
            num = (uint8_t)buf[2 * i + 320] | (((uint8_t)buf[2 * i + 1 + 320]) << 8);
            input_frame1[i] = num;
        }
        //压缩数据
        speex_bits_reset(&bits_enc);
        speex_encode(Enc_State,input_frame1,&bits_enc);
        nbytes = speex_bits_write(&bits_enc,bytes,800);
        frame.append(bytes,nbytes);
        //qDebug() << "compress after size :" << frame.size();
        Q_EMIT signal_audio_frame(frame);
        return;
    }
    //Q_EMIT signal_audio_frame( frame );
#else
    QByteArray m_buffer(4096,0);//字节数组
    qint64 len = m_audio_in->bytesReady();
    if (len < 640){
        qDebug()<<"size not enough 640";
        return;
    }//字节足够640之后 进行转码 小于 质量不行


    m_buffer_in->read(m_buffer.data(), 640);//字节读入缓冲区
    QByteArray frame;
    qDebug()<<"device read data";
    frame.append(m_buffer.data(),640);

    Q_EMIT signal_audio_frame( frame );
#endif
}

void Audio_Read::pause()
{
    if(m_record_state == state::_record){
        if(m_audio_in){
            m_audio_in->stop();
            //声音采集恢复
            delete m_audio_in;
            m_audio_in = nullptr;
        }
        if(m_timer){
            m_timer->stop();
            delete m_timer;
            m_timer = nullptr;
        }

    }
    m_record_state = state::_pause;
}
