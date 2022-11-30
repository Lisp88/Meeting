#ifndef SET_USER_H
#define SET_USER_H

#include <QDialog>

namespace Ui {
class set_user;
}

class set_user : public QDialog
{
    Q_OBJECT

public:
    explicit set_user(QWidget *parent = nullptr);
    ~set_user();
    void set_info(int icon_id, QString name, QString feeling);
signals:
    void signal_commit_info(int icon_id, QString name, QString feeling = "比较懒，啥也没写");

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::set_user *ui;
    int m_icon_id;
    QString m_name;
    QString m_feeling;
};

#endif // SET_USER_H
