#ifndef IMGPROCESSOR_H
#define IMGPROCESSOR_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <QDebug>

class ImgProcessor
{
public:
    ImgProcessor();
    ImgProcessor(std::string imgPathH, std::string imgPathL);
    ~ImgProcessor();

    /**
     * @brief toString  int转string，用于读取图片通道数和位深
     * @param type
     * @return
     */
    std::string toString(int type);

    /**
     * @brief readPixel 读取单个点的像素值
     * @param x         像素点所在x坐标
     * @param y         像素点所在y坐标
     * @param imgPath   图片路径
     * @return
     */
    ushort readPixel(int x, int y, std::string imgPath);
    ushort *readPixel_(int x, int y, std::string imgPath);
    /**
     * @brief readPixels 读取整张图片像素值
     * @param imgPath   图片路径
     */
    ushort** readPixels(std::string imgPath);

    /**
     * @brief readBoard 返回一个板子（64个像素）
     * @param col       列数
     * @param imgPath   图片路径
     * @return
     */
    unsigned short *readBoard(int col, std::string imgPath);
    /**
     * @brief readBoardH    //返回高能板
     * @param col           //当前所在列
     * @return
     */
    unsigned short *readBoardH(int col);

    /**
     * @brief readBoardH
     * @param col
     * @return
     */
    unsigned short *readBoardL(int col);

private:
    cv::Mat m_mat;


//    ushort *m_pixels[1024][1618] = {{nullptr}};
    ushort *m_pixel = nullptr;
    ushort *m_pixels[1618][1024] = {{nullptr}};

    ushort *m_boardsH[16][64];    //一列的高能板
    ushort *m_boardsL[16][64];    //一列的低能板
//    ushort m_pixels_[65535][65535] = {{0}};

//    ushort **_m_pixels_;

//    uchar **m_pixelsT;
};

#endif // IMGPROCESSOR_H
