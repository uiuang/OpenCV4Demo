package com.uiuang.opencv4demo;

import android.graphics.Bitmap;

public class ImageProcessing {
    static {
        System.loadLibrary("image_processing-lib");
    }

    public native Bitmap blurryBitmap(Bitmap bitmap);
}
