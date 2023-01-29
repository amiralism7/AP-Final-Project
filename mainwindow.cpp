#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , port_broadcast{1234}
    , command_ip{"192.168.100.62"}
    , command_port{1235}
{
    ui->setupUi(this);


    udp_r.bind(QHostAddress{"0.0.0.0"}, port_broadcast);
    connect(&udp_r, &QUdpSocket::readyRead, this, &MainWindow::handle_incoming);


    connect(ui->lineEdit_port_bc, &QLineEdit::editingFinished, this, &MainWindow::change_bc_port);
    connect(ui->lineEdit_command_ip, &QLineEdit::editingFinished, this, &MainWindow::change_command_ip);
    connect(ui->lineEdit_port, &QLineEdit::editingFinished, this, &MainWindow::change_command_port);
    
    connect(ui->up_button, &QPushButton::clicked, this, &MainWindow::go_up);
    connect(ui->right_button, &QPushButton::clicked, this, &MainWindow::go_right);
    connect(ui->left_button, &QPushButton::clicked, this, &MainWindow::go_left);
    connect(ui->down_button, &QPushButton::clicked, this, &MainWindow::go_down);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::parse (std::string s)
{
    std::vector<std::string> values;
    std::string token;
    size_t pos;
    std::string line{s};
    line.erase(s.length()-1, s.length());
    line.erase(0, 1);
    while ((pos = line.find(":")) != std::string::npos) 
    {
        token = line.substr(0, pos);
        // std::cout << token << "---";
        values.push_back(token);
        line.erase(0, pos + 1);
    }
    values.push_back(line);
    
    values[1] = values[1].substr(1, values[1].length()- 15);
    values[2] = values[2].substr(1, values[2].length()- 15);
    values[3] = values[3].substr(0, values[3].length()- 11);
    values[4] = values[4].substr(0, values[4].length()- 13);
    values[5] = values[5].substr(1, values[5].length()- 8);
    values[6] = values[6].substr(1, values[6].length()- 14);
    values[7] = values[7].substr(1, values[7].length()- 14);
    values.erase(values.begin());

//  Now each element in values is a string containing information
    score_blue = QString::fromStdString(values[2]);
    score_red = QString::fromStdString(values[7]);

    ui->red_score_label->setText(score_red);
    ui->our_score_label->setText(score_blue);


    for (auto x:values)
        std::cout << x << "     ";
    std::cout << std::endl;
}

void MainWindow::change_bc_port()
{
    // lineEdit_port_bc
    port_broadcast = static_cast<short int>(ui->lineEdit_port_bc->text().toInt());
    // didnt work
    // QUdpSocket udp_r2;
    // udp_r2.bind(QHostAddress{"0.0.0.0"}, port_broadcast);
    // std::swap(udp_r2, udp_r)
}

void MainWindow::change_command_ip()
{
    command_ip = ui->lineEdit_command_ip->text();
}

void MainWindow::change_command_port()
{
    command_port = static_cast<short int>(ui->lineEdit_port->text().toInt());
    
}

void MainWindow::go_up()
{
    packet = "Up";
    udp_s.writeDatagram(packet.toStdString().c_str(), packet.length(), QHostAddress{command_ip}, command_port);
    qDebug() << " ";
    qDebug() << "sending to ip: " << command_ip << " and port: " << command_port << "and message: " << packet.toStdString().c_str() << "with length: " << packet.length();
}
void MainWindow::go_right()
{
    packet = "Right";
    udp_s.writeDatagram(packet.toStdString().c_str(), packet.length(), QHostAddress{command_ip}, command_port);
    qDebug() << " ";
    qDebug() << "sending to ip: " << command_ip << " and port: " << command_port << "and message: " << packet.toStdString().c_str() << "with length: " << packet.length();
}
void MainWindow::go_left()
{
    packet = "Left";
    udp_s.writeDatagram(packet.toStdString().c_str(), packet.length(), QHostAddress{command_ip}, command_port);
    qDebug() << " ";
    qDebug() << "sending to ip: " << command_ip << " and port: " << command_port << "and message: " << packet.toStdString().c_str() << "with length: " << packet.length();
}
void MainWindow::go_down()
{
    packet = "Down";
    udp_s.writeDatagram(packet.toStdString().c_str(), packet.length(), QHostAddress{command_ip}, command_port);
    qDebug() << " ";
    qDebug() << "sending to ip: " << command_ip << " and port: " << command_port << "and message: " << packet.toStdString().c_str() << "with length: " << packet.length();
}


