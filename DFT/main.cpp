#include <iostream>
#include <vector>
#include <complex>
#include <iostream>
#include <iomanip>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const std::string inputPath = "../input/";

void ReadImage(cv::Mat& img);

int main()
{
    cv::Mat source;
    ReadImage(source);

    return 0;
}

void ReadImage(cv::Mat& img)
{
    std::cout << std::endl << "Name of input image: ";

    std::string nameOfImg;
    std::getline(std::cin, nameOfImg);

    img = cv::imread(inputPath+nameOfImg, cv::IMREAD_GRAYSCALE);

    while (!img.data)
    {
        std::cout << std::endl << "Error! No such file, or opening file was not succesful!" << std::endl;
        std::cout << std::endl << "Please enter the name of input image again: ";

        std::getline(std::cin, nameOfImg);

        img = cv::imread(inputPath+nameOfImg, cv::IMREAD_GRAYSCALE);
    }

    cv::namedWindow("window1");
    cv::moveWindow("window1", 10, 10);
    cv::imshow("window1", img);
    cv::waitKey();
    cv::destroyAllWindows();
}
