#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

//    ushort a = 65534;
//    uchar *b[1][10];
//    b[0][0] = (uchar*)&a;
//    int *c = (int*)b[0][0];
//    qDebug()<<*c;


//    for(int i = 0; i<100*100; i++)
//    {
//        std::cout<<**(ptr+10*10)<<std::endl;
//    }

//    std::cout << ptr[100][100] << std::endl;


//    m_udpSender = new UdpSender();

//    m_udpSender->sendSettings("127.0.0.1", "135", 0, 0, 200);
//    m_udpSender->cyclicSend();

//    m_imgProcessor = new ImgProcessor();

//    unsigned short **ptr = m_imgProcessor->readPixel("D:\\Temp\\img\\IhSample1.png");
//    std::cout<<**(ptr+1024)<<std::endl;

//    unsigned short adder = **(ptr+1024);
//    unsigned char *adderuc = (unsigned char*) *(ptr+1024);
//    for(int i = 0; i < 64; i++)
//    {

//    }
//    std::cout<<"adder: "<<adder<<std::endl;

//    std::cout<<"adderuc: "<<(int)*adderuc<<std::endl;
//    for(int i = 0; i<1024*1618; i++)
//    {
//        for(int j = i; j<1024; j++)
//        {
////           unsigned char *adderuc = (unsigned char*) *(ptr+1024);

//        }
//    }

//    for(int i = 0; i<1024*1618; i++)
//    {

//    }

    m_imgProcessor = new ImgProcessor();
    m_udpSender = new UdpSender();

    ui->sendBtn->setEnabled(false);
    ui->stopBtn->setEnabled(false);

    connect(ui->imgPathBtnH, &QPushButton::clicked, this, &Widget::imgPathBtnHIsClicked);   //高能图片按钮
    connect(ui->imgPathBtnL, &QPushButton::clicked, this, &Widget::imgPathBtnLIsClicked);   //低能图片按钮

    connect(ui->sendBtn, &QPushButton::clicked, this, [=]()
    {
        ui->sendBtn->setEnabled(false);
        ui->stopBtn->setEnabled(true);
        ui->imgPathBtnH->setEnabled(false);
        ui->imgPathBtnL->setEnabled(false);
        qDebug()<<__LINE__;
        m_udpSender->sendSettings(ui->IPlineEdit->text(), ui->portLineEdit->text(), ui->intervalSpinBox->value());

        /*直接整张图在udpsender中发送会报错*/
//        unsigned short **pixelsH = m_imgProcessor->readPixels(m_imgPathH.toStdString());    //高能图像素值
//        unsigned short **pixelsL = m_imgProcessor->readPixels(m_imgPathL.toStdString());    //低能图像素值
//        m_udpSender->cyclicSend( pixelsH,  pixelsL);

        /*单个像素点读取发送会报错*/
//        for(int i = 0; i < 1618; i++)
//        {
//            for(int j = 0; j < 1024; j++)
//            {
//                unsigned short pixelH = m_imgProcessor->readPixel(i, j, m_imgPathH.toStdString());
//                unsigned short pixelL = m_imgProcessor->readPixel(i, j, m_imgPathH.toStdString());
//                m_udpSender->cyclicSend(pixelH,pixelL);
////                std::cout<<m_imgProcessor->readPixel(0, 1, m_imgPathH.toStdString())<<std::endl;
//            }
//        }

        /*测试单个像素发送，发送正确*/
        unsigned short pixelH = m_imgProcessor->readPixel(0, 1, m_imgPathH.toStdString());
        unsigned short pixelL = m_imgProcessor->readPixel(0, 1, m_imgPathH.toStdString());
        m_udpSender->cyclicSend(pixelH, pixelL);

//        for(int i = 0; i < 1024; i++)
//        {
//            for(int j = 0; )
//        }



    });

    connect(ui->stopBtn, &QPushButton::clicked, this, &Widget::stopBtnIsClicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::imgPathBtnHIsClicked()
{
    ui->imgPathLabelH->setText(QFileDialog::getOpenFileName(this, "打开图片", "D:\\Temp\\img","(*.png)"));
    if(ui->imgPathLabelH->text() != "")
    {
        ui->imgLabelH->setPixmap(QPixmap(m_imgPathH).scaled(ui->imgLabelH->width(), ui->imgLabelH->height(), Qt::KeepAspectRatio));

        flag[0] = true;
        if(flag[0] == true && flag[1] == true)
        {
            ui->sendBtn->setEnabled(true);
        }
    }
    else
    {
        ui->imgLabelH->setPixmap(QPixmap());
        flag[0] = false;
        ui->sendBtn->setEnabled(false);
    }
}

void Widget::imgPathBtnLIsClicked()
{
    ui->imgPathLabelL->setText(QFileDialog::getOpenFileName(this, "打开图片", "D:\\Temp\\img","(*.png)"));
    if(ui->imgPathLabelL->text() != "")
    {
        ui->imgLabelL->setPixmap(QPixmap(m_imgPathL).scaled(ui->imgLabelL->width(), ui->imgLabelL->height(), Qt::KeepAspectRatio));

        flag[1] = true;
        if(flag[0] == true && flag[1] == true)
        {
            ui->sendBtn->setEnabled(true);
        }
    }
    else
    {
        ui->imgLabelL->setPixmap(QPixmap());
        flag[1] = false;
        ui->sendBtn->setEnabled(false);
    }
}

void Widget::stopBtnIsClicked()
{
    ui->stopBtn->setEnabled(false);
    ui->sendBtn->setEnabled(true);
    ui->imgPathBtnH->setEnabled(true);
    ui->imgPathBtnL->setEnabled(true);
    m_udpSender->stopSend();
}

