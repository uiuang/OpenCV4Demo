//
// Created by Zsc on 2020/11/15.
//

#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <android/log.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <random>
#include <cmath>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>

using namespace cv;
using namespace std;

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "debug", __VA_ARGS__))

#define JNI_API(name) Java_com_uiuang_opencv4demo_jni_OpenCVImageUtil_##name

#define ASSERT(status, ret) if(!(status)){return ret;}
#define ASSERT_FALSE(status) ASSERT(status,false)

CascadeClassifier cascadeClassifier;

extern "C" {
bool BitmapToMat(JNIEnv *env, jobject obj_bitmap, cv::Mat &matrix);
bool MatToBitmap(JNIEnv *env, cv::Mat &matrix, jobject obj_bitmap);
JNIEXPORT jintArray JNICALL
JNI_API(ImageBlur)(JNIEnv *env, jclass type, jintArray buf, jint w, jint h) {
    //读取int数组并转化为Mat类型
    jint *cbuf = env->GetIntArrayElements(buf, JNI_FALSE);
    if (NULL == cbuf) {
        return 0;
    }
    Mat imgData(h, w, CV_8UC4, (unsigned char *) cbuf);
    cv::cvtColor(imgData, imgData, COLOR_BGRA2RGB);
//这里进行图像相关操作
    blur(imgData, imgData, Size(20, 20));
//对图像相关操作完毕
    cv::cvtColor(imgData, imgData, COLOR_BGR2RGBA);
    uchar *ptr = imgData.data;
    int size = w * h;
    jintArray result = env->NewIntArray(size);
    env->SetIntArrayRegion(result, 0, size, reinterpret_cast<const jint *>(ptr));
    env->ReleaseIntArrayElements(buf, cbuf, 0);
    return result;
}
JNIEXPORT jobject  JNICALL JNI_API(counterWorld)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;

    BitmapToMat(env, bitmap, src);
    int middleRows = src.rows >> 1;
    int quarterRows = middleRows >> 1;
    Mat mat(src.size(), src.type());

    for (int row = 0; row < middleRows; ++row) {

        for (int col = 0; col < src.cols; ++col) {

            mat.at<float>(middleRows + row, col) = src.at<float>(
                    quarterRows + row, col);
        }

    }


    for (int row = 0; row < middleRows; ++row) {
        for (int col = 0; col < src.cols; ++col) {
            mat.at<float>(middleRows - row, col) = src.at<float>(quarterRows + row, col);
        }
    }


    MatToBitmap(env, mat, bitmap);
    return bitmap;
}
//马赛克
JNIEXPORT jobject JNICALL JNI_API(mosaic)(JNIEnv *env, jclass type, jobject bitmap) {

    //表示每一个Vec4b对象中,可以存储4个字符型数据,可以用这样的类对象去存储—4通道RGB+Alpha的图

    Mat src;
    BitmapToMat(env, bitmap, src);
    LOGD("nBitmapToMat: BitmapToMat finish");
    int size = 10; //分成8 * 8 的小块 填充相同的颜色
    Mat mosaic = src.clone();
    for (int row = 0; row < src.rows - size; row += size) {
        for (int col = 0; col < src.cols - size; col += size) {
            Vec4b src_pix = src.at<Vec4b>(row, col);
            for (int row_i = 0; row_i < size; ++row_i) {
                for (int col_i = 0; col_i < size; ++col_i) {
                    mosaic.at<Vec4b>(row + row_i, col + col_i) = src_pix;
                }
            }
        }
    }
    LOGD("nBitmapToMat:mosaic");
    MatToBitmap(env, mosaic, bitmap);
    return bitmap;
}
//图片模糊
JNIEXPORT jobject JNICALL JNI_API(blur)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
//    cv::cvtColor(src, src, COLOR_BGRA2GRAY);
    blur(src, src, Size(51, 51));
    MatToBitmap(env, src, bitmap);
    return bitmap;
}

//灰度化
JNIEXPORT jobject JNICALL JNI_API(gray)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    //这里进行图像相关操作
    cv::cvtColor(src, src, COLOR_BGRA2GRAY);
    cv::threshold(src, src, 100, 255, CV_THRESH_BINARY);
    MatToBitmap(env, src, bitmap);
    return bitmap;
}
//浮雕
JNIEXPORT jobject JNICALL JNI_API(relief)(JNIEnv *env, jclass type, jobject bitmap) {

    Mat src;
    BitmapToMat(env, bitmap, src);
    int index = 3;
    //这里进行图像相关操作
    Mat img0(src.size(), CV_8UC3);
    for (int y = 1; y < src.rows - 1; y++) {
        uchar *p0 = src.ptr<uchar>(y);
        uchar *p1 = src.ptr<uchar>(y + 1);

        uchar *q0 = img0.ptr<uchar>(y);
        for (int x = 1; x < src.cols - 1; x++) {
            for (int i = 0; i < index; i++) {
                int tmp0 = p1[index * (x + 1) + i] - p0[index * (x - 1) + i] + 128;//浮雕
                if (tmp0 < 0) {
                    q0[index * x + i] = 0;
                } else if (tmp0 > 255) {
                    q0[index * x + i] = 255;
                } else {
                    q0[index * x + i] = tmp0;
                }
            }
        }
    }
    MatToBitmap(env, img0, bitmap);
    return bitmap;
}
/**
 * 雕刻
 *
 * @param env
 * @param type
 * @param bitmap
 * @return
 */
