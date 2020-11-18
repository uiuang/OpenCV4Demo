package com.uiuang.opencv4demo.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;

import com.uiuang.opencv4demo.jni.BitmapOperation;
import com.uiuang.opencv4demo.R;

public class BitmapOperationActivity extends AppCompatActivity implements View.OnClickListener {
    private Button mBt_reverse,mBt_gray1,mBt_gray2,mBt_gray3,mBt_removeG,mBt_removeB,mBt_removeR;
    private ImageView mImageView;
    private SeekBar mSeek_brightness,mSeek_contrast;
    private Bitmap mBitmap;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bitmap_operation);

        mBt_reverse = findViewById(R.id.bt_reverse);
        mBt_gray1 = findViewById(R.id.bt_gray1);
        mBt_gray2 = findViewById(R.id.bt_gray2);
        mBt_gray3 = findViewById(R.id.bt_gray3);
        mBt_removeG = findViewById(R.id.bt_removeG);
        mBt_removeB = findViewById(R.id.bt_removeB);
        mBt_removeR = findViewById(R.id.bt_removeR);
        mImageView = findViewById(R.id.img);
        mSeek_brightness = findViewById(R.id.seek_brightness);
        mSeek_contrast = findViewById(R.id.seek_contrast);
        mBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.shuchang);
        mBt_reverse.setOnClickListener(this);
        mBt_gray1.setOnClickListener(this);
        mBt_gray2.setOnClickListener(this);
        mBt_gray3.setOnClickListener(this);
        mBt_removeG.setOnClickListener(this);
        mBt_removeB.setOnClickListener(this);
        mBt_removeR.setOnClickListener(this);
        //亮度
        mSeek_brightness.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                mBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.shuchang);
                int brightness = progress-50;
                BitmapOperation.bitmapBrightness(mBitmap,brightness);
                mImageView.setImageBitmap(mBitmap);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
        //对比度
        mSeek_contrast.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                mBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.shuchang);
                float contrast = progress/100f;
                BitmapOperation. bitmapContrast(mBitmap,contrast);
                mImageView.setImageBitmap(mBitmap);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
    }

    @Override
    public void onClick(View v) {
        mBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.shuchang);
        switch (v.getId()){
            case R.id.bt_gray1:
                BitmapOperation.bitmapGray1(mBitmap);
                break;
            case R.id.bt_gray2:
                BitmapOperation.  bitmapGray2(mBitmap);
                break;
            case R.id.bt_gray3:
                BitmapOperation.   bitmapGray3(mBitmap);
                break;
            case R.id.bt_removeG:
                BitmapOperation.  bitmapRemoveG(mBitmap);
                break;
            case R.id.bt_removeB:
                BitmapOperation.  bitmapRemoveB(mBitmap);
                break;
            case R.id.bt_removeR:
                BitmapOperation. bitmapRemoveR(mBitmap);
                break;
            case R.id.bt_reverse:
                BitmapOperation. bitmapReverse(mBitmap);
                break;
        }
        mImageView.setImageBitmap(mBitmap);
    }
}