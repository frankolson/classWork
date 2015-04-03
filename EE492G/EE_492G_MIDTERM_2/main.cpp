#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//////////////////////////////////////// Get Ellipse /////////////////////////////////////////////
void findEllipse(Mat& img, Mat& final){
	// find contours
	vector<vector<Point> > contours;
	findContours(img, contours, RETR_LIST, CHAIN_APPROX_NONE);
	cout << contours.size() << endl;

	// fit the ellipses
	for (size_t i = 0; i < contours.size(); i++){
		size_t count = contours[i].size();
        if( count < 6 )
            continue;

        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);

        if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*30 )
            continue;
        drawContours(final, contours, (int)i, Scalar::all(255), 1, 8);

        ellipse(final, box, Scalar(0,0,255), 1, LINE_AA);
        ellipse(final, box.center, box.size*0.5f, box.angle, 0, 360, Scalar(0,255,255), 1, LINE_AA);
        Point2f vtx[4];
        box.points(vtx);
	}

	// show results in a sizable window
	namedWindow("Result", WINDOW_NORMAL);
	imshow("Result", final);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////// Remove Noise /////////////////////////////////////////////
void removeSmallBlobs(Mat& im, int thresh)
{
    // Only accept CV_8UC1
    if (im.channels() != 1 || im.type() != CV_8U){
        cout << "not the right type" << endl;
        return;
    }

    // Find all contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(im.clone(), contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

    for (size_t i = 0; i < contours.size(); i++){
        // look for hierarchy[i][3]!=-1, ie hole boundaries
        if ( (hierarchy[i][3] != -1) && (contourArea(contours[i]) < thresh) ) {
            // fill with white
            drawContours( im, contours, i, Scalar(255,255,255),-1  );
        }
        //drawContours(im, contours, i, Scalar(255,255,255),-1);
    }
    cout << endl << endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////// Segmentation Large //////////////////////////////////////////
void segmentationLarge(Mat img){
	// grayscale
	Mat binaryLarge,binarySmall;
	// Split Channels
	Mat channel[3];
	split(img,channel);
	channel[0].convertTo(binaryLarge, CV_8U);
	channel[1].convertTo(binarySmall, CV_8U);

	//OTSU and remove noise (Large and Small)
	threshold(binaryLarge, binaryLarge, 0, 255, THRESH_BINARY | THRESH_OTSU); // large
	removeSmallBlobs(binaryLarge, 70); //large
	threshold(binarySmall, binarySmall, 120, 255, THRESH_BINARY ); //small

	// threshold (Small)
	threshold(binarySmall, binarySmall, 120, 255, THRESH_BINARY );

	// distance transform (Small)
	Mat dist = Mat::zeros(binarySmall.size(), CV_8U);
	distanceTransform(binarySmall, dist, DIST_L2, 3);
	normalize(dist, dist, 0, 1.0, NORM_MINMAX);

	// threshold (Small)
	threshold(dist, dist, 0, 255, THRESH_BINARY );
	dist.convertTo(dist, CV_8U);

	// Find Small Cells
	findEllipse(dist,img);

	// Find Large Cells
	findEllipse(binaryLarge, img);

}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////// Segmentation Small //////////////////////////////////////////
void segmentationSmall(Mat img){
	// grayscale
	Mat binary;
	// Split Channels
	Mat channel[3];
	split(img,channel);


	// threshold
	threshold(binary, binary, 120, 255, THRESH_BINARY );

	// distance transform
	Mat dist = Mat::zeros(binary.size(), CV_8U);
	distanceTransform(binary, dist, DIST_L2, 3);
	normalize(dist, dist, 0, 1.0, NORM_MINMAX);

	// threshold
	threshold(dist, dist, 0, 255, THRESH_BINARY );
	dist.convertTo(dist, CV_8U);

    findEllipse(dist,img);

}
//////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv){
	// Check arguments and display description message if necessary
	if(argc != 2){
		cout << "Usage: Segment cell nucleus and their spots" << endl;
		return -1;
	}

	// Read input file
	Mat image;
	image = imread(argv[1]);

	// Check to see if the file was read correctly
	if(!image.data){
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	// Show original image
	namedWindow("Original", WINDOW_NORMAL);
	imshow("Original", image);

	// Segment
	segmentationLarge(image.clone());
	//segmentationSmall(image.clone());

	waitKey(0);
	return 0;
}
