package com.example.think.surfcpp;

/**
 * Created by Think on 5/2/17.
 */

public class OpencvNative {

    public native static int convertGray(long matAddrRgba, long matAddrGray);
    public native static int findtag(long matAddrtag, int index);
}
