#ifndef WORK_THREAD_H
#define WORK_THREAD_H

#include <QObject>
#include <QSharedPointer>
#include <QThread>
class work_thread : public QObject
{
    Q_OBJECT
public:
    explicit work_thread(QObject *parent = nullptr);
    ~work_thread();
private:
    QThread* mp_thread;
};

#endif // WORK_THREAD_H
