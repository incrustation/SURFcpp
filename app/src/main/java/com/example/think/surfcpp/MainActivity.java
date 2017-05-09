package com.example.think.surfcpp;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCameraView;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

public class MainActivity extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2 {

    private static String TAG = "MainActivity";
    JavaCameraView javaCameraView;

    Mat mRGBA, mGray, tag1, tag2, tag3, tag4, tag5;

    static{
        Log.i("Sys library loading", "This should call the onLoad function");
        System.loadLibrary("MyOpencvLibs");
    }
    BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch(status){
                case BaseLoaderCallback.SUCCESS:
                    javaCameraView.enableView();
                    break;
                default:
                    super.onManagerConnected(status);
                    break;
            }

        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(new String[] {Manifest.permission.CAMERA}, 1);
            }
        }
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        javaCameraView = (JavaCameraView)findViewById(R.id.java_camera_view);
        javaCameraView.setVisibility(View.VISIBLE);
        javaCameraView.setCvCameraViewListener(this);
    }

    @Override
    protected void onPause(){
        super.onPause();
        if(javaCameraView == null){
            javaCameraView.disableView();
        }
    }

    @Override
    protected void onDestroy(){
        super.onDestroy();
        if(javaCameraView == null){
            javaCameraView.disableView();
        }
    }

    @Override
    protected void onResume(){
        super.onResume();
        if(OpenCVLoader.initDebug()){
            Log.i(TAG, "Opencv loaded successfully");
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        } else {
            Log.i(TAG, "Opencv not loaded");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_1_0, this, mLoaderCallback);
        }
    }


    @Override
    public void onCameraViewStarted(int width, int height) {
        mRGBA = new Mat(height, width, CvType.CV_8UC4);
        mGray = new Mat(height, width, CvType.CV_8UC1);
        tag1 = new Mat(height, width, CvType.CV_8UC4);
        tag2 = new Mat(height, width, CvType.CV_8UC4);
        tag3 = new Mat(height, width, CvType.CV_8UC4);
        tag4 = new Mat(height, width, CvType.CV_8UC4);
        tag5 = new Mat(height, width, CvType.CV_8UC4);

        //trying to obtain the tag image from Drawable
        Bitmap tag_dummy1 = BitmapFactory.decodeResource(getResources(), R.mipmap.cat2);//Yellow
        Utils.bitmapToMat(tag_dummy1, tag1);
        Bitmap tag_dummy2 = BitmapFactory.decodeResource(getResources(), R.mipmap.cat3);//green
        Utils.bitmapToMat(tag_dummy2, tag2);
        Bitmap tag_dummy3 = BitmapFactory.decodeResource(getResources(), R.mipmap.cat4); //red
        Utils.bitmapToMat(tag_dummy3, tag3);
        Bitmap tag_dummy4 = BitmapFactory.decodeResource(getResources(), R.mipmap.meme1); //deep blue
        Utils.bitmapToMat(tag_dummy4, tag4);
        Bitmap tag_dummy5 = BitmapFactory.decodeResource(getResources(), R.mipmap.circ_bar5); //light blue
        Utils.bitmapToMat(tag_dummy5, tag5);

        OpencvNative.findtag(tag1.getNativeObjAddr(), 0);
        OpencvNative.findtag(tag2.getNativeObjAddr(), 1);
        OpencvNative.findtag(tag3.getNativeObjAddr(), 2);
        OpencvNative.findtag(tag4.getNativeObjAddr(), 3);
        OpencvNative.findtag(tag5.getNativeObjAddr(), 4);
    }

    @Override
    public void onCameraViewStopped() {
        mRGBA.release();
        mGray.release();

    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        mRGBA = inputFrame.rgba();

        OpencvNative.convertGray(mRGBA.getNativeObjAddr(), mGray.getNativeObjAddr());

        return mGray;
    }
}
