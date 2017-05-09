#include <com_example_think_surfcpp_OpencvNative.h>

vector<KeyPoint> kpts_tag, kpts_tag1, kpts_tag2, kpts_tag3, kpts_tag4, kpts_tag5, kpts_scene;
Mat desc_tag;
Mat desc_scene, img_tag, img_matches;
vector<Mat> desc_tags(5);
vector <double> match_size(5);
int minHessian = 400;
Ptr<SURF> surf = SURF::create(minHessian);
FlannBasedMatcher matcher;
vector< DMatch > matches1, matches2, matches3, matches4, matches5;
vector< DMatch > good_matches1, good_matches2, good_matches3, good_matches4, good_matches5;
vector<Point2f> tag_corners(4), scene_corners1(4), scene_corners2(4), scene_corners3(4), scene_corners4(4), scene_corners5(4);

bool response_comparator(const DMatch& p1, const DMatch& p2) {
        return p1.distance < p2.distance;
}

int toGray(Mat img, Mat& gray){
    //convert to gray scale
    cvtColor(img, gray, CV_RGBA2GRAY);
    //get keypoints and matches
    surf->detectAndCompute(gray, noArray(), kpts_scene, desc_scene);
    Mat black_screen = Mat::zeros(gray.rows,gray.cols,gray.type());
    drawKeypoints(black_screen, kpts_scene, black_screen, 255, 0);
    if(desc_scene.empty()){
            return 0;
        }
    if(desc_scene.type()!=CV_32F) {
            desc_scene.convertTo(desc_tag, CV_32F);
    }

    matcher.match(desc_tags[0], desc_scene, matches1);
    matcher.match(desc_tags[1], desc_scene, matches2);
    matcher.match(desc_tags[2], desc_scene, matches3);
    matcher.match(desc_tags[3], desc_scene, matches4);
    matcher.match(desc_tags[4], desc_scene, matches5);
    if (matches1.size() == 0 ||matches2.size() == 0||matches3.size() == 0||matches4.size() == 0
        ||matches5.size() == 0){
        gray = black_screen;
        return 0;
    }
    //find good matches

    double max_dist = 0;
    double min_dist = 100;

    //sort matches by distance
    std::sort(matches1.begin(), matches1.end(), response_comparator);
    std::sort(matches2.begin(), matches2.end(), response_comparator);
    std::sort(matches3.begin(), matches3.end(), response_comparator);
    std::sort(matches4.begin(), matches4.end(), response_comparator);
    std::sort(matches5.begin(), matches5.end(), response_comparator);


    int limit1 = (matches1.size()>60)? 60:matches1.size();
    int limit2 = (matches2.size()>60)? 60:matches2.size();
    int limit3 = (matches3.size()>60)? 60:matches3.size();
    int limit4 = (matches4.size()>60)? 60:matches4.size();
    int limit5 = (matches5.size()>60)? 60:matches5.size();
    good_matches1 = matches1;
    good_matches2 = matches2;
    good_matches3 = matches3;
    good_matches4 = matches4;
    good_matches5 = matches5;
    match_size[0] = good_matches1[limit1-1].distance;
    match_size[1] = good_matches2[limit2-1].distance;
    match_size[2] = good_matches3[limit3-1].distance;
    match_size[3] = good_matches4[limit4-1].distance;
    match_size[4] = good_matches5[limit5-1].distance;
    std::sort(match_size.begin(), match_size.end());
    vector< Point2f > tag1, tag2, tag3, tag4, tag5, scene1, scene2, scene3, scene4, scene5;
    for( int i = 0; i < good_matches1.size(); i++ ){
        //-- Get the keypoints from the good matches
        tag1.push_back( kpts_tag1[ good_matches1[i].queryIdx ].pt );
        scene1.push_back( kpts_scene[ good_matches1[i].trainIdx ].pt );
    }
    for( int i = 0; i < good_matches2.size(); i++ ){
        //-- Get the keypoints from the good matches
        tag2.push_back( kpts_tag2[ good_matches2[i].queryIdx ].pt );
        scene2.push_back( kpts_scene[ good_matches2[i].trainIdx ].pt );
    }
    for( int i = 0; i < good_matches3.size(); i++ ){
        //-- Get the keypoints from the good matches
        tag3.push_back( kpts_tag3[ good_matches3[i].queryIdx ].pt );
        scene3.push_back( kpts_scene[ good_matches3[i].trainIdx ].pt );
    }
    for( int i = 0; i < good_matches4.size(); i++ ){
        //-- Get the keypoints from the good matches
        tag4.push_back( kpts_tag4[ good_matches4[i].queryIdx ].pt );
        scene4.push_back( kpts_scene[ good_matches4[i].trainIdx ].pt );
    }
    for( int i = 0; i < good_matches5.size(); i++ ){
        //-- Get the keypoints from the good matches
        tag5.push_back( kpts_tag5[ good_matches5[i].queryIdx ].pt );
        scene5.push_back( kpts_scene[ good_matches5[i].trainIdx ].pt );
    }
    if (tag1.size() == 0||tag2.size()==0||tag3.size()==0||tag4.size()==0||tag5.size()==0) {
        gray = black_screen;
        return 0;
    }

    Mat H1 = findHomography( tag1, scene1, CV_RANSAC );
    Mat H2 = findHomography( tag2, scene2, CV_RANSAC );
    Mat H3 = findHomography( tag3, scene3, CV_RANSAC );
    Mat H4 = findHomography( tag4, scene4, CV_RANSAC );
    Mat H5 = findHomography( tag5, scene5, CV_RANSAC );


    if (!H1.empty()){
        perspectiveTransform( tag_corners, scene_corners1, H1);
        line( black_screen, scene_corners1[0], scene_corners1[1], Scalar(255, 255, 0), 4 );
        line( black_screen, scene_corners1[1], scene_corners1[2], Scalar(255, 255, 0), 4 );
        line( black_screen, scene_corners1[2], scene_corners1[3], Scalar(255, 255, 0), 4 );
        line( black_screen, scene_corners1[3], scene_corners1[0], Scalar(255, 255, 0), 4 );
    }

    if (!H2.empty()){
        perspectiveTransform( tag_corners, scene_corners2, H2);
        line( black_screen, scene_corners2[0], scene_corners2[1], Scalar(0, 255, 0), 4 );
        line( black_screen, scene_corners2[1], scene_corners2[2], Scalar(0, 255, 0), 4 );
        line( black_screen, scene_corners2[2], scene_corners2[3], Scalar(0, 255, 0), 4 );
        line( black_screen, scene_corners2[3], scene_corners2[0], Scalar(0, 255, 0), 4 );
    }

    if (!H3.empty()){
        perspectiveTransform( tag_corners, scene_corners3, H3);
        line( black_screen, scene_corners3[0], scene_corners3[1], Scalar(255, 0, 0), 4 );
        line( black_screen, scene_corners3[1], scene_corners3[2], Scalar(255, 0, 0), 4 );
        line( black_screen, scene_corners3[2], scene_corners3[3], Scalar(255, 0, 0), 4 );
        line( black_screen, scene_corners3[3], scene_corners3[0], Scalar(255, 0, 0), 4 );
    }

    if (!H4.empty()){
        perspectiveTransform( tag_corners, scene_corners4, H4);
        line( black_screen, scene_corners4[0], scene_corners4[1], Scalar(0, 0, 255), 4 );
        line( black_screen, scene_corners4[1], scene_corners4[2], Scalar(0, 0, 255), 4 );
        line( black_screen, scene_corners4[2], scene_corners4[3], Scalar(0, 0, 255), 4 );
        line( black_screen, scene_corners4[3], scene_corners4[0], Scalar(0, 0, 255), 4 );
    }

    if (!H5.empty()){
        perspectiveTransform( tag_corners, scene_corners5, H5);
        line( black_screen, scene_corners5[0], scene_corners5[1], Scalar(0, 255, 255), 4 );
        line( black_screen, scene_corners5[1], scene_corners5[2], Scalar(0, 255, 255), 4 );
        line( black_screen, scene_corners5[2], scene_corners5[3], Scalar(0, 255, 255), 4 );
        line( black_screen, scene_corners5[3], scene_corners5[0], Scalar(0, 255, 255), 4 );
    }
    gray = black_screen;
    Mat trans_mat = Mat(2,3,CV_32F);
       trans_mat.at<float>(0,0) = 1;
       trans_mat.at<float>(0,1) = 0;
       trans_mat.at<float>(0,2) = 50;
       trans_mat.at<float>(1,0) = 0;
       trans_mat.at<float>(1,1) = 1;
       trans_mat.at<float>(1,2) = 0;
       warpAffine(black_screen,black_screen,trans_mat,black_screen.size());
    if( !img.data || !gray.data ){
        std::cout<< " --(!) Error reading images " << std::endl;
        return -1;
    }

    return 0;
}

int gettag(Mat img, int index){
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
    desc_tags[index] = desc_tag;
    switch(index){
        case 0:
            kpts_tag1 = kpts_tag;
            break;
        case 1:
            kpts_tag2 = kpts_tag;
            break;
        case 2:
            kpts_tag3 = kpts_tag;
            break;
        case 3:
            kpts_tag4 = kpts_tag;
            break;
        case 4:
            kpts_tag5 = kpts_tag;
            break;
        default:
        std::cout<< " --(!) Error filing kpts_tag " << std::endl;
        break;
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
    (JNIEnv *, jclass, jlong addrtag, jint index){
       Mat& mtag = *(Mat*)addrtag;
       int idx = (int)index;
       int conv;
       jint retVal;

       conv = gettag(mtag, idx);
       retVal = (jint)conv;

       return retVal;
    }

