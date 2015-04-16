/*************************** Delaunay Triangulation ***************************/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iterator>

#define CURVE_THRESH 0

using namespace std;
using namespace cv;

////////////////////////////////// Delaunay Traingulation ////////////////////////////////////////
void triangulation( Mat& img, Subdiv2D& subdiv, Scalar delaunay_color )
{
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point> pt(3);
    bool draw;

    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));

        // get rid of lines trailing to the outside
        draw = true;
        for(int j = 0; j < 3 ; j++){
        	if(pt[j].x > img.size().width || pt[j].y > img.size().height || pt[j].x < 0 || pt[j].y < 0){
        		draw=false;
        	}
        }
        if(draw){
			line(img, pt[0], pt[1], delaunay_color, 1, LINE_AA, 0);
			line(img, pt[1], pt[2], delaunay_color, 1, LINE_AA, 0);
			line(img, pt[2], pt[0], delaunay_color, 1, LINE_AA, 0);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// index ////////////////////////////////////////////////
int index(int i, int j, int n){
	if ((i + j) < 0){
		return (n + j);
	}
	else{
		return ((i+j)%(n));
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////// Gaussian Filter ///////////////////////////////////////////
vector<float> gaussian(int sigma){
	// initiate variables
	int width = 9 * sigma + 1;
	int w = (9 * sigma) / 2;
	vector<float> kernal(width);
	float sum = 0.0;
	float den = 2*sigma*sigma;

	// populate kernal
	for (int i = -w; i <= w; i++){
		// multiply times constant and fill kernal
		kernal[i+w] = (exp(-(i*i)/den))/(sqrt(CV_PI * 2)*sigma);
		sum += kernal[i+w];
	}

	// Normalize answer
	for (int i = 0; i < width; i++)
		kernal[i] /= sum;

	// return kernal
	return kernal;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Guassian Filter derivative (dG) ////////////////////////////////////
vector<float> gaussianDerivative(int sigma){
	// initiate variables
	int width = 9 * sigma + 1;
	int w = (9 * sigma) / 2;
	vector<float> kernal(width);
	float sum = 0.0;
	float den = 2 * sigma*sigma;

	// populate kernal
	for (int i = -w; i <= w; i++){
		// multiply times constant and fill kernal
		kernal[i + w] = -(exp(-(i*i) / den) * i) / ((sqrt(CV_PI * 2)*pow(sigma,3)));
		sum += kernal[i + w];
	}

	// Normalize answer
	for (int i = 0; i < width; i++)
		kernal[i] /= sum;

	// return kernal
	return kernal;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////// Guassian Filter derivative (ddG) ////////////////////////////////////
vector<float> gaussianDerivative2(int sigma){
	// initiate variables
	int width = 9 * sigma + 1;
	int w = (9 * sigma) / 2;
	vector<float> kernal(width);
	float sum = 0.0;
	float den = 2 * sigma*sigma;

	// populate kernal
	for (int i = -w; i <= w; i++){
		// multiply times constant and fill kernal
		kernal[i + w] = ((exp(-(i*i) / den)) * ((i-sigma)*(i+sigma))) / (sqrt(CV_PI * 2)*pow(sigma,5));
		sum += kernal[i + w];
	}

	// Normalize answer
	for (int i = 0; i < width; i++)
		kernal[i] /= sum;

	// return kernal
	return kernal;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// Circular Convolution /////////////////////////////////////////
vector<float> cconv(vector<float> data, vector<float> filter){
	// obtain filter sizes
	int n = data.size();
	int w = filter.size();

	// find the center
	int c;
	if (w % 2 == 0){
		c = w / 2;
	}
	else{
		c = (w + 1) / 2;
	}

	// create zero filled vector
	vector<float> y(n);
	float fillVal = 0.0;
	fill(y.begin(), y.end(), fillVal);

	// perform circular convolution
	for (int i = 0; i < n; i++){
		for (int j = -c; j < c-1; j++){
			y[i] += filter[j + c] * data[index(i, j, n)];
		}
	}

	return y;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////// Vector Power /////////////////////////////////////////////
void vectPower(vector<float> A, int power, vector<float>& out){
	// apply exponent
	for (size_t i = 0; i < A.size(); i++){
		out[i] = pow(A[i], power);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////// Find Curvature ///////////////////////////////////////////
void getCurvature(vector<vector<Point> >& contours, vector<vector<float> >& k, Mat img){
	// Initialize variables
	int sig = 2;

	// create guassian kernal
	vector<float> G;
	G = gaussian(sig);

	// Gaussian derivations
	vector<float> dG, ddG;
	dG = gaussianDerivative(sig); // 1st
	ddG = gaussianDerivative2(sig); // 2nd

	// find largest contour
	double largestArea = 0.0;
	int largestContour = 0;
	for(size_t a = 0; a < contours.size(); a++){
		if(largestArea < contourArea(contours[a])){
			largestArea = contourArea(contours[a]);
			largestContour = a;
		}
	}

	// Apply curvature equation
	const vector<Point>& c = contours[largestContour];

	vector<float> X(c.size());
	vector<float> Y(c.size());
	cout << endl << " Contour: " << largestContour << endl;
	for (size_t j = 0; j < c.size(); j++)
	{
		// fill one dimensional matrix x and y
		X[j] = c[j].x;
		Y[j] = c[j].y;
		//cout << X[j] << "," << Y[j] << endl;
	}

	// get derivatives
	vector<float> dx = cconv(X, dG);
	vector<float> ddx = cconv(X, ddG);
	vector<float> dy = cconv(Y, dG);
	vector<float> ddy = cconv(Y, ddG);

	// curvature operations
	vector<float> A(dx.size()), B(dy.size()), C(dx.size()), D(dx.size()),
					E(dy.size()), F(dx.size()), H(dx.size());

	transform(dx.begin(), dx.end(), ddy.begin(), A.begin(), multiplies<float>());
	transform(dy.begin(), dy.end(), ddx.begin(), B.begin(), multiplies<float>());
	transform(A.begin(), A.end(), B.begin(), C.begin(), minus<float>());

	vectPower(dx, 2, D);
	vectPower(dy, 2, E);

	transform(D.begin(), D.end(), E.begin(), F.begin(), plus<float>());
	//add(D, E, F);
	vectPower(F, 1.5, H);

	// curvature
	vector<float> kl(H.size());
	transform(H.begin(), H.end(), C.begin(), kl.begin(), divides<float>());

	//k[i] = kl;
	cout << "Max element: " << *max_element(kl.begin(), kl.end()) << endl << endl;


	// write output file
	cout << A.size() << endl;
	ofstream os ("data.txt");
	for(size_t i = 0; i < kl.size(); i++){
		os << i << " " << kl[i] << endl;
	}
	os.close();

	// Create Subdiv2d for Delaunay Triangulation
	Rect imageSize(0,0,img.size().width, img.size().height);
	Subdiv2D subdiv(imageSize);

	//TODO Convert kl back to k
	// plot points of curvature
	Mat imgCopy = img.clone();
	for(size_t p = 0; p < kl.size(); p++){
		if(kl[p] > CURVE_THRESH){
			Point2f center = Point(X[p],Y[p]);
			circle(imgCopy, center, 1, Scalar(0,0,255), -1);

			// insert point into subdiv for delaunay
			subdiv.insert(center);
		}
	}
	imshow("Points of Maximum Curvature", imgCopy);

	// draw triangulation
	triangulation(img, subdiv, Scalar(0,0,255));
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////// Segment and Contours ////////////////////////////////////////
void segment(Mat& img, Mat& imgColor){
	// OTSU
	Mat binary;
	threshold(img, binary, 128, 255, THRESH_BINARY_INV | THRESH_OTSU);
	erode(binary, binary, Mat(), Point(-1,-1), 1);
	//imshow("Binary",binary);

	// get contours
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));

	// draw only the largest contour
	// find largest contour
	double largestArea = 0.0;
	int largestContour = 0;
	for(size_t i = 1; i < contours.size(); i++){
		if(largestArea < contourArea(contours[i])){
			largestArea = contourArea(contours[i]);
			largestContour = (int)i;
		}
	}

	//draw contours
	Mat colorCopy = imgColor.clone();
	drawContours(colorCopy, contours, largestContour, Scalar(0,0,255),2,8);
	imshow("Contours", colorCopy);

	// get curvature the triangulation
	vector<vector<float> > k;
	getCurvature(contours, k, imgColor);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv){
	// Account for wrong number arguments
	if (argc != 2)
	{
		cout << "Usage: Calculating curvature" << endl;
		return -1;
	}

	// Read the file and create grayscale
	Mat image,gray;
	image = imread(argv[1]);
	cvtColor(image,gray, COLOR_BGR2GRAY);

	// Check for invalid input
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// CREATE SYNTHETIC SQUARE FOR TESTING
	Mat image1 = Mat(500, 500, CV_8UC1, double(50));
	Mat image2 = Mat(250, 250, CV_8UC1, double(150));
	image2.copyTo(image1.colRange(124,374).rowRange(124,374));
	Mat imageColor;
	cvtColor(image1,imageColor, COLOR_GRAY2BGR);

	// Draw triangulation
	segment(gray,image);

	// show results
	imshow("Final",image);

	waitKey(0);
	return 0;
}