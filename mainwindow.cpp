#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , command_ip{"192.168.100.62"}
    , command_port{1235}
    , cell_num{43}
    , port_broadcast{1234}
    , packet{"Right"}
    , length_scene{static_cast<short int> ((static_cast<short int>(600/cell_num))*cell_num)}
    , team_color{"Blue"}
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    
    set_field();
    
    udp_r.bind(QHostAddress{"0.0.0.0"}, port_broadcast);
    connect(&udp_r, &QUdpSocket::readyRead, this, &MainWindow::handle_incoming);


    connect(ui->lineEdit_port_bc, &QLineEdit::editingFinished, this, &MainWindow::change_bc_port);
    connect(ui->lineEdit_command_ip, &QLineEdit::editingFinished, this, &MainWindow::change_command_ip);
    connect(ui->lineEdit_port_blue, &QLineEdit::editingFinished, this, &MainWindow::change_command_port);
    connect(ui->lineEdit_port_red, &QLineEdit::editingFinished, this, &MainWindow::change_command_port);
    connect(ui->radioButton_blue, &QRadioButton::toggled, this, &MainWindow::change_command_port);
    connect(ui->radioButton_red, &QRadioButton::toggled, this, &MainWindow::change_command_port);
    
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


    if (ui->checkBox1->isChecked()==false)
        packet = algo_solve();
    qDebug() << "packet: " << packet ;
    qDebug() << "************************";

    // for (size_t i{}; i<cell_num; i++)
    // {
    //     for (size_t j{}; j<cell_num; j++)
    //     {
    //         std::cout<< *map[i][j] << "    ";
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
    ui->blue_score_label->setText(score_blue);

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

    
    head_x_red = std::stoi(red_x);
    head_y_red = std::stoi(red_y);
    head_x_blue = std::stoi(blue_x);
    head_y_blue = std::stoi(blue_y);
    draw_element();
    
    
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
    
    if (ui-> radioButton_blue -> isChecked())
    {
        command_port = static_cast<short int>(ui->lineEdit_port_blue->text().toInt());
        team_color = "Blue";
    }
    else
    {
        command_port = static_cast<short int>(ui->lineEdit_port_red->text().toInt());
        team_color = "Red";
    }
    
}

