#pragma once
#include <algorithm>

unsigned char *negateImage(unsigned char *img, int w, int h) {
  unsigned char *result = new unsigned char[w * h];
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
      result[y * w + x] = 255 - img[y * w + x];
  return result;
}

double clamp(double d, double min, double max) {
  const double t = d < min ? min : d;
  return t > max ? max : t;
}

unsigned char fromDoubleToPixel(double dblpixel) {
  double clamped = clamp(dblpixel, 0, 255);
  return static_cast<unsigned char>(clamped);
}

unsigned char *applyLinFilter(unsigned char *img, int wImg, int hImg,
                              double *filter, int wFilter, int hFilter,
                              double factor) {
  unsigned char *result = new unsigned char[wImg * hImg];
  std::memcpy(result, img, wImg * hImg);
  for (int y = hFilter / 2; y < (hImg - hFilter / 2); y++) {
    for (int x = wFilter / 2; x < (wImg - wFilter / 2); x++) {
      double sum = 0.0;
      for (int ySub = 0; ySub < hFilter; ++ySub) {
        for (int xSub = 0; xSub < wFilter; ++xSub) {
          sum += (double)img[(y + ySub - hFilter / 2) * wImg +
                             (x + xSub - wFilter / 2)] *
                 filter[ySub * wFilter + xSub];
        }
      }
      result[y * wImg + x] = fromDoubleToPixel(sum * factor);
    }
  }

  return result;
}

unsigned char *applyMedianFilter(unsigned char *img, int wImg, int hImg,
                                 int size) {
  unsigned char *result = new unsigned char[wImg * hImg];
  std::memcpy(result, img, wImg * hImg);
  unsigned char values[size * size];

  for (int y = size / 2; y < (hImg - size / 2); y++) {
    for (int x = size / 2; x < (wImg - size / 2); x++) {
      int counter = 0;
      for (int ySub = 0; ySub < size; ++ySub) {
        for (int xSub = 0; xSub < size; ++xSub) {
          values[counter++] =
              img[(y + ySub - size / 2) * wImg + (x + xSub - size / 2)];
        }
      }
      std::sort(values, values + size * size);
      result[y * wImg + x] = values[(size * size - 1) / 2];
    }
  }

  return result;
}

unsigned char getMedianFromHist(int *hist, int size) {
  int counter = 0;
  int target = (size * size - 1) / 2;
  for (int i = 0; i < 256; ++i) {
    counter += hist[i];
    if (counter > target) {
      return i;
    }
  }

  return -1;
}

unsigned char *applyMedianFilterOptimized(unsigned char *img, int wImg,
                                          int hImg, int size) {
  unsigned char *result = new unsigned char[wImg * hImg];
  std::memcpy(result, img, wImg * hImg);
  int hist[256] = {0};

  for (int y = size / 2; y < (hImg - size / 2); y++) {
    for (int i = 0; i < 256; ++i)
      hist[i] = 0;

    for (int x = size / 2; x < (wImg - size / 2); x++) {
      if (x == size / 2) {
        for (int ySub = 0; ySub < size; ++ySub) {
          for (int xSub = 0; xSub < size; ++xSub) {
            hist[img[(y + ySub - size / 2) * wImg + (x + xSub - size / 2)]]++;
          }
        }
      } else {
        for (int ySub = 0; ySub < size; ++ySub) {
          hist[img[(y + ySub - size / 2) * wImg + (x - size / 2 - 1)]]--;
          hist[img[(y + ySub - size / 2) * wImg + (x + size / 2)]]++;
        }
      }
      result[y * wImg + x] = getMedianFromHist(hist, size);
    }
  }

  return result;
}

unsigned char getMeanFromAlphaTrimmed(int *hist, int size, int d) {
  int counter = 0;
  int copy_hist[256] = {};
  for (int i = 0; i < 256; ++i) {
    copy_hist[i] = hist[i];
  }

  for (int i = 0; i < 256; ++i) {
    if (copy_hist[i] > 0) {
      counter += copy_hist[i];
      copy_hist[i] = 0;
      if (counter >= d / 2) {
        copy_hist[i] = counter - d / 2;
        break;
      }
    }
  }

  counter = 0;
  for (int i = 255; i >= 0; --i) {
    if (copy_hist[i] > 0) {
      counter += copy_hist[i];
      copy_hist[i] = 0;
      if (counter >= d / 2) {
        copy_hist[i] = counter - d / 2;
        break;
      }
    }
  }

  counter = 0;
  int mean = 0;
  for (int i = 0; i < 256; ++i) {
    if (copy_hist[i] > 0) {
      mean += i;
      counter += copy_hist[i];
    }
  }

  return mean / counter;
}