JNIEXPORT jobject JNICALL JNI_API(sculpture)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    int index = 3;
    //这里进行图像相关操作
    Mat img1(src.size(), CV_8UC3);
    for (int y = 1; y < src.rows - 1; y++) {
        uchar *p0 = src.ptr<uchar>(y);
        uchar *p1 = src.ptr<uchar>(y + 1);

        uchar *q1 = img1.ptr<uchar>(y);
        for (int x = 1; x < src.cols - 1; x++) {
            for (int i = 0; i < index; i++) {
                int tmp1 = p0[index * (x - 1) + i] - p1[index * (x + 1) + i] + 128;//雕刻
                if (tmp1 < 0) {
                    q1[index * x + i] = 0;
                } else if (tmp1 > 255) {
                    q1[index * x + i] = 255;
                } else {
                    q1[index * x + i] = tmp1;
                }
            }
        }
    }
    MatToBitmap(env, img1, bitmap);
    return bitmap;

}

//油画
JNIEXPORT jobject JNICALL JNI_API(oilPaiting)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    //治理进行图像相关操作
    Mat gray;
    cvtColor(src, gray, COLOR_BGRA2GRAY);
    Mat res;
    res = src.clone();
    const int g_size = 5;
    const int t_size = 8;
    for (int row = 0; row < src.rows - t_size; ++row) {
        for (int col = 0; col < src.cols - t_size; ++col) {
            //统计灰度等级
            int grade[g_size + 1] = {0};
            int b[g_size + 1] = {0};
            int g[g_size + 1] = {0};
            int r[g_size + 1] = {0};
            for (int t_row = 0; t_row < t_size; ++t_row) {
                for (int t_col = 0; t_col < t_size; ++t_col) {
                    uchar gray_value = gray.at<uchar>(row + t_row, col + t_col);
                    int grade_index = gray_value / (255 / g_size);
                    grade[grade_index] += 1;
                    b[grade_index] += src.at<Vec4b>(row + t_row, col + t_col)[0];
                    g[grade_index] += src.at<Vec4b>(row + t_row, col + t_col)[1];
                    r[grade_index] += src.at<Vec4b>(row + t_row, col + t_col)[2];
                }
            }
//找出最多落入像素最多的等级
            int max_index = 0;
            int max = grade[0];
            for (int index = 0; index <= g_size; ++index) {
                if (grade[index] > max) {
                    max_index = index;
                    max = grade[index];
                }
            }
            res.at<Vec4b>(row, col)[0] = b[max_index] / max;
            res.at<Vec4b>(row, col)[1] = g[max_index] / max;
            res.at<Vec4b>(row, col)[2] = r[max_index] / max;

        }
    }

    MatToBitmap(env, res, bitmap);
    return bitmap;

}