void MainWindow::go_up()
{
    if (ui->checkBox1->isChecked())
    {
        packet = "Up";
        udp_s.writeDatagram(packet.toStdString().c_str(), packet.length(), QHostAddress{command_ip}, command_port);
        qDebug() << " ";
        qDebug() << "sending to ip: " << command_ip << " and port: " << command_port << "and message: " << packet.toStdString().c_str() << "with length: " << packet.length();
    }
}
void MainWindow::go_right()
{
    if (ui->checkBox1->isChecked())
    {
        packet = "Right";
        udp_s.writeDatagram(packet.toStdString().c_str(), packet.length(), QHostAddress{command_ip}, command_port);
        qDebug() << " ";
        qDebug() << "sending to ip: " << command_ip << " and port: " << command_port << "and message: " << packet.toStdString().c_str() << "with length: " << packet.length();
    }
}
void MainWindow::go_left()
{
    if (ui->checkBox1->isChecked())
    {
        packet = "Left";
        udp_s.writeDatagram(packet.toStdString().c_str(), packet.length(), QHostAddress{command_ip}, command_port);
        qDebug() << " ";
        qDebug() << "sending to ip: " << command_ip << " and port: " << command_port << "and message: " << packet.toStdString().c_str() << "with length: " << packet.length();
    }
}
void MainWindow::go_down()
{
    if (ui->checkBox1->isChecked())
    {
        packet = "Down";
        udp_s.writeDatagram(packet.toStdString().c_str(), packet.length(), QHostAddress{command_ip}, command_port);
        qDebug() << " ";
        qDebug() << "sending to ip: " << command_ip << " and port: " << command_port << "and message: " << packet.toStdString().c_str() << "with length: " << packet.length();
    }
}
void MainWindow::set_field()
{
    // std::vector<std::vector<int>> map_tmp;
    // std::vector<int> row_tmp;
    
    int* temp_int{};
    map.clear();
    row_tmp.clear();
    for (size_t j{}; j<cell_num; j++)
    {
        temp_int = new int {0};
        row_tmp.push_back(temp_int);
    }
    map.push_back(std::move(row_tmp));
    row_tmp.clear();

 
    for (size_t i{}; i<cell_num-2; i++)
    {
        temp_int = new int {0};
        row_tmp.push_back(temp_int);
        for (size_t j{}; j<cell_num-2; j++)
        {
            temp_int = new int {1};
            row_tmp.push_back(temp_int);
        }
        temp_int = new int {0};
        row_tmp.push_back(temp_int);
        map.push_back(std::move(row_tmp));
        row_tmp.clear();
    }
    for (size_t j{}; j<cell_num; j++)
    {
        temp_int = new int {0};
        row_tmp.push_back(temp_int);
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

    head_x_red = (cell_num-1)/2 - 2;
    head_y_red = (cell_num-1)/2;
    head_x_blue = (cell_num-1)/2 + 2;
    head_y_blue = (cell_num-1)/2;
    

    draw_element();


    temp_int = new int {0};
    map[head_y_red][head_x_red] = temp_int;
    map[head_y_blue][head_x_blue] = temp_int;


    

    
}
void MainWindow::draw_element()
{
    qint16 part{static_cast<short int> (length_scene/cell_num)};
    QGraphicsRectItem *rectangle;
    QPen pen_red(Qt::red);
    QPen pen_blue(Qt::blue);
    QBrush brush_red(Qt::red);
    QBrush brush_blue(Qt::blue);
    rectangle = scene->addRect((head_x_blue)*part, (head_y_blue)*part, part , part, pen_blue, brush_blue);
    rectangle = scene->addRect((head_x_red)*part, (head_y_red)*part, part , part, pen_red, brush_red);

    
    int* temp_int;
    temp_int = new int {0};
    map[head_y_blue][head_x_blue] = temp_int;
    map[head_y_red][head_x_red] = temp_int;
    

    udp_s.writeDatagram(packet.toStdString().c_str(), packet.length(), QHostAddress{command_ip}, command_port);
}


QString MainWindow::algo_solve()
{
    QString dir2go{"Up"};
    QString dir2go_2nd{"Down"};
    qint16 x_red{head_x_red};
    qint16 y_red{head_y_red};
    qint16 x_blue{head_x_blue};
    qint16 y_blue{head_y_blue};


    int tmp_LoS{};
    int max_LoS{};
    int max_LoS_2nd{};
    int LoS_Blue{};
    int LoS_Red{};
    count(y_red, x_red, LoS_Red);
    reset_count();
    count(y_blue, x_blue, LoS_Blue);
    reset_count();
    if (team_color == "Blue")
    {

        if (packet == "Up")
            y_blue --;
        if (packet == "Down")
            y_blue ++;
        if (packet == "Left")
            x_blue --;
        if (packet == "Right")
            x_blue ++;


        // up
        
        count(y_blue-1, x_blue, max_LoS);
        if (*map[y_blue-1][x_blue] == 0)
            max_LoS = 0;
        std::cout << "Up: " << max_LoS << std::endl;
        reset_count();
        // down
        
        count(y_blue+1, x_blue, max_LoS_2nd);
        if (*map[y_blue+1][x_blue] == 0)
            max_LoS_2nd = 0;
        std::cout << "Down: " << max_LoS_2nd << std::endl;
        reset_count();
        if (max_LoS_2nd > max_LoS)
        {
            dir2go = "Down";
            dir2go_2nd = "Up";
            tmp_LoS = max_LoS;
            max_LoS = max_LoS_2nd;
            max_LoS_2nd = tmp_LoS;
        }
        else if (max_LoS_2nd == max_LoS)
        {
            max_LoS_2nd = 0;
        }


        
        // left
        
        count(y_blue, x_blue-1, tmp_LoS);
        if (*map[y_blue][x_blue-1] == 0)
            {
                tmp_LoS = 0;
            }
        std::cout << "Left: " << tmp_LoS << std::endl;
        reset_count();
        if (tmp_LoS > max_LoS)
        {
            dir2go_2nd = dir2go;
            dir2go = "Left";
            max_LoS_2nd = max_LoS;
            max_LoS = tmp_LoS;
        }
        else if (tmp_LoS > max_LoS_2nd)
        {
            max_LoS_2nd = tmp_LoS;
            dir2go_2nd = "Left";
        }
        tmp_LoS = 0;


        // right
        count(y_blue, x_blue+1, tmp_LoS);
        if (*map[y_blue][x_blue+1] == 1)
            tmp_LoS = 0;
        std::cout << "Right: " << tmp_LoS << std::endl;
        reset_count();
        tmp_LoS = 0;

        if (tmp_LoS > max_LoS)
        {
            dir2go_2nd = dir2go;
            dir2go = "Right";
            max_LoS_2nd = max_LoS;
            max_LoS = tmp_LoS;
        }
        else if (tmp_LoS > max_LoS_2nd)
        {
            max_LoS_2nd = tmp_LoS;
            dir2go_2nd = "Right";
        }
        
        qDebug() << "dir2go: " << dir2go ;
        qDebug() << "dir2go LoS: " << max_LoS ;
        qDebug() << "dir2go_2nd: " << dir2go_2nd ;
        qDebug() << "dir2go_2nd LoS: " << max_LoS_2nd ;
        qDebug() << "";
        


        if (max_LoS != LoS_Red)
            return dir2go;
        else if (max_LoS/2 >= max_LoS_2nd)
            return dir2go;
        else 
            return dir2go_2nd;
    }
    else
    {
        // up
        count(y_red-1, x_red, max_LoS);
        reset_count();
        // down
        count(y_red+1, x_red, max_LoS_2nd);
        reset_count();
        if (max_LoS_2nd > max_LoS)
        {
            dir2go = "Down";
            dir2go_2nd = "Up";
            tmp_LoS = max_LoS;
            max_LoS = max_LoS_2nd;
            max_LoS_2nd = tmp_LoS;
        }
        else if (max_LoS_2nd == max_LoS)
        {
            max_LoS_2nd = 0;
        }
        // left
        count(y_red, x_red-1, tmp_LoS);
        reset_count();
        if (tmp_LoS > max_LoS)
        {
            dir2go_2nd = dir2go;
            dir2go = "Left";
            max_LoS_2nd = max_LoS;
            max_LoS = tmp_LoS;
        }
        else if (tmp_LoS > max_LoS_2nd)
        {
            max_LoS_2nd = tmp_LoS;
            dir2go_2nd = "Left";
        }
        // right
        count(y_red, x_red+1, tmp_LoS);
        reset_count();
        if (tmp_LoS > max_LoS)
        {
            dir2go_2nd = dir2go;
            dir2go = "Right";
            max_LoS_2nd = max_LoS;
            max_LoS = tmp_LoS;
        }
        else if (tmp_LoS > max_LoS_2nd)
        {
            max_LoS_2nd = tmp_LoS;
            dir2go_2nd = "Right";
        }
        
        if (max_LoS != LoS_Blue)
            return dir2go;
        else if (max_LoS/2 > max_LoS_2nd)
            return dir2go;
        else 
            return dir2go_2nd;

    }
    
}

void MainWindow::count(int i, int j, int &ans)
{
    ans++;
    *map[i][j] = 2;
    
    if (*map[i+1][j] == 1)
        count(i+1, j, ans); 
    if (*map[i-1][j] == 1)
        count(i-1, j, ans); 
    if (*map[i][j+1] == 1)
        count(i, j+1, ans); 
    if (*map[i][j-1] == 1)
        count(i, j-1, ans); 
    

}

void MainWindow::reset_count()
{
    for (size_t i{}; i<cell_num; i++)
    {
        for (size_t j{}; j<cell_num; j++)
        {
            if (*map[i][j]==2)
                *map[i][j] = 1;
        }
    }
}