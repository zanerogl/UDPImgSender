#include "udpsender.h"

UdpSender::UdpSender(QWidget *parent) : QWidget(parent)
{
    m_socket = new QUdpSocket(this);
    m_socket->bind(QHostAddress::AnyIPv4, 10001);

//    std::cout<< *m_array[1618] << std::endl;

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

    m_package[0].append(m_packageHead);

    /*计时器声明*/
    m_sendTimer = new QTimer(this);
    m_sendTimer->setTimerType(Qt::PreciseTimer);    //提高计时器的精度，windows下没什么用，主要用于linux
    m_sendTimer->setInterval(2);

    connect(m_sendTimer, &QTimer::timeout, this, &UdpSender::sendData);

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

    for(int i = 0; i<4; i++)
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

void UdpSender::sendData()
{
    for(int i = 0; i < 8; i++)
    {
        m_socket->writeDatagram(m_package[i], QHostAddress(m_ipaddress), m_port.toUInt());
    }
}

void UdpSender::stopSend()
{
    if(m_sendTimer->isActive())
    {
        m_sendTimer->stop();
    }
}

