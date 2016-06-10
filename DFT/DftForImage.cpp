#include "DftForImage.h"

DftForImage::DftForImage(cv::Mat source) : _source(source)
{
    _planes[0] = cv::Mat_<float>(source);
    _planes[1] = cv::Mat::zeros(source.size(), CV_32F);
    cv::merge(_planes, 2, _complexMat);

    ComputeBitReversalHelpingArray(_bitReversalHelpingArrayForRowIndices, _complexMat.rows);
    ComputeBitReversalHelpingArray(_bitReversalHelpingArrayForColumnIndices, _complexMat.cols);
}

void DftForImage::Process()
{
    DoDftForImage();
    ComputeMagnitude();
    SwitchToLogarithmicScale();
    RearrangeQuadrants();
    NormalizeImage();
    ShowResult();
}

void DftForImage::ComputeBitReversalHelpingArray(std::vector<int>& bitReversalHelpingArray, const int& numberOfIndices)
{
    const int exponent = std::log2(numberOfIndices);

    for (int i = 0; i < numberOfIndices; ++i)
    {
        bitReversalHelpingArray.push_back(0);
    }

    bitReversalHelpingArray[0] = 0;
    int base = 1;
    int reversed = numberOfIndices / 2;
    for (int digit = 1; digit <= exponent; ++digit)
    {
        bitReversalHelpingArray[base] = reversed;
        int index = 1;
        while (index < base)
        {
            bitReversalHelpingArray[base+index] = bitReversalHelpingArray[base] + bitReversalHelpingArray[index];
            ++index;
        }
        base *= 2;
        reversed /= 2;
    }
}

void DftForImage::DoDftForImage()
{
    DoDftForEachRow();
    DoDftForEachColumn();

    //to check our algorithm we can use cv::dft(_complexMat, _complexMat); instead of running
    //DoDftForEachRow(); and DoDftForEachColumn();  -  so that we can compare the result images
}

void DftForImage::DoDftForEachRow()
{
    for (int rowIndex = 0; rowIndex < _complexMat.rows; ++rowIndex)
    {
        DftForRow(rowIndex, 0, _complexMat.cols);
    }

    RestoreNaturalColumnIndexOrderFromReversedOrder();
}

void DftForImage::DoDftForEachColumn()
{
    for (int columnIndex = 0; columnIndex < _complexMat.cols; ++columnIndex)
    {
        DftForColumn(columnIndex, 0, _complexMat.rows);
    }

    RestoreNaturalRowIndexOrderFromReversedOrder();
}

void DftForImage::DftForRow(const int& currentRowOfComplexMat, const int& columnIndexOfBeginning, const int& length)
{
    for (int localIndex = 0; localIndex <= length / 2 - 1; ++localIndex)
    {
        int firstIndex = columnIndexOfBeginning + localIndex;
        int secondIndex = firstIndex + length / 2;

        std::complex<float> a(_complexMat.at<cv::Vec2f>(currentRowOfComplexMat, firstIndex)[0], _complexMat.at<cv::Vec2f>(currentRowOfComplexMat, firstIndex)[1]);
        std::complex<float> b(_complexMat.at<cv::Vec2f>(currentRowOfComplexMat, secondIndex)[0], _complexMat.at<cv::Vec2f>(currentRowOfComplexMat, secondIndex)[1]);

        const float PI = std::acos(-1);
        const std::complex<float> I(0, 1);

        std::complex<float> firstNewElement(a + b);
        std::complex<float> secondNewElement((a - b) * std::exp((float)(-2)*PI*I*(float)localIndex/(float)length));

        _complexMat.at<cv::Vec2f>(currentRowOfComplexMat, firstIndex) = {firstNewElement.real(), firstNewElement.imag()};
        _complexMat.at<cv::Vec2f>(currentRowOfComplexMat, secondIndex) = {secondNewElement.real(), secondNewElement.imag()};
    }

    if (length > 2)
    {
        DftForRow(currentRowOfComplexMat, columnIndexOfBeginning, length / 2);
        DftForRow(currentRowOfComplexMat, columnIndexOfBeginning + length / 2, length / 2);
    }
}

