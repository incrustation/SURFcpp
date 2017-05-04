/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <android/log.h>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
/* Header for class com_example_think_surfcpp_OpencvNative */

#ifndef _Included_com_example_think_surfcpp_OpencvNative
#define _Included_com_example_think_surfcpp_OpencvNative
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_think_surfcpp_OpencvNative
 * Method:    convertGray
 * Signature: (JJ)I
 */
int toGray(Mat img, Mat& gray);
bool response_comparator(const DMatch& p1, const DMatch& p2);
double individualTest(Mat MatImage, int c);
int run_all_tests(Mat MatImage, Rect bounding_rect);

JNIEXPORT jint JNICALL Java_com_example_think_surfcpp_OpencvNative_convertGray
  (JNIEnv *, jclass, jlong, jlong);

JNIEXPORT jint JNICALL Java_com_example_think_surfcpp_OpencvNative_findtag
  (JNIEnv *, jclass, jlong, jint);

#ifdef __cplusplus
}
#endif
#endif
