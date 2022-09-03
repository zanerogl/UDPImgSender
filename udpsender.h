#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QWidget>
#include <QUdpSocket>
#include <QTimer>
#include <QString>
#include <iostream>
//#include "imgprocessor.h"
#include <typeinfo>

class UdpSender : public QWidget
{
    Q_OBJECT
public:
    explicit UdpSender(QWidget *parent = nullptr);
    ~UdpSender();

    /**
     * @brief sendSettings      发送设置
     * @param ipaddress         ip
     * @param port              端口
     * @param interval          发送间隔
     * @param hValue            高能值
     * @param lValue            低能值
     */
    void sendSettings(QString ipaddress, QString port, int interval, ushort hValue = 0, ushort lValue = 0);

    /**
     * @brief cyclicSend
     * @param array
     */
    /**
     * @brief cyclicSend        循环发送数据
     * @param arrayH            高能图像素值
     * @param arrayL            低能图像素值
     */
    void cyclicSend(unsigned short **arrayH, unsigned short **arrayL);

    void cyclicSend(unsigned short pixelH, unsigned short pixelL);

    /**
     * @brief sendData  发送数据
     */
    void sendData();

    /**
     * @brief stopSend  停止发送
     */
    void stopSend();

    ushort **m_array;

signals:

private:
    QUdpSocket *m_socket = nullptr;
//    ImgProcessor *m_imgProcessor;

    QString m_ipaddress;
    QString m_port;
    unsigned short m_hValue;
    unsigned short m_lValue;
    int m_interval;

    QByteArray m_packageHead;   //包头
    QByteArray m_package[8];

    QByteArray m_arrayOneCard;
    QByteArray m_arrayFourCard;

    QTimer *m_sendTimer  = nullptr; //循环发送时用的计时器


    uchar *m_data;

};

#endif // UDPSENDER_H
