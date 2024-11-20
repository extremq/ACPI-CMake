#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

unsigned char *cv_negateImage(unsigned char *img, int w, int h) {
  unsigned char *result = new unsigned char[w * h];
  cv::Mat inMat(h, w, CV_8UC1, img);
  cv::Mat negateMat(h, w, CV_8UC1, result);
  cv::bitwise_not(inMat, negateMat);
  return result;
}

unsigned char *cv_binaryImage(unsigned char *img, int w, int h) {
  unsigned char *result = new unsigned char[w * h];
  cv::Mat inMat(h, w, CV_8UC1, img);
  cv::Mat binaryMat(h, w, CV_8UC1, result);
  cv::threshold(inMat, binaryMat, 120, 255, 1);
  return result;
}

int *cv_calcHist(unsigned char *img, int w, int h) {
    cv::Mat inMat(h, w, CV_8UC1, img);
    cv::Mat hist;
        float range[] = { 0, 256 };
    const float* histRange[] = { range };
    int histSize = 256;
    cv::calcHist(&inMat, 1, 0, cv::Mat(), hist, 1, &histSize, histRange, true, false);

    int* result = new int[256];
    for (int i = 0; i < 256; ++i) {
        result[i] = hist.at<float>(i);
    }

    return result;
}

int *cv_calcEqHist(unsigned char *img, int w, int h) {
    cv::Mat inMat(h, w, CV_8UC1, img);
    cv::Mat eqInMat;
    cv::equalizeHist(inMat, eqInMat);
    cv::Mat hist;
        float range[] = { 0, 256 };
    const float* histRange[] = { range };
    int histSize = 256;
    cv::calcHist(&eqInMat, 1, 0, cv::Mat(), hist, 1, &histSize, histRange, true, false);

    int* result = new int[256];
    for (int i = 0; i < 256; ++i) {
        result[i] = hist.at<float>(i);
    }

    return result;
}