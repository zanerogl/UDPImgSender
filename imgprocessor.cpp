#include "imgprocessor.h"

ImgProcessor::ImgProcessor()
{

}

ImgProcessor::ImgProcessor(std::string imgPathH, std::string imgPathL)
{
    using namespace cv;


//    m_matL = cv::imread(imgPathL, cv::IMREAD_UNCHANGED);    //按原图读入
//    cv::cvtColor(m_matL, m_matL, cv::COLOR_BGRA2GRAY);      //四通道转灰度图（单通道）
//    flip(m_matL, m_matL, -1);
}

std::string ImgProcessor::toString(int type)
{
    std::string strType;
    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);
    switch (depth)
    {
      case CV_8U:
        strType = "CV_8U"; break;
      case CV_8S:
        strType = "CV_8S"; break;
      case CV_16U:
        strType = "CV_16U"; break;
      case CV_16S:
        strType = "CV_16S"; break;
      case CV_32S:
        strType = "CV_32S"; break;
      case CV_32F:
        strType = "CV_32F"; break;
      case CV_64F:
        strType = "CV_64F"; break;
      default:
        strType = "UNKNOWN_TYPE"; break;
    }
    strType += "C";
    strType += (chans + '0');
    return strType;
}

ushort ImgProcessor::readPixel(int x, int y, std::string imgPath)
{
    m_mat = cv::imread(imgPath ,cv::IMREAD_UNCHANGED);   //按原图读入
    cv::cvtColor(m_mat, m_mat, cv::COLOR_BGRA2GRAY);    //四通道转灰度图（单通道）
    return m_mat.at<ushort>(x, y);
}

ushort *ImgProcessor::readPixel_(int x, int y, std::string imgPath)
{
    m_mat = cv::imread(imgPath ,cv::IMREAD_UNCHANGED);   //按原图读入
    cv::cvtColor(m_mat, m_mat, cv::COLOR_BGRA2GRAY);    //四通道转灰度图（单通道）
    *m_pixel = m_mat.at<ushort>(x,y);
    return m_pixel;
}

ushort** ImgProcessor::readPixels(std::string imgPath)
{
    m_mat = cv::imread(imgPath ,cv::IMREAD_UNCHANGED);   //按原图读入
    std::cout<<toString(m_mat.type())<<std::endl;       //查看位深，通道数（例如：CV_16UC2，表示的是元素类型是一个16位的无符号整数，通道数为2)
    cv::cvtColor(m_mat, m_mat, cv::COLOR_BGRA2GRAY);    //四通道转灰度图（单通道）
    qDebug()<<"cols"<<m_mat.rows;

    for(int i = 0; i<m_mat.cols; i++)
    {
        for(int j = 0; j<m_mat.rows; j++)
        {
            unsigned short *pixel = new unsigned short;
            pixel = &m_mat.at<ushort>(i, j);//单通道16位深，通过查看图像信息知道位深为16；指针类型转换不会丢失数据
            m_pixels[i][j] = pixel;

//            if(i == 0 && j == 1)
//            {
//                std::cout << "ushort" << *pixel << std::endl;
//                std::cout << "[][]" << *m_pixels[i][j] << std::endl;
//            }
        }
    }
//    std::cout << __LINE__ << " " << *m_pixels[100][0] << std::endl;
    return *m_pixels;
}

unsigned short* ImgProcessor::readBoard(int col, std::string imgPath)
{
    m_mat = cv::imread(imgPath ,cv::IMREAD_UNCHANGED);   //按原图读入
    std::cout<<toString(m_mat.type())<<std::endl;       //查看位深，通道数（例如：CV_16UC2，表示的是元素类型是一个16位的无符号整数，通道数为2)
    cv::cvtColor(m_mat, m_mat, cv::COLOR_BGRA2GRAY);    //四通道转灰度图（单通道）
    for(int i = 0; i<16; i++)
    {
        for(int j = 0; j<64; j++)
        {
            m_boardsH[i][j] = &m_mat.at<ushort>(col, j+64*i);   //单通道16位深，通过查看图像信息知道位深为16；指针类型转换不会丢失数据
        }
    }
    return **m_boardsH;
}

unsigned short *ImgProcessor::readBoardH(int col)
{
//    for(int i = 0; i<16; i++)
//    {
//        for(int j = 0; j<64; j++)
//        {
//            m_boardsH[i][j] = &m_matH.at<ushort>(col, j+64*i);
//        }
//    }
    return **m_boardsH;
}

unsigned short *ImgProcessor::readBoardL(int col)
{
//    for(int i = 0; i<16; i++)
//    {
//        for(int j = 0; j<64; j++)
//        {
//            m_boardsL[i][j] = &m_matL.at<ushort>(col, j+64*i);
//        }
//    }
    return **m_boardsL;
}