void DftForImage::DftForColumn(const int& currentColumnOfComplexMat, const int& rowIndexOfBeginning, const int& length)
{
    for (int localIndex = 0; localIndex <= length / 2 - 1; ++localIndex)
    {
        int firstIndex = rowIndexOfBeginning + localIndex;
        int secondIndex = firstIndex + length / 2;

        std::complex<float> a(_complexMat.at<cv::Vec2f>(firstIndex, currentColumnOfComplexMat)[0], _complexMat.at<cv::Vec2f>(firstIndex, currentColumnOfComplexMat)[1]);
        std::complex<float> b(_complexMat.at<cv::Vec2f>(secondIndex, currentColumnOfComplexMat)[0], _complexMat.at<cv::Vec2f>(secondIndex, currentColumnOfComplexMat)[1]);

        const float PI = std::acos(-1);
        const std::complex<float> I(0, 1);

        std::complex<float> firstNewElement(a + b);
        std::complex<float> secondNewElement((a - b) * std::exp((float)(-2)*PI*I*(float)localIndex/(float)length));

        _complexMat.at<cv::Vec2f>(firstIndex, currentColumnOfComplexMat) = {firstNewElement.real(), firstNewElement.imag()};
        _complexMat.at<cv::Vec2f>(secondIndex, currentColumnOfComplexMat) = {secondNewElement.real(), secondNewElement.imag()};
    }

    if (length > 2)
    {
        DftForColumn(currentColumnOfComplexMat, rowIndexOfBeginning, length / 2);
        DftForColumn(currentColumnOfComplexMat, rowIndexOfBeginning + length / 2, length / 2);
    }
}

void DftForImage::RestoreNaturalColumnIndexOrderFromReversedOrder()
{
    for (int i = 0; i < _complexMat.cols; ++i)
    {
        if (i < _bitReversalHelpingArrayForColumnIndices[i])
        {
            //swap _complexMat's i. column and _complexMat's _bitReversalHelpingArrayForColumnIndices[i]. column:

            cv::Mat tempColumn = _complexMat.col(i).clone();
            _complexMat.col(_bitReversalHelpingArrayForColumnIndices[i]).copyTo(_complexMat.col(i));
            tempColumn.copyTo(_complexMat.col(_bitReversalHelpingArrayForColumnIndices[i]));
        }
    }
}

void DftForImage::RestoreNaturalRowIndexOrderFromReversedOrder()
{
    for (int i = 0; i < _complexMat.rows; ++i)
    {
        if (i < _bitReversalHelpingArrayForRowIndices[i])
        {
            //swap _complexMat's i. row and _complexMat's _bitReversalHelpingArrayForColumnIndices[i]. row:

            cv::Mat tempRow = _complexMat.row(i).clone();
            _complexMat.row(_bitReversalHelpingArrayForRowIndices[i]).copyTo(_complexMat.row(i));
            tempRow.copyTo(_complexMat.row(_bitReversalHelpingArrayForRowIndices[i]));
        }
    }
}

void DftForImage::ComputeMagnitude()
{
    cv::split(_complexMat, _planes); // planes[0] = Re(DFT(_complexMat), planes[1] = Im(DFT(_complexMat))
    cv::magnitude(_planes[0], _planes[1], _magnitudeOfComplexMat);
}

void DftForImage::SwitchToLogarithmicScale()
{
    // switching to logarithmic scale (log(1 + sqrt(Re(DFT(_complexMat))^2 + Im(DFT(_complexMat))^2))):
    _magnitudeOfComplexMat += cv::Scalar::all(1);
    cv::log(_magnitudeOfComplexMat, _magnitudeOfComplexMat);
}

void DftForImage::RearrangeQuadrants()
{
    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = _magnitudeOfComplexMat.cols/2;
    int cy = _magnitudeOfComplexMat.rows/2;

    cv::Mat q0(_magnitudeOfComplexMat, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    cv::Mat q1(_magnitudeOfComplexMat, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(_magnitudeOfComplexMat, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(_magnitudeOfComplexMat, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void DftForImage::NormalizeImage()
{
    cv::normalize(_magnitudeOfComplexMat, _magnitudeOfComplexMat, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
                                                                                    // viewable image form (float between values 0 and 1).
}

void DftForImage::ShowResult()
{
    cv::namedWindow("Input image");
    cv::moveWindow("Input image", 10, 10);
    cv::imshow("Input image", _source);

    cv::namedWindow("Spectrum magnitude");
    cv::moveWindow("Spectrum magnitude", 600, 10);
    cv::imshow("Spectrum magnitude", _magnitudeOfComplexMat);

    // to get a viewable saved image after imwrite we need pixel values between 0 and 255:
    for (int i = 0; i < _magnitudeOfComplexMat.rows; ++i)
    {
        for (int j = 0; j < _magnitudeOfComplexMat.cols; ++j)
        {
            _magnitudeOfComplexMat.at<float>(i, j) = 255 * _magnitudeOfComplexMat.at<float>(i, j);
        }
    }

    cv::imwrite("../source.png", _source);
    cv::imwrite("../result.png", _magnitudeOfComplexMat);

    cv::waitKey();
    cv::destroyAllWindows();
}
