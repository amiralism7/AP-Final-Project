// #include <QNetworkDatagram>
// #include <QObject>
// #include <QUdpSocket>
// #include <iostream>
// #include <string>

// class udp_R: public QObject
// {
//     Q_OBJECT
// public:
//     udp_R():QObject{nullptr}
//     {
//         udp.bind(QHostAddress{"0.0.0.0"}, 1234);
//         connect(&udp, &QUdpSocket::readyRead, this, &udp_R::handle_incoming);
        
//     }
//     void handle_incoming()
//     {
//         QNetworkDatagram recv = udp.receiveDatagram();
//         // qDebug() << recv.data();


//         std::string s{recv.data()};
//         parse(s);
//         // std::cout << s << std::endl;
        
//     }
//     void parse (std::string s)
//     {
//         std::string token;
//         size_t pos;
//         std::string line{s};
//         line.erase(s.length()-1, s.length());
//         line.erase(0, 1);
//         while ((pos = line.find(",")) != std::string::npos) 
//         {
//             token = line.substr(0, pos);
//             std::cout << token << "---";
//             line.erase(0, pos + 1);
//         }
//         std::cout << line;
//         std::cout << std::endl;
//     }
// private:
//     QUdpSocket udp;
// };