//轮廓图
JNIEXPORT jobject JNICALL JNI_API(canary)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    //这里进行图像相关操作
    cv::cvtColor(src, src, COLOR_BGRA2GRAY);
    cv::GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
    //调用Canny 方法实现边缘检测
    cv::Canny(src, src, 50, 150, 3, false);
    MatToBitmap(env, src, bitmap);
    return bitmap;

}
//毛玻璃
JNIEXPORT jobject JNICALL JNI_API(frostedGlass)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);

    int size = 8; //分成5 * 5的小块 在小块中随机取值填充

    Mat frostedGlass = src.clone();

    RNG rng((unsigned) time(NULL));

    for (int row = 0; row < src.rows - size; ++row) {
        for (int col = 0; col < src.cols - size; ++col) {
            int roandnumber = rng.uniform(0, size);
            frostedGlass.at<Vec4b>(row, col)[0] = src.at<Vec4b>(row + roandnumber,
                                                                col + roandnumber)[0];
            frostedGlass.at<Vec4b>(row, col)[1] = src.at<Vec4b>(row + roandnumber,
                                                                col + roandnumber)[1];
            frostedGlass.at<Vec4b>(row, col)[2] = src.at<Vec4b>(row + roandnumber,
                                                                col + roandnumber)[2];
        }
    }

    MatToBitmap(env, frostedGlass, bitmap);
    return bitmap;
}
//直方图均衡化
JNIEXPORT jobject JNICALL JNI_API(equalizeHist)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    Mat gray, final;
    //这里进行图片的处理
    cv::cvtColor(src, gray, COLOR_BGR2GRAY);
    //直方图均衡化
    cv::equalizeHist(gray, final);

    MatToBitmap(env, final, bitmap);
    return bitmap;
}
//图像增强，拉普拉斯算子
JNIEXPORT jobject JNICALL JNI_API(lapulasi)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    //这里进行图片的处理
    Mat gray, laplaci, final;
    //高斯模糊
    cv::GaussianBlur(src, src, Size(5, 5), 0, 0);
    //转灰度图像
    cv::cvtColor(src, gray, CV_BGR2GRAY);
    //拉普拉斯处理
    cv::Laplacian(gray, laplaci, CV_16S, 3, 1, 0);
    //绝对值处理
    cv::convertScaleAbs(laplaci, laplaci);
    //后期处理
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    cv::morphologyEx(laplaci, final, CV_MOP_TOPHAT, kernel);
    cv::threshold(final, final, 100, 255, THRESH_TOZERO_INV);
    cv::threshold(final, final, 20, 90, THRESH_BINARY);

    MatToBitmap(env, final, bitmap);
    return bitmap;
}
////图像翻转
JNIEXPORT jobject JNICALL JNI_API(flip)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    //这里进行图片的处理
    //0倒过来，1翻转，-1倒过来加翻转
    cv::flip(src, src, -1);
    MatToBitmap(env, src, bitmap);
    return bitmap;
}
//图像叠加 线性处理 不对
JNIEXPORT jobject JNICALL JNI_API(add)(JNIEnv *env, jclass type, jobject bitmap, jobject bitmap2) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    Mat src2;
    BitmapToMat(env, bitmap2, src2);
    //这里进行图片的处理
    //【2】定义一个Mat类型并给其设定ROI区域
    //第一个参数，InputArray类型的src1，表示需要加权的第一个数组，常常填一个Mat。
//第二个参数，alpha，表示第一个数组的权重
//第三个参数，src2，表示第二个数组，它需要和第一个数组拥有相同的尺寸和通道数。
//第四个参数，beta，表示第二个数组的权重值。
//第五个参数，dst，输出的数组，它和输入的两个数组拥有相同的尺寸和通道数。
//第六个参数，gamma，一个加到权重总和上的标量值。看下面的式子自然会理解。
//第七个参数，dtype，输出阵列的可选深度，有默认值-1。;当两个输入数组具有相同的深度时，个参数设置为-1（默认值），即等同于src1.depth（）。dst = src1

    cv::addWeighted(src2, 0.5, src, 0.3, 0.0, src2);
    MatToBitmap(env, src2, bitmap2);
    return bitmap2;
}
//图像膨胀
JNIEXPORT jobject JNICALL JNI_API(dilate)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    //这里进行图片的处理
    Mat *kernel = new Mat();
    dilate(src, src, *kernel);
    MatToBitmap(env, src, bitmap);
    return bitmap;
}
//图像侵蚀
JNIEXPORT jobject JNICALL JNI_API(erode)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    //这里进行图片的处理
    Mat *kernel = new Mat();
    erode(src, src, *kernel);
    MatToBitmap(env, src, bitmap);
    return bitmap;
}
//图像扭曲
JNIEXPORT jobject JNICALL JNI_API(warping)(JNIEnv *env, jclass type, jobject bitmap) {
    Mat src;
    BitmapToMat(env, bitmap, src);
    //这里进行图片的处理
    Point2f srcTri[3];
    Point2f dstTri[3];

    Mat warp_mat(2, 3, CV_32FC1);
    //设置三个点来计算仿射变换
    srcTri[0] = Point2f(0, 0);
    srcTri[1] = Point2f(src.cols - 1, 0);
    srcTri[2] = Point2f(0, src.rows - 1);

    dstTri[0] = Point2f(src.cols * 0.0, src.rows * 0.33);
    dstTri[1] = Point2f(src.cols * 0.85, src.rows * 0.25);
    dstTri[2] = Point2f(src.cols * 0.15, src.rows * 0.7);

    //计算仿射变换矩阵
    warp_mat = getAffineTransform(srcTri, dstTri);
    //创建仿射变换目标图像与原图像尺寸类型相同
    Mat warp_dstImage = Mat::zeros(src.rows, src.cols, src.type());
    cv::warpAffine(src, src, warp_mat, warp_dstImage.size());

    MatToBitmap(env, src, bitmap);
    return bitmap;
}


