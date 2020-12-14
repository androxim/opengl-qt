#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget.h"
#include "thinkgear.h"
#include <QStringListModel>

class MyWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyWidget* openglw;
    QTimer* mindwt;
    QTimer* atten_timer;
    bool mwconnected;
    int connectionId, mw_attent, curr_att, prev_att;
    double angle_change = 0;
    float scale = 1;

    QStringList strList1;
    QStringListModel *strListM1;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    
    void mindwaveconnect();
    void printdata(QString str);

private slots:

    void mindwtUpdate();

    void atten_timerUpdate();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
