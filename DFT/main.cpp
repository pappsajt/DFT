#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>
#include <complex>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "DftForImage.h"

const std::string inputPath = "../input/";

void ReadImage(cv::Mat& img);

int main()
{
    cv::Mat source;
    ReadImage(source);

    DftForImage dft(source);
    dft.Process();

    return 0;
}

void ReadImage(cv::Mat& img)
{
    std::cout << std::endl << "Name of input image: ";

    std::string nameOfImg;
    std::getline(std::cin, nameOfImg);

    img = cv::imread(inputPath+nameOfImg, cv::IMREAD_GRAYSCALE);

    bool numberOfRowsIsPowerOfTwo = true;
    bool numberOfColumnsIsPowerOfTwo = true;

    if (img.data)
    {
        numberOfRowsIsPowerOfTwo = ((img.rows & (img.rows - 1)) == 0);
        numberOfColumnsIsPowerOfTwo = ((img.cols & (img.cols - 1)) == 0);
    }

    while (!img.data || !(numberOfRowsIsPowerOfTwo && numberOfColumnsIsPowerOfTwo))
    {
        if (!img.data)
        {
            std::cout << std::endl << "  Error - no such file, or opening file was not succesful!" << std::endl;
            std::cout << "    Please enter the name of input image again: ";
        }
        else if (!(numberOfRowsIsPowerOfTwo && numberOfColumnsIsPowerOfTwo))
        {
            std::cout << std::endl << "  Error - not suitable image size!\n    Number of rows and number of columns have to be power of two!" << std::endl;
            std::cout << "    Please enter the name of a suitable input image: ";
        }

        std::getline(std::cin, nameOfImg);

        img = cv::imread(inputPath+nameOfImg, cv::IMREAD_GRAYSCALE);

        if (img.data)
        {
            numberOfRowsIsPowerOfTwo = ((img.rows & (img.rows - 1)) == 0);
            numberOfColumnsIsPowerOfTwo = ((img.cols & (img.cols - 1)) == 0);
        }
    }
}
