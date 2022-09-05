#include "udpsender.h"

UdpSender::UdpSender(QWidget *parent) : QWidget(parent)
{
    m_socket = new QUdpSocket(this);
    m_socket->bind(QHostAddress::AnyIPv4, 10001);

    m_imgProcessor = new ImgProcessor("D:\\Temp\\img\\IhSample1.png", "D:\\Temp\\img\\IlSample1.png");

    /*包头*/
    {
        m_packageHead.append(0xff);
        m_packageHead.append(0xff);
        m_packageHead.append(0x01);
        m_packageHead.append(0x01);
        m_packageHead.append(0xff);
        m_packageHead.append(0xff);
        m_packageHead.append(0x01);
        m_packageHead.append(0x01);
    }

//    m_package[0].append(m_packageHead);

    /*计时器声明*/
    m_sendTimer = new QTimer(this);
    m_sendTimer->setTimerType(Qt::PreciseTimer);    //提高计时器的精度，windows下没什么用，主要用于linux
    m_sendTimer->setInterval(2);

    connect(m_sendTimer, &QTimer::timeout, this, &UdpSender::sendData);



//    m_matH = cv::imread("D:\\Temp\\img\\IhSample1.png", cv::IMREAD_UNCHANGED);   //按原图读入
//    if(m_matH.empty()) return;
//    cv::cvtColor(m_matH, m_matH, cv::COLOR_BGRA2GRAY);    //四通道转灰度图（单通道）
//    cv::transpose(m_matH, m_matH);
////    flip(m_matH, m_matH, -1);

//    m_matL = cv::imread("D:\\Temp\\img\\IlSample1.png", cv::IMREAD_UNCHANGED);   //按原图读入
//    if(m_matL.empty()) return;
//    cv::cvtColor(m_matL, m_matL, cv::COLOR_BGRA2GRAY);    //四通道转灰度图（单通道）
//    cv::transpose(m_matL, m_matL);
////    flip(m_matL, m_matL, -1);

}

UdpSender::~UdpSender()
{

}

void UdpSender::sendSettings(QString ipaddress, QString port, int interval, ushort hValue, ushort lValue)
{
    m_ipaddress = ipaddress;
    m_port = port;
    m_hValue = hValue;
    m_lValue = lValue;
    m_interval = interval;
}

void UdpSender::sendSettings(QString ipaddress, QString port, int interval, std::string imgPathH, std::string imgPathL)
{
    m_ipaddress = ipaddress;
    m_port = port;
    m_interval = interval;

    m_matH = cv::imread(imgPathH);
    if(m_matH.empty()) return;
    cv::cvtColor(m_matH, m_matH, cv::COLOR_BGRA2GRAY);    //四通道转灰度图（单通道）
    cv::transpose(m_matH, m_matH);

    m_matL = cv::imread(imgPathL);
    if(m_matL.empty()) return;
    cv::cvtColor(m_matL, m_matL, cv::COLOR_BGRA2GRAY);    //四通道转灰度图（单通道）
    cv::transpose(m_matL, m_matL);
}

void UdpSender::cyclicSend(unsigned short **arrayH, unsigned short **arrayL)
{
    for(int i = 0; i<1024*1618; i++)
    {
        unsigned char *adderH = (unsigned char*)*(arrayH+i);
        unsigned char *adderL = (unsigned char*)*(arrayL+i);

        m_arrayOneCard.clear();
        m_arrayFourCard.clear();
        for (int i = 0; i < 64; i++)
        {
            m_arrayOneCard.append(adderH[0]).append(adderH[1]); //0代表 numberShortH 的低八位，1代表高八位
        }

        for(int i = 64; i < 64 * 2; i++)
        {
            m_arrayOneCard.append(adderL[0]).append(adderL[1]);
        }

        for (int i = 0; i<4; i++)
        {
            m_arrayFourCard.append(m_arrayOneCard);
        }

        for(int i = 0; i < 8; i++)
        {
            m_package[i].clear();
            if(i == 0)
            {
                m_package[i].append(m_packageHead).append(m_arrayFourCard);
            }
            else
            {
                m_package[i].append(m_arrayFourCard);
            }
        }
        sendData();
//        m_sendTimer->start(m_interval);
    }
}

void UdpSender::cyclicSend(unsigned short pixelH, unsigned short pixelL)
{
    unsigned char *adderH = (unsigned char*)&pixelH;
    unsigned char *adderL = (unsigned char*)&pixelL;

    m_arrayOneCard.clear();
    m_arrayFourCard.clear();
    for(int i = 0; i < 64; i++)
    {
        m_arrayOneCard.append(adderH[0]).append(adderH[1]); //0代表 numberShortH 的低八位，1代表高八位
    }

    for(int i = 0; i < 64; i++)
    {
        m_arrayOneCard.append(adderL[0]).append(adderL[1]);
    }

    for(int i = 0; i < 4; i++)
    {
        m_arrayFourCard.append(m_arrayOneCard);
    }

    for(int i = 0; i < 8; i++)
    {
        m_package[i].clear();
        if(i == 0)
        {
            m_package[i].append(m_packageHead).append(m_arrayFourCard);
        }
        else
        {
            m_package[i].append(m_arrayFourCard);
        }
    }

    for(int i = 0; i < 8; i++)
    {
        m_socket->writeDatagram(m_package[i], QHostAddress(m_ipaddress), m_port.toUInt());
    }
    //    m_sendTimer->start(m_interval);
}

