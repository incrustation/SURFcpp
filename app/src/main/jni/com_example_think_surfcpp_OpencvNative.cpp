#include <com_example_think_surfcpp_OpencvNative.h>

vector<KeyPoint> kpts_tag, kpts_scene;
Mat desc_tag, desc_scene, img_tag, img_matches;
int minHessian = 400;
Ptr<SURF> surf = SURF::create(minHessian);
FlannBasedMatcher matcher;
vector< DMatch > matches, good_matches;
vector<Point2f> tag_corners(4), scene_corners(4);


int toGray(Mat img, Mat& gray){
    //convert to gray scale
    cvtColor(img, gray, CV_RGBA2GRAY);
    //get keypoints and matches
    surf->detectAndCompute(gray, noArray(), kpts_scene, desc_scene);
    if(desc_scene.type()!=CV_32F) {
            desc_scene.convertTo(desc_tag, CV_32F);
        }
    drawKeypoints(gray, kpts_scene, gray, 255, 0);
    matcher.match(desc_tag, desc_scene, matches);
    if (matches.size() == 0){
        gray = img;
        return 0;
    }
    //find good matches

    double max_dist = 0;
    double min_dist = 100;
    /*
    for( int i = 0; i < desc_tag.rows; i++ ){
        double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }
    for( int i = 0; i < desc_tag.rows; i++ ){
        //double good_dist = max_dist - min_dist/3;
        if( matches[i].distance < 1.1 *min_dist ){
            good_matches.push_back( matches[i]);
        }
    }
    */

    //sort matches by distance
    int limit = (matches.size()>100)? 100:matches.size();
    good_matches = matches;
    vector< Point2f > tag, scene;
    vector< KeyPoint > kpts_good;
    for( int i = 0; i < good_matches.size(); i++ ){
        //-- Get the keypoints from the good matches
        tag.push_back( kpts_tag[ good_matches[i].queryIdx ].pt );
        scene.push_back( kpts_scene[ good_matches[i].trainIdx ].pt );
        kpts_good.push_back(kpts_scene[ good_matches[i].trainIdx ]);
    }
    drawKeypoints(gray, kpts_good, gray, Scalar(255, 0, 255), 0);
    if (tag.size() == 0 || scene.size()==0) {
        gray = img;
        return 0;
    }

    Mat H = findHomography( tag, scene, CV_RANSAC );
    if(H.empty()){
        gray = img;
        return 0;
    }
    perspectiveTransform( tag_corners, scene_corners, H);

    line( gray, scene_corners[0], scene_corners[1], Scalar(255, 255, 0), 4 );
    line( gray, scene_corners[1], scene_corners[2], Scalar( 255, 255, 0), 4 );
    line( gray, scene_corners[2], scene_corners[3], Scalar( 255, 255, 0), 4 );
    line( gray, scene_corners[3], scene_corners[0], Scalar( 255, 255, 0), 4 );

    if( !img.data || !gray.data ){
        std::cout<< " --(!) Error reading images " << std::endl;
        return -1;
    }

    return 0;
}

int gettag(Mat img){
    cvtColor(img, img, CV_RGBA2GRAY);
    img_tag = img;
    tag_corners[0] = cvPoint(0,0);
    tag_corners[1] = cvPoint( img.cols, 0 );
    tag_corners[2] = cvPoint( img.cols, img.rows );
    tag_corners[3] = cvPoint( 0, img.rows );
    surf->detectAndCompute(img, noArray(), kpts_tag, desc_tag);
    if(desc_tag.type()!=CV_32F) {
        desc_tag.convertTo(desc_tag, CV_32F);
    }
    if(!img.data){
        std::cout<< " --(!) Error reading images " << std::endl;
        return -1;
    }

    return 0;
}

JNIEXPORT jint JNICALL Java_com_example_think_surfcpp_OpencvNative_convertGray
  (JNIEnv *, jclass, jlong addrRgba, jlong addrGray){
     Mat& mRgb = *(Mat*)addrRgba;
     Mat& mGray = *(Mat*)addrGray;

     int conv;
     jint retVal;

     conv = toGray(mRgb, mGray);
     retVal = (jint)conv;

     return retVal;
  }

  JNIEXPORT jint JNICALL Java_com_example_think_surfcpp_OpencvNative_findtag
    (JNIEnv *, jclass, jlong addrtag){
       Mat& mtag = *(Mat*)addrtag;

       int conv;
       jint retVal;

       conv = gettag(mtag);
       retVal = (jint)conv;

       return retVal;
    }

