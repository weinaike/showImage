#ifndef QIMAGEMAT
#define QIMAGEMAT
#include "qimage.h"
#include "opencv.hpp"
#include "qdebug.h"

using namespace cv;

QImage MatToQImage(const Mat &);
Mat QImage2cvMat(QImage image);

#endif // QIMAGEMAT

