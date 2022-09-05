#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_udpSender = new UdpSender();

    ui->sendBtn->setEnabled(false);
    ui->stopBtn->setEnabled(false);

    connect(ui->imgPathBtnH, &QPushButton::clicked, this, &Widget::imgPathBtnHIsClicked);   //高能图片按钮
    connect(ui->imgPathBtnL, &QPushButton::clicked, this, &Widget::imgPathBtnLIsClicked);   //低能图片按钮

    connect(ui->sendBtn, &QPushButton::clicked, this, &Widget::sendBtnIsClicked);
    connect(ui->stopBtn, &QPushButton::clicked, this, &Widget::stopBtnIsClicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::imgPathBtnHIsClicked()
{
    QString str = QFileDialog::getOpenFileName(this, "打开图片", "D:\\Temp\\img","(*.png)");
    ui->imgPathLabelH->setText(str);

    m_imgPathH = str;
    pathConvert_Single2Double(m_imgPathH);

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
    QString str = QFileDialog::getOpenFileName(this, "打开图片", "D:\\Temp\\img","(*.png)");
    ui->imgPathLabelL->setText(str);

    m_imgPathL = str;
    pathConvert_Single2Double(m_imgPathL);

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

void Widget::sendBtnIsClicked()
{
    ui->sendBtn->setEnabled(false);
    ui->stopBtn->setEnabled(true);
    m_udpSender->sendSettings(ui->IPlineEdit->text(), ui->portLineEdit->text(),
                              ui->intervalSpinBox->value(),
                              m_imgPathH.toStdString(), m_imgPathL.toStdString());
    m_udpSender->cyclicSendEx();
}

void Widget::stopBtnIsClicked()
{
    ui->stopBtn->setEnabled(false);
    ui->sendBtn->setEnabled(true);
    ui->imgPathBtnH->setEnabled(true);
    ui->imgPathBtnL->setEnabled(true);
    m_udpSender->stopSend();
}

void Widget::pathConvert_Single2Double(QString &str)
{
//    qDebug()<<str.size()<<str;
    int length = str.size();
    for(int i = 0; i<length; i++)
    {
        if(str.at(i) =="/")
        {
            str.insert(i, "/");
            i++;
        }
    }
//    qDebug()<<str;
}

