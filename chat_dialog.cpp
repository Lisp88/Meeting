#include "chat_dialog.h"
#include "ui_chat_dialog.h"
#include "QDebug"
chat_dialog::chat_dialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::chat_dialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(test()), this, SLOT(test_()));
}

chat_dialog::~chat_dialog()
{
    delete ui;
}

void chat_dialog::add_info(int icon_id, QString name, QString text)
{
    chat_info_widget* ct = new chat_info_widget();
    ct->set_info(icon_id, name, text);
    QListWidgetItem* item = new QListWidgetItem(ui->lw_chat_show);
    item->setSizeHint({ct->width(), 110});
    ui->lw_chat_show->setItemWidget(item, ct);
}

void chat_dialog::close_chat()
{
    ui->lw_chat_show->clear();
}
#include <QTime>
void chat_dialog::on_pb_send_clicked()
{
    QString text = ui->te_chat_input->toPlainText();
    ui->te_chat_input->clear();
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
    text = QString("[%1] %2").arg(time_str).arg(text);
    qDebug() << text;
    Q_EMIT signal_send_info(text);
}


#include <QMessageBox>
void chat_dialog::on_pb_close_chat_clicked()
{
    if(QMessageBox::question(this, "提示", "是否退出当前会议聊天") == QMessageBox::Yes){
        this->close();
    }
}

