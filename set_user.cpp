#include "set_user.h"
#include "ui_set_user.h"

set_user::set_user(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::set_user)
{
    ui->setupUi(this);

    for(int i = 0; i < 36; ++i){
        ui->comboBox->addItem(QIcon(QString(":/resource/tx/%1.png").arg(i)), "");
    }
}

set_user::~set_user()
{
    delete ui;
}

void set_user::set_info(int icon_id, QString name, QString feeling)
{
    m_icon_id = icon_id;
    m_name = name;
    m_feeling = feeling;

    ui->comboBox->setCurrentIndex(m_icon_id);
    ui->lineEdit->setText(m_name);
    ui->lineEdit_2->setText(m_feeling);
}
//提交
void set_user::on_pushButton_clicked()
{
    int icon_id = ui->comboBox->currentIndex();
    QString name = ui->lineEdit->text();
    QString feeling = ui->lineEdit_2->text();
    this->hide();
    Q_EMIT signal_commit_info(icon_id, name, feeling);

    set_info(icon_id, name, feeling);

}

//清空
void set_user::on_pushButton_2_clicked()
{
    ui->lb_name->setText("");
    ui->lb_feeling->setText("");
}

