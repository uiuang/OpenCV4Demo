package com.uiuang.opencv4demo.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RadioGroup;
import android.widget.Toast;

import com.luck.picture.lib.PictureSelector;
import com.luck.picture.lib.config.PictureConfig;
import com.luck.picture.lib.config.PictureMimeType;
import com.luck.picture.lib.entity.LocalMedia;
import com.uiuang.opencv4demo.R;
import com.uiuang.opencv4demo.jni.OpenCVImageUtil;
import com.uiuang.opencv4demo.util.FileUtil;
import com.uiuang.opencv4demo.util.GlideEngine;

import java.util.List;

public class OpenImageActivity extends AppCompatActivity {
    private ImageView imageView;
    private ImageView imageView2;
    private String mPicFilePath;
    private final int PHOTO_REQUEST = 20;
    private RadioGroup radioGroup, radioGroup2;
    private boolean isGroup2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_image);
        imageView = findViewById(R.id.image);
        imageView2 = findViewById(R.id.image2);
        radioGroup = findViewById(R.id.radiogroup);
        radioGroup2 = findViewById(R.id.radiogroup2);
        Button actionButton = findViewById(R.id.btn_action);
        actionButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                PictureSelector.create(OpenImageActivity.this)
                        .openGallery(PictureMimeType.ofImage())
                        .imageEngine(GlideEngine.createGlideEngine()) // Please refer to the Demo GlideEngine.java
                        .selectionMode(PictureConfig.SINGLE)
                        .forResult(PictureConfig.CHOOSE_REQUEST);
            }
        });
//        imageView2.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                Bitmap src = BitmapFactory.decodeResource(getResources(), R.drawable.shuchang);
//                Bitmap resultBit = OpenCVImageUtil.oilPaiting(src);
//                imageView2.setImageBitmap(resultBit);
//            }
//        });
    }


    public void onClickView(View view) {
        Bitmap src = null;
        Bitmap resultBit = null;
        if (!TextUtils.isEmpty(mPicFilePath)) {
            src = FileUtil.picFileToBitmap(mPicFilePath);
        } else {
            src = FileUtil.resourceToBitmap(OpenImageActivity.this, R.drawable.shuchang);
        }
        int id = view.getId();
        switch (id) {
            case R.id.radio1:
                //灰度化
                resultBit = OpenCVImageUtil.gray(src);
                isGroup2 = false;
                break;
            case R.id.radio2:
                //浮雕
                resultBit = OpenCVImageUtil.relief(src);
                isGroup2 = false;
                break;
            case R.id.radio3:
                //油画
                resultBit = OpenCVImageUtil.oilPaiting(src);
                isGroup2 = false;
                break;
            case R.id.radio4:
                //轮廓图
                resultBit = OpenCVImageUtil.canary(src);
                isGroup2 = false;
                break;
            case R.id.radio5:
                //模糊
                resultBit = OpenCVImageUtil.blur(src);
                isGroup2 = false;
                break;
            case R.id.radio6:
                //毛玻璃
                resultBit = OpenCVImageUtil.frostedGlass(src);
                isGroup2 = false;
                break;
            case R.id.radio7:
                //马赛克
                resultBit = OpenCVImageUtil.mosaic(src);
                isGroup2 = false;
                break;
            case R.id.radio8:
                //图像增强
                resultBit = OpenCVImageUtil.equalizeHist(src);
                isGroup2 = true;
                break;
            case R.id.radio9:
                //图像增强
                resultBit = OpenCVImageUtil.lapulasi(src);
                isGroup2 = true;
                break;
            case R.id.radio10:
                //图像翻转
                resultBit = OpenCVImageUtil.flip(src);
                isGroup2 = true;
                break;
            case R.id.radio11:
                //图像叠加
                src = FileUtil.resourceToBitmap(OpenImageActivity.this, R.drawable.shuchang);
                Bitmap src2 = FileUtil.resourceToBitmap(OpenImageActivity.this, R.drawable.shuchang);
                resultBit = OpenCVImageUtil.add(src, src2);
                isGroup2 = true;
                break;
            case R.id.radio12:
                //图像膨胀
                resultBit = OpenCVImageUtil.dilate(src);
                isGroup2 = true;
                break;
            case R.id.radio13:
                //图像侵蚀
                resultBit = OpenCVImageUtil.erode(src);
                isGroup2 = true;
                break;
            case R.id.radio14:
                //图像扭曲
                resultBit = OpenCVImageUtil.warping(src);
                isGroup2 = true;
                break;

        }
        if (isGroup2) {
            radioGroup.clearCheck();
        } else {
            radioGroup2.clearCheck();
        }
        if (resultBit != null) {
            imageView2.setImageBitmap(resultBit);
        } else {
            Toast.makeText(OpenImageActivity.this, "处理出错", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 调用JNI的ImageBlur(int[] pixels,int w,int h)接口实现图像模糊
     */

    public Bitmap doImageBlur(Bitmap origImage) {
        int w = origImage.getWidth();
        int h = origImage.getHeight();
        int[] pixels = new int[w * h];
        origImage.getPixels(pixels, 0, w, 0, 0, w, h);
        int[] image = OpenCVImageUtil.ImageBlur(pixels, w, h);//JNI
        //最后将返回的int数组转为bitmap类型。
        Bitmap desImage = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        //faceall为返回的int数组
        desImage.setPixels(image, 0, w, 0, 0, w, h);
        return desImage;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK) {
            switch (requestCode) {
                case PictureConfig.CHOOSE_REQUEST:
                    // onResult Callback
                    List<LocalMedia> selectList = PictureSelector.obtainMultipleResult(data);
                    if (selectList != null && selectList.size() > 0) {
                       mPicFilePath = selectList.get(0).getRealPath();
//                        String filePath = FileUtil.getFileAbsolutePath(this, uri);
                        Log.d("yeqing", "filePath==" + mPicFilePath);
                        imageView.setImageBitmap(BitmapFactory.decodeFile(mPicFilePath));
                    }
                    break;
                default:
                    break;
            }
        }
    }
}