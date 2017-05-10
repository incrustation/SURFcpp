package com.example.think.surfcpp;

/**
 * Created by Think on 5/2/17.
 */

public class OpencvNative {

    public native static int convertGray(long matAddrRgba, long matAddrGray);
    public native static int findTag(long matAddrtag, int index);
    public native static int initializeSVM();
}
