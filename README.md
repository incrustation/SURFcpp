


## How to 
- Go to Android.mk and change the OPENCVROOT and NDK_MODULE_PATH based on your OS's directories
- Go to build.gradle and change the first line of `task ndkBuild(type: Exec, description: 'Compile JNI source via NDK') ` to the appropriate directory
- Install and run APK on Andro
