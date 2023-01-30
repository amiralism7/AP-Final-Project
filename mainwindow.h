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

    void handle_incoming()
    {
        QNetworkDatagram recv = udp_r.receiveDatagram();
        // qDebug() << recv.data();
        std::string s{recv.data()};
        MainWindow::parse(s);
        // std::cout << s << std::endl;
        
    }
    void parse (std::string s);
    void change_bc_port();
    void change_command_ip();
    void change_command_port();
    void go_up();
    void go_right();
    void go_left();
    void go_down();
    void set_field();
    void draw_element(qint16 x_red, qint16 y_red, qint16 x_blue, qint16 y_blue);
    

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


};
#endif // MAINWINDOW_H
