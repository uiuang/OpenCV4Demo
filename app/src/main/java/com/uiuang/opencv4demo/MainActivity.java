package com.uiuang.opencv4demo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.uiuang.opencv4demo.ui.BitmapOperationActivity;
import com.uiuang.opencv4demo.ui.OpenImageActivity;

public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button buttonBitmap = findViewById(R.id.buttonBitmap);
        Button buttonImage = findViewById(R.id.buttonImage);

        buttonBitmap.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, BitmapOperationActivity.class);
                startActivity(intent);
            }
        });
        buttonImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, OpenImageActivity.class);
                startActivity(intent);
            }
        });
    }

}