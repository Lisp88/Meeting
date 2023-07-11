#ifndef ROOM_DIALOG_H
#define ROOM_DIALOG_H

#include <QDialog>
#include <QVBoxLayout>//做垂直布局
#include "customwidget.h"
#include "user_show.h"
#include "chat_dialog.h"

namespace Ui {
class room_dialog;
}

class room_dialog : public CustomMoveDialog
{
    Q_OBJECT

public:
    explicit room_dialog(QWidget *parent = nullptr);
    ~room_dialog();
//slots:
    void set_room_id(int room_id);

    void add_user(QWidget* user);

    void remove_user(QWidget* user);

    void room_clear();

    void set_big_image(int user_id, QImage& img);
signals:
    void signal_quit_meeting();

    void signal_audio_open();

    void signal_video_open();

    void signal_video_close();

    void signal_audio_close();

    void signal_desk_close();

    void signal_desk_open();

    void signal_pic_id(int id);

    void signal_open_chat();
public slots:
    void slot_set_big_image_info(int user_id, QString name);
private slots:
    void on_pb_min_clicked();

    void on_pb_max_clicked();

    void on_pb_exit_2_clicked();

    void on_pb_hide_clicked();

    void on_pb_exit_meeting_clicked();

    void on_cb_capture_clicked();

    void on_cb_audio_clicked();

    void on_cb_desk_clicked();

    void on_cbx_lvjing_currentIndexChanged(int index);

    void on_pushButton_clicked();

private:
    Ui::room_dialog *ui;
    QVBoxLayout* m_user_layout;
    chat_dialog* m_chat_dialog;
};

#endif // ROOM_DIALOG_H
