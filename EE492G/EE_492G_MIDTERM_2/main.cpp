#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/////////////////////////////////////// Segmentation /////////////////////////////////////////////
Mat segmentation(Mat image){
	// grayscale
	Mat binary;
	// Split Channels
	Mat channel[3];
	split(image,channel);
	channel[0].convertTo(binary, CV_8U);
	imshow("BLUE", binary);

	// Reduce Noise
	GaussianBlur(binary, binary, Size(5, 5), 0.5, 0.5); //applying Gaussian filter
	GaussianBlur(image, image, Size(5, 5), 0.5, 0.5); //applying Gaussian filter

	//OTSU and remove noise200,
	threshold(binary, binary, 0, 255, THRESH_BINARY | THRESH_OTSU); //TO-DO: bwAreaOpen(matlab)

	// Calculate distance transform
	Mat objects, fg;
	Mat dist = Mat::zeros(binary.size(), CV_8U);
	distanceTransform(binary, dist, DIST_L2, 3);
	normalize(dist, dist, 0, 1.0, NORM_MINMAX);

	// Threshold the distance transform results
	vector<vector<Point> > contours;
	threshold(dist, objects, 0, 255, THRESH_BINARY);

	// Dilate threshold results
	dilate(dist, fg, Mat(), Point(-1, -1), 10);
	dilate(objects, objects, Mat(), Point(-1, -1), 10);

	// Subtract small foreground from large foreground
	fg = fg - dist;

	// Threshold and convert results
	threshold(fg, fg, 0, 255, THRESH_BINARY);
	fg.convertTo(fg, CV_8U);
	objects.convertTo(objects, CV_8U);

	// cluster peaks
	bitwise_xor(fg, objects, fg);
	dilate(fg, fg, cv::Mat(), cv::Point(-1, -1), 3);

	// Identify image pixels without objects
	Mat bg;
	dilate(binary, bg, Mat(), Point(-1, -1), 1);
	threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);

	// connected components used to separate each peak (bean)
	// This is really important as  the watershed algorithm needs
	// separately identified intensity value valleys in order to
	// segment properly
	Mat dt;
	fg.copyTo(dt);
	dt.convertTo(dt, CV_8U);
	Mat lbl(dt.size(), CV_32S);
	int ncc = connectedComponents(dt, lbl, 4);
	cout << "Number of Beans: " << ncc - 1 << endl;

	// Create a gray color spectrum to paint the peaks
	vector<unsigned char> colors(ncc);
	colors[0] = 0; // for background
	for (int label = 1; label < ncc; ++label)
		colors[label] = 255 / ncc*label;

	// Paint every separate peak with a shade of gray
	for (int x = 0; x < dt.rows; x++) {
		for (int y = 0; y < dt.cols; y++) {
			int label = lbl.at<int>(x, y);
			dt.at<unsigned char>(x, y) = colors[label];
		}
	}

	// Create markers image
	Mat markers(binary.size(), CV_8U, Scalar(0));
	markers = dt + bg;

	// Convert to image of ints
	markers.convertTo(markers, CV_32S);

	// Apply watershed
	watershed(image, markers);

	// show segmentation
	Mat tmp;
	markers.convertTo(tmp, CV_8U);

	// show watershed results
	Mat tmp2;
	markers.convertTo(tmp2, CV_8U, 255, 255);
	namedWindow( "watershed", WINDOW_NORMAL );
	imshow("watershed", tmp2);

	return tmp2;
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

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", image);

	// Segment
	Mat segment;
	segment = segmentation(image);

	waitKey(0);
	return 0;
}
