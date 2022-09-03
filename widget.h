#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QPixmap>

#include "opencv2/opencv.hpp"

#include "udpsender.h"
#include "imgprocessor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    /**
     * @brief imgPathBtnHIsClicked  点击高能图片路径按钮
     */
    void imgPathBtnHIsClicked();

    /**
     * @brief imgPathBtnLIsClicked  点击低能图片路径按钮
     */
    void imgPathBtnLIsClicked();

    /**
     * @brief stopBtnIsClicked  点击停止按钮
     */
    void stopBtnIsClicked();

private:
    Ui::Widget *ui;
//    cv::Mat m_mat;

    UdpSender *m_udpSender;
    ImgProcessor *m_imgProcessor;
    QString m_imgPathH = "D:\\Temp\\img\\IhSample1.png";
    QString m_imgPathL = "D:\\Temp\\img\\IlSample1.png";

    bool flag[2] {false};

};
#endif // WIDGET_H
