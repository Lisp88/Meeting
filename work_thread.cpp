#include "work_thread.h"

work_thread::work_thread(QObject *parent) : QObject(parent), mp_thread(new QThread)
{
    this->moveToThread(mp_thread);

    mp_thread->start();
}
#include <QDebug>
work_thread::~work_thread()
{
    if(mp_thread){
        mp_thread->quit();
        mp_thread->wait(100);
        if(mp_thread->isRunning()){
            mp_thread->terminate();
        }
        delete mp_thread;
        mp_thread = nullptr;
    }
    qDebug()<<"work_thread::~work_thread()";
}
