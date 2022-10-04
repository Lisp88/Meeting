#ifndef MAIN_WINDOWS_H
#define MAIN_WINDOWS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class main_windows; }
QT_END_NAMESPACE

class main_windows : public QMainWindow
{
    Q_OBJECT

public:
    main_windows(QWidget *parent = nullptr);
    ~main_windows();

private:
    Ui::main_windows *ui;
};
#endif // MAIN_WINDOWS_H