unsigned char *applyAlphaTrimmedMeanFilter(unsigned char *img, int wImg,
                                           int hImg, int size) {
  unsigned char *result = new unsigned char[wImg * hImg];
  std::memcpy(result, img, wImg * hImg);
  int hist[256] = {0};

  for (int y = size / 2; y < (hImg - size / 2); y++) {
    for (int i = 0; i < 256; ++i)
      hist[i] = 0;

    for (int x = size / 2; x < (wImg - size / 2); x++) {
      if (x == size / 2) {
        for (int ySub = 0; ySub < size; ++ySub) {
          for (int xSub = 0; xSub < size; ++xSub) {
            hist[img[(y + ySub - size / 2) * wImg + (x + xSub - size / 2)]]++;
          }
        }
      } else {
        for (int ySub = 0; ySub < size; ++ySub) {
          hist[img[(y + ySub - size / 2) * wImg + (x - size / 2 - 1)]]--;
          hist[img[(y + ySub - size / 2) * wImg + (x + size / 2)]]++;
        }
      }
      result[y * wImg + x] = getMeanFromAlphaTrimmed(hist, size, size);
    }
  }

  return result;
}

unsigned char *applyMaxMinFilter(unsigned char *img, int wImg, int hImg,
                                 int size, bool isMax) {
  unsigned char *result = new unsigned char[wImg * hImg];
  std::memcpy(result, img, wImg * hImg);
  unsigned char target = 0;

  for (int y = size / 2; y < (hImg - size / 2); y++) {
    for (int x = size / 2; x < (wImg - size / 2); x++) {
      target = img[y * wImg + x];
      for (int ySub = 0; ySub < size; ++ySub) {
        for (int xSub = 0; xSub < size; ++xSub) {
          if (isMax) {
            target =
                target < img[(y + ySub - size / 2) * wImg +
                             (x + xSub - size / 2)]
                    ? img[(y + ySub - size / 2) * wImg + (x + xSub - size / 2)]
                    : target;
          } else {
            target =
                target > img[(y + ySub - size / 2) * wImg +
                             (x + xSub - size / 2)]
                    ? img[(y + ySub - size / 2) * wImg + (x + xSub - size / 2)]
                    : target;
          }
        }
      }
      result[y * wImg + x] = target;
    }
  }

  return result;
}

unsigned char *applyMidpointFilter(unsigned char *img, int wImg, int hImg,
                                   int size) {
  unsigned char *result = new unsigned char[wImg * hImg];
  std::memcpy(result, img, wImg * hImg);
  unsigned char maxim, minim = 0;

  for (int y = size / 2; y < (hImg - size / 2); y++) {
    for (int x = size / 2; x < (wImg - size / 2); x++) {
      maxim = minim = img[y * wImg + x];
      for (int ySub = 0; ySub < size; ++ySub) {
        for (int xSub = 0; xSub < size; ++xSub) {
          maxim =
              maxim < img[(y + ySub - size / 2) * wImg + (x + xSub - size / 2)]
                  ? img[(y + ySub - size / 2) * wImg + (x + xSub - size / 2)]
                  : maxim;
          minim =
              minim > img[(y + ySub - size / 2) * wImg + (x + xSub - size / 2)]
                  ? img[(y + ySub - size / 2) * wImg + (x + xSub - size / 2)]
                  : minim;
        }
      }
      result[y * wImg + x] =
          (static_cast<int>(maxim) + static_cast<int>(minim)) / 2;
    }
  }

  return result;
}

unsigned char *zoomConv(unsigned char *img, int wImg, int hImg, int scale = 2) {

  unsigned char *result = new unsigned char[scale * scale * wImg * hImg];
  std::memset(result, 0, scale * scale * wImg * hImg);

  int scaledWImg = scale * wImg;
  int scaledHImg = scale * hImg;

  for (int y = 0; y < hImg; ++y) {
    for (int x = 0; x < wImg; ++x) {
      result[(y * scale) * (wImg * scale) + (x * scale)] = img[y * wImg + x];
    }
  }

  // double filter[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
  double filter[25] = {1,4,6,4,1,4,16,24,16,4,6,24,36,24,6,4,16,24,16,4,1,4,6,4,1};

  unsigned char *filtered =
      // applyLinFilter(result, scaledWImg, scaledHImg, filter, 3, 3, 1.0 / 4);
      applyLinFilter(result, scaledWImg, scaledHImg, filter, 5, 5, 1.0 / 64);

  return filtered;
}

unsigned char *pyrUp(unsigned char *img, int wImg, int hImg, int amount) {
  unsigned char *result = img;
  for (int i = 0; i < amount; ++i) {
    result = zoomConv(result, wImg, hImg);
    wImg *= 2;
    hImg *= 2;
  }

  return result;
}