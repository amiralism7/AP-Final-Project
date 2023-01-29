#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkDatagram>
#include <QObject>
#include <QUdpSocket>
#include <iostream>
#include <string>
#include <vector>



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
    // {
    //     std::vector<std::string> values;
    //     std::string token;
    //     size_t pos;
    //     std::string line{s};
    //     line.erase(s.length()-1, s.length());
    //     line.erase(0, 1);
    //     while ((pos = line.find(":")) != std::string::npos) 
    //     {
    //         token = line.substr(0, pos);
    //         // std::cout << token << "---";
    //         values.push_back(token);
    //         line.erase(0, pos + 1);
    //     }
    //     values.push_back(line);
        
    //     values[1] = values[1].substr(1, values[1].length()- 15);
    //     values[2] = values[2].substr(1, values[2].length()- 15);
    //     values[3] = values[3].substr(0, values[3].length()- 11);
    //     values[4] = values[4].substr(0, values[4].length()- 13);
    //     values[5] = values[5].substr(1, values[5].length()- 8);
    //     values[6] = values[6].substr(1, values[6].length()- 14);
    //     values[7] = values[7].substr(1, values[7].length()- 14);
    //     values.erase(values.begin());

    // //  Now each element in values is a string containing information
    //     score_blue = QString::fromStdString(values[2]);
    //     score_red = QString::fromStdString(values[7]);

    //     ui->red_score_label->setText(score_red);
    //     ui->our_score_label->setText(score_blue);


    //     for (auto x:values)
    //         std::cout << x << "     ";
    //     std::cout << std::endl;
    // }
    void change_bc_port();
    void change_command_ip();
    void change_command_port();
    void go_up();
    void go_right();
    void go_left();
    void go_down();
    

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


};
#endif // MAINWINDOW_H
