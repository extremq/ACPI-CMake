#include "ImageGrid.h"
#include "operatii.h"
#include "operatii_cv.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  ImageGrid *grid = new ImageGrid("Prelucrarea imaginilor");

  QString imageDir = "../res/Images/";
  QString imageFile(imageDir + "lena512.bmp");
  // QString imageFile(imageDir + "Fig5.03_zgomotGaussian.jpg");
  // QString imageFile(imageDir + "Fig5.03_zgomotSarePiper.jpg");
  // QString imageFile(imageDir + "Fig5.03_zgomotUniform.jpg");
  // QString imageFile(imageDir + "Fig5.03.jpg");

  grid->addImage(imageFile, 0, 0);
  int w, h;
  unsigned char *img = Tools::readImageGray8(imageFile, w, h);

  unsigned char *cv_negate = cv_negateImage(img, w, h);
  grid->addImage(cv_negate, w, h, 0, 1, "cv negate");

  unsigned char *cv_binary = cv_binaryImage(img, w, h);
  grid->addImage(cv_binary, w, h, 0, 2, "cv binary");

  int *hist = cv_calcHist(img, w, h);
  grid->addHistogram(hist, 256, 1, 0, "histogram");
  grid->show();

  int *eqhist = cv_calcEqHist(img, w, h);
  grid->addHistogram(eqhist, 256, 1, 1, "eq histogram");
  grid->show();

  return a.exec();
}
