#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , command_ip{"192.168.100.62"}
    , command_port{1235}
    , cell_num{13}
    , port_broadcast{1234}
    , packet{"Right"}
    , length_scene{static_cast<short int> ((static_cast<short int>(600/cell_num))*cell_num)}
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    
    // QGraphicsRectItem *rectangle;
    // QGraphicsLineItem *line;
    // QBrush brush(Qt::black);
    // QPen pen(Qt::black);
    // qint16 part{static_cast<short int> (length_scene/cell_num)};
    // // rectangle = scene->addRect(0, 0, 2 , 2, pen, brush);
    // rectangle = scene->addRect(0, 0, length_scene, part, pen, brush);
    // rectangle = scene->addRect(0, 0, part , length_scene, pen, brush);
    // rectangle = scene->addRect(length_scene-part, 0, part , length_scene, pen, brush);
    // rectangle = scene->addRect( 0, length_scene-part, length_scene , part, pen, brush);
    
    // // line = scene->addLine(x1, y1, x2, y2, pen2);
    // for (size_t i{}; i<=cell_num; i++)
    // {
    //     line = scene->addLine(0, part*i, length_scene, part*i, pen);
    //     line = scene->addLine(part*i, 0, part*i, length_scene, pen);
    // }
    set_field();
    

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
void MainWindow::handle_incoming()
{
    QNetworkDatagram recv = udp_r.receiveDatagram();
    // qDebug() << recv.data();
    std::string s{recv.data()};
    MainWindow::parse(s);
    // std::cout << s << std::endl;


    // for (size_t i{}; i<cell_num; i++)
    // {
    //     for (size_t j{}; j<cell_num; j++)
    //     {
    //         std::cout<< map[i][j] << "    ";
    //     }
    //     std::cout<< std::endl;
    // }
    // std::cout<< "*********************************************" <<std::endl;
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

    cell_num = std::stoi(values[3]);
//  Now each element in values is a string containing information
    score_blue = QString::fromStdString(values[2]);
    score_red = QString::fromStdString(values[7]);

    ui->red_score_label->setText(score_red);
    ui->our_score_label->setText(score_blue);

// Now we wannt to draw
    

    if (std::count(values[1].begin(), values[1].end(), '[') == 2)
    {
        scene->clear();
        ui->graphicsView->items().clear();
        set_field();
    }


    std::string blue_loc {values[1].substr(1, values[1].find("]")-1)};
    std::string red_loc {values[6].substr(1, values[6].find("]")-1)};

    
    std::string blue_x{blue_loc.substr(blue_loc.find(",")+1, blue_loc.length()-blue_loc.find(",")-1)};
    std::string blue_y{blue_loc.substr(0,blue_loc.find(","))};
    std::string red_x{red_loc.substr(red_loc.find(",")+1, red_loc.length()-red_loc.find(",")-1)};
    std::string red_y{red_loc.substr(0,red_loc.find(","))};

    // std::cout << std::endl ;
    // std::cout << "****************" << blue_x << " "<< blue_y << "****************";
    // std::cout << std::endl ;

    // std::cout << std::endl ;
    // std::cout << "****************" << red_x << " "<< red_y << "****************";
    // std::cout << std::endl ;

    draw_element(std::stoi(red_x), std::stoi(red_y), std::stoi(blue_x), std::stoi(blue_y));
    

    // for (auto x:values)
    //     std::cout << x << "     ";
    // std::cout << std::endl;
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
void MainWindow::set_field()
{
    // std::vector<std::vector<int>> map_tmp;
    // std::vector<int> row_tmp;

    map.clear();
    row_tmp.clear();
    for (size_t j{}; j<cell_num; j++)
    {
        row_tmp.push_back(0);
    }
    map.push_back(std::move(row_tmp));
    row_tmp.clear();

    // for (size_t j{}; j<cell_num; j++)
    // {
    //     std::cout<< map[0][j] << "    ";
    // }
    // std::cout<< std::endl;

    for (size_t i{}; i<cell_num-2; i++)
    {
        row_tmp.push_back(0);
        for (size_t j{}; j<cell_num-2; j++)
        {
            row_tmp.push_back(1);
        }
        row_tmp.push_back(0);
        map.push_back(std::move(row_tmp));
        row_tmp.clear();
    }
    for (size_t j{}; j<cell_num; j++)
    {
        row_tmp.push_back(0);
    }
    map.push_back(std::move(row_tmp));
    row_tmp.clear();


  

    QGraphicsRectItem *rectangle;
    QGraphicsLineItem *line;
    QBrush brush(Qt::black);
    QPen pen(Qt::black);
    qint16 part{static_cast<short int> (length_scene/cell_num)};
    // rectangle = scene->addRect(0, 0, 2 , 2, pen, brush);
    rectangle = scene->addRect(0, 0, length_scene, part, pen, brush);
    rectangle = scene->addRect(0, 0, part , length_scene, pen, brush);
    rectangle = scene->addRect(length_scene-part, 0, part , length_scene, pen, brush);
    rectangle = scene->addRect( 0, length_scene-part, length_scene , part, pen, brush);
    
    // line = scene->addLine(x1, y1, x2, y2, pen2);
    for (size_t i{}; i<=cell_num; i++)
    {
        line = scene->addLine(0, part*i, length_scene, part*i, pen);
        line = scene->addLine(part*i, 0, part*i, length_scene, pen);
    }

    draw_element((cell_num-1)/2 - 2, (cell_num-1)/2, (cell_num-1)/2 + 2, (cell_num-1)/2 );



    map[(cell_num-1)/2][(cell_num-1)/2 - 2]=0;
    map[(cell_num-1)/2][(cell_num-1)/2 + 2]=0;

    head_x = (cell_num-1)/2 + 2;
    head_y = (cell_num-1)/2;
    // for (size_t i{}; i<cell_num; i++)
    // {
    //     for (size_t j{}; j<cell_num; j++)
    //     {
    //         std::cout<< map[i][j] << "    ";
    //     }
    //     std::cout<< std::endl;
    // }
}
void MainWindow::draw_element(qint16 x_red, qint16 y_red, qint16 x_blue, qint16 y_blue)
{
    qint16 part{static_cast<short int> (length_scene/cell_num)};
    QGraphicsRectItem *rectangle;
    QPen pen_red(Qt::red);
    QPen pen_blue(Qt::blue);
    QBrush brush_red(Qt::red);
    QBrush brush_blue(Qt::blue);
    rectangle = scene->addRect((x_blue)*part, (y_blue)*part, part , part, pen_blue, brush_blue);
    rectangle = scene->addRect((x_red)*part, (y_red)*part, part , part, pen_red, brush_red);

    head_x = x_blue;
    head_y = y_blue;
    
    map[y_blue][x_blue] = 0;

    
}

