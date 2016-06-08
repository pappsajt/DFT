#ifndef DFTFORIMAGE_H
#define DFTFORIMAGE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>
#include <complex>
#include <iostream>
#include <iomanip>
#include <cmath>

class DftForImage
{
public:
    DftForImage(cv::Mat source);
    void Process();

private:
    cv::Mat _source;
    cv::Mat _planes[2];
    cv::Mat _complexMat;
    cv::Mat _magnitudeOfComplexMat;

    void DoDftForImage();
    void IterateRows();
    void DftForRow(const int& currentRowOfComplexMat, const int& columnIndexOfBeginning, const int& length);
    void ComputeMagnitude();
    void SwitchToLogarithmicScale();
    void RearrangeQuadrants();
    void NormalizeImage();
    void ShowResult();
};

#endif // DFTFORIMAGE_H
