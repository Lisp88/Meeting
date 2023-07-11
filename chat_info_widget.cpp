#include "chat_info_widget.h"
#include "ui_chat_info_widget.h"

chat_info_widget::chat_info_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chat_info_widget)
{
    ui->setupUi(this);
}

chat_info_widget::~chat_info_widget()
{
    delete ui;
}

void chat_info_widget::set_info(int icon_id, QString name, QString text)
{
    ui->toolButton->setIcon(QIcon(QString(":/resource/tx/%1.png").arg(icon_id)));
    ui->toolButton->setText(name);
    ui->label->setText(text);
}
