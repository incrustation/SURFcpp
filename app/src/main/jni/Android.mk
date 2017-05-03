LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#opencv
OPENCVROOT:= /Users/Think/Dropbox/Junior/capstone
OPENCV_CMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED

ifdef OPENCV_ANDROID_SDK
  ifneq ("","$(wildcard $(OPENCV_ANDROID_SDK)/OpenCV.mk)")
    include ${OPENCV_ANDROID_SDK}/OpenCV.mk
  else
    include ${OPENCV_ANDROID_SDK}/sdk/native/jni/OpenCV.mk
  endif
else
  include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk
endif

NDK_MODULE_PATH=/Users/Think/Library/Android/sdk/ndk-bundle
LOCAL_SRC_FILES:=com_example_think_surfcpp_OpencvNative.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl
LOCAL_MODULE:= MyOpencvLibs

include $(BUILD_SHARED_LIBRARY)