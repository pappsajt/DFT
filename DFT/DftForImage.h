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
    std::vector<int> _bitReversalHelpingArrayForRowIndices;
    std::vector<int> _bitReversalHelpingArrayForColumnIndices;

    void DoDftForImage();
    void DoDftForEachRow();
    void DftForRow(const int& currentRowOfComplexMat, const int& columnIndexOfBeginning, const int& length);
    void RestoreNaturalColumnIndexOrderFromReversedOrder();
    void ComputeMagnitude();
    void SwitchToLogarithmicScale();
    void RearrangeQuadrants();
    void NormalizeImage();
    void ShowResult();
    void ComputeBitReversalHelpingArray(std::vector<int>& _bitReversalHelpingArray, const int& numberOfIndices);
};

#endif // DFTFORIMAGE_H
