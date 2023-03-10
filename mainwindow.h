#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkDatagram>
#include <QObject>
#include <QUdpSocket>
#include <iostream>
#include <string>
#include <vector>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <algorithm>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void handle_incoming();
    void parse (std::string s);
    void change_bc_port();
    void change_command_ip();
    void change_command_port();
    void go_up();
    void go_right();
    void go_left();
    void go_down();
    void set_field();
    void draw_element();
    QString algo_solve();
    void count(int i, int j, int &ans);
    void reset_count();
    

private:
    Ui::MainWindow *ui;
    QUdpSocket udp_r;
    QUdpSocket udp_s;
    QString command_ip;
    qint16 command_port;
    QString score_blue;
    QString score_red;
    qint16 cell_num;
    qint16 port_broadcast;
    qint16 port_send;
    QString ip_command;
    QString packet;
    QGraphicsScene *scene;
    qint16 length_scene;
    std::vector<std::vector<int*>> map;
    // std::vector<std::vector<int>> map_tmp;
    std::vector<int*> row_tmp;
    qint16 head_x_blue;
    qint16 head_y_blue;
    qint16 head_x_red;
    qint16 head_y_red;
    QString team_color;
    

    


};
#endif // MAINWINDOW_H