void UdpSender::cyclicSend(unsigned short *pixelH, unsigned short *pixelL)
{
    Q_UNUSED(pixelH);
    Q_UNUSED(pixelL);
}

void UdpSender::cyclicSend()
{
    int num = 0;
    unsigned short *boardsH = nullptr;
    unsigned short *boardsL = nullptr;
    unsigned char *adderH = nullptr;
    unsigned char *adderL = nullptr;

    for(int i = 0; i < 1618; i++)
    {
        /*高低能板子拼接*/
        boardsH = (unsigned short*)m_imgProcessor->readBoardH(i);    //读取一列的高能板
        boardsL = (unsigned short*)m_imgProcessor->readBoardL(i);    //读取一列的低能板
        std::cout<<num++<<std::endl;

        m_arrayOneCardH.clear();
        m_arrayOneCardL.clear();
        m_arrayTwoCardHL->clear();
        for(int i = 0; i < 1024; i++)
        {
            adderH = (unsigned char*) &boardsH+i;
            adderL = (unsigned char*) &boardsL+i;
            m_arrayOneCardH.append(adderH[0]).append(adderH[1]);
            m_arrayOneCardL.append(adderL[0]).append(adderL[1]);
            if(i == 100)
                qDebug()<<m_arrayOneCardH;
            if(i+1 % 64 == 0)
            {
                /*高能拼低能*/
                m_arrayTwoCardHL[i%64].append(m_arrayOneCardH).append(m_arrayOneCardL); //64*16=1024
            }
        }

        /**
        for(int i = 0; i<16; i++)
        {
            for(int j = 0; j < 64*2; j++) //高能板
            {
                m_arrayOneCardH.append(adderH[j]);
            }
            for(int j = 0; j < 64*2; j++) //低能板
            {
                m_arrayOneCardL.append(adderL[j]);
            }
            m_arrayTwoCardHL[i].append(m_arrayOneCardH).append(m_arrayOneCardL);
        }
        */

        /*分包，16个板子分8次发送，每次2个（2个高能，2个低能）*/
        for(int i = 0; i < 8; i++)
        {
            m_package[i].clear();
            if( i == 0 )    //添加包头
            {
                m_package[i].append(m_packageHead).append(m_arrayTwoCardHL[i]).append(m_arrayTwoCardHL[i+1]);
                continue;
            }
            m_package[i].append(m_arrayTwoCardHL[i]).append(m_arrayTwoCardHL[i+1]);
        }

        /*发送*/
        sendData();
    }
}

void UdpSender::cyclicSendEx()
{
    int rows = m_matH.rows;
    int cols = m_matH.cols;
    qDebug() << "imgsH col row:" << rows << cols;
    rows = m_matL.rows;
    cols = m_matL.cols;
    qDebug() << "imgsL col row:" << rows << cols;
    for (int rowIndex = 0; rowIndex < rows ; rowIndex++) {
        uchar* linePtrH = m_matH.ptr(0);
        uchar* linePtrL = m_matL.ptr(0);


        package.clear();
        package.append(m_packageHead);

        for (int cardIndex = 0; cardIndex < 16; cardIndex++) {
            uchar* cardPtrH = linePtrH + cardIndex * 64 * sizeof (ushort);
            uchar* cardPtrL = linePtrL + cardIndex * 64 * sizeof (ushort);

            package.append((char *)cardPtrH, 64 * sizeof (ushort));
            package.append((char *)cardPtrL, 64 * sizeof (ushort));
        }
        m_sendTimer->start(m_interval);
        sendData();
    }
}

void UdpSender::sendData()
{
    int packageLength = package.length();
    int pacageNums = packageLength / 1024;
    int lastPkgBytes = packageLength % 1024;
//        qDebug() << "package:" << packageLength << pacageNums << lastPkgBytes;
//        qDebug() << package;

    for (int pkgIndex = 0 ; pkgIndex < pacageNums ; pkgIndex++) {
        QByteArray splitPackage = package.mid(pkgIndex*1024, 1024);
        m_socket->writeDatagram(splitPackage, QHostAddress(m_ipaddress), m_port.toUInt());
    }
    if(lastPkgBytes > 0){
        m_socket->writeDatagram(package.mid(pacageNums, lastPkgBytes), QHostAddress(m_ipaddress), m_port.toUInt());
    }

//    for(int i = 0; i < 8; i++)
//    {
//        m_socket->writeDatagram(m_package[i], QHostAddress(m_ipaddress), m_port.toUInt());
//    }
}

void UdpSender::stopSend()
{
    if(m_sendTimer->isActive())
    {
        m_sendTimer->stop();
    }
}

void UdpSender::test()
{
    unsigned short *boardsH = (unsigned short*)m_imgProcessor->readBoardH(0);    //读取一列的高能板
    unsigned short *boardsL = (unsigned short*)m_imgProcessor->readBoardL(0);    //读取一列的低能板
    std::cout<<m_imgProcessor->readPixel(0,32,"D:\\Temp\\img\\IhSample1.png")<<std::endl;

//    std::cout<<sizeof (boardsH)/2;

    unsigned char *adderL;
    unsigned char *adderH;
    for(int i = 0; i<1024; i++)
    {
//        adderH = ()
        std::cout<<*boardsH+i<<std::endl;
    }

//    unsigned char* adderH = (unsigned char*)&boardsH[32];
}