bool BitmapToMat(JNIEnv *env, jobject obj_bitmap, cv::Mat &matrix) {
    void *bitmapPixels;//保存图片像素
    AndroidBitmapInfo bitmapInfo;//保存图片参数
    ASSERT_FALSE(AndroidBitmap_getInfo(env, obj_bitmap, &bitmapInfo) >= 0);//获取图片参数
    ASSERT_FALSE(bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                 bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565);//只支持 ARGB_8888和RGB_565
    ASSERT_FALSE(AndroidBitmap_lockPixels(env, obj_bitmap, &bitmapPixels) >= 0);//获取图片参数（锁定内存块）
    ASSERT_FALSE(bitmapPixels);
    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        cv::Mat tmp(bitmapInfo.height, bitmapInfo.width, CV_8UC4, bitmapPixels);//建立临时mat
        tmp.copyTo(matrix);//拷贝目标matrix
    } else {
        cv::Mat tmp(bitmapInfo.height, bitmapInfo.width, CV_8UC2, bitmapPixels);
        cv::cvtColor(tmp, matrix, cv::COLOR_BGRA2BGR565);
    }
    AndroidBitmap_unlockPixels(env, obj_bitmap);//解锁
    return true;

}

bool MatToBitmap(JNIEnv *env, cv::Mat &matrix, jobject obj_bitmap) {
    void *bitmapPixels;
    AndroidBitmapInfo bitmapInfo;
    ASSERT_FALSE(AndroidBitmap_getInfo(env, obj_bitmap, &bitmapInfo) >= 0);
    ASSERT_FALSE(bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                 bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565);
    ASSERT_FALSE(matrix.dims == 2 && bitmapInfo.height == (uint32_t) matrix.rows &&
                 bitmapInfo.width == (uint32_t) matrix.cols);// 必须是 2 维矩阵，长宽一致
    ASSERT_FALSE(matrix.type() == CV_8UC1 || matrix.type() == CV_8UC3 || matrix.type() == CV_8UC4);
    ASSERT_FALSE(AndroidBitmap_lockPixels(env, obj_bitmap, &bitmapPixels) >= 0);
    ASSERT_FALSE(bitmapPixels);
    //CV_8UC1，CV_8UC2，CV_8UC3。 其中 U代表 Unsigned 无符号、C代表CvMat 后面的数字代表通道数
    //（最后的1、2、3表示通道数，譬如RGB3通道就用CV_8UC3）
    //1--bit_depth---比特数---代表8bite,16bites,32bites,64bites---举个例子吧--比如说,如
    //        如果你现在创建了一个存储--灰度图片的Mat对象,这个图像的大小为宽100,高100,那么,现在这张
    //        灰度图片中有10000个像素点，它每一个像素点在内存空间所占的空间大小是8bite,8位--所以它对
    //        应的就是CV_8
    //     2--S|U|F--S--代表---signed int---有符号整形
    //               U--代表--unsigned int--无符号整形
    //               F--代表--float---------单精度浮点型
    //     3--C<number_of_channels>----代表---一张图片的通道数,比如:
    //         1--灰度图片--grayImg---是--单通道图像
    //         2--RGB彩色图像---------是--3通道图像
    //         3--带Alph通道的RGB图像--是--4通道图像
    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        cv::Mat tmp(bitmapInfo.height, bitmapInfo.width, CV_8UC4, bitmapPixels);
        switch (matrix.type()) {
            case CV_8UC1:
//                cv::cvtColor(matrix, tmp, CV_GRAY2BGRA);
                cv::cvtColor(matrix, tmp, cv::COLOR_GRAY2BGRA);
                break;
            case CV_8UC2:
                cv::cvtColor(matrix, tmp, cv::COLOR_BGR5652BGRA);
                break;
            case CV_8UC3:
                cv::cvtColor(matrix, tmp, cv::COLOR_RGB2BGRA);
                break;
            case CV_8UC4:
                matrix.copyTo(tmp);
                break;
            default:
                AndroidBitmap_unlockPixels(env, obj_bitmap);
                return false;
        }
    } else {
        cv::Mat tmp(bitmapInfo.height, bitmapInfo.width, CV_8UC2, bitmapPixels);
        switch (matrix.type()) {
            case CV_8UC1:
                cv::cvtColor(matrix, tmp, cv::COLOR_GRAY2BGR565);
                break;
            case CV_8UC2:
                matrix.copyTo(tmp);
                break;
            case CV_8UC3:
                cv::cvtColor(matrix, tmp, cv::COLOR_RGB2BGR565);
                break;
            case CV_8UC4:
                //对应bitmap   RGBA_8888
//                cv::cvtColor(mat, tmp, CV_BGRA2BGR565);
                cv::cvtColor(matrix, tmp, cv::COLOR_BGRA2BGR565);
                break;
            default:
                AndroidBitmap_unlockPixels(env, obj_bitmap);
                return false;
        }
    }
    AndroidBitmap_unlockPixels(env, obj_bitmap);
    return true;
}
}



