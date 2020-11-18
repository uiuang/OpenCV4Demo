package com.uiuang.opencv4demo.jni;

import android.graphics.Bitmap;

public class BitmapOperation {
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * 亮度
     *
     * @param bitmap
     */
    public static native void bitmapBrightness(Bitmap bitmap, int brightness);

    /**
     * 对比度
     *
     * @param bitmap
     */
    public static native void bitmapContrast(Object bitmap, float contrast);

    /**
     * 灰度化1
     */
    public static native void bitmapGray1(Object bitmap);

    /**
     * 灰度化2
     */
    public static native void bitmapGray2(Object bitmap);

    /**
     * 灰度化3
     */
    public static native void bitmapGray3(Object bitmap);

    /**
     * 反相
     */
    public static native void bitmapReverse(Object bitmap);

    /**
     * 去蓝
     */
    public static native void bitmapRemoveB(Object bitmap);

    /**
     * 去绿
     */
    public static native void bitmapRemoveG(Object bitmap);

    /**
     * 去红
     */
    public static native void bitmapRemoveR(Object bitmap);
}
