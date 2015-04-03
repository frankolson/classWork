#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define PI 3.14159265

using namespace cv;
using namespace std;

/* Draw Axis */
void drawAxis(Mat& out, RotatedRect box){
	// initial setup
	double angle = box.angle;
	int h = box.size.height;
	int w = box.size.width;

	// Initialize center point
	int xInitial = box.center.x;
	int yInitial = box.center.y;

	/* Major Axis */
	// Top point
	Point Top;
	Top.x = ((w/2)*(cos(angle*PI/180))) + xInitial;
	Top.y = ((w/2)*(sin(angle*PI/180))) + yInitial;
	// Bottom Point
	Point Bottom;
	Bottom.x = xInitial - ((w/2)*(cos(angle*PI/180)));
	Bottom.y = yInitial - ((w/2)*(sin(angle*PI/180)));
	// Draw Major Axis
	line(out, Top, Bottom, Scalar(0,255,0));

	/* Minor Axis */
	// Left point
	Point Left;
	Left.x = ((h/2)*(cos((angle+90)*PI/180))) + xInitial;
	Left.y = ((h/2)*(sin((angle+90)*PI/180))) + yInitial;
	// Right Point
	Point Right;
	Right.x = xInitial - ((h/2)*(cos((angle+90)*PI/180)));
	Right.y = yInitial - ((h/2)*(sin((angle+90)*PI/180)));
	// Draw Major Axis
	line(out, Left, Right, Scalar(0,255,0));

}

/* Fit ellipse */
void findEllipse(Mat& img){
	// find contours
	vector<vector<Point> > contours;
	findContours(img, contours, RETR_LIST, CHAIN_APPROX_NONE);

	// initialize output picture
	Mat cimage = Mat::zeros(img.size(), CV_8UC3);

	// fit the ellipses
	for (size_t i = 0; i < contours.size()-1; i++){
		size_t count = contours[i].size();
        if( count < 6 )
            continue;

        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);

        // Draw Axis
        drawAxis(cimage, box);

        if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*30 )
            continue;
        drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);

        ellipse(cimage, box, Scalar(0,0,255), 1, LINE_AA);
        ellipse(cimage, box.center, box.size*0.5f, box.angle, 0, 360, Scalar(0,255,255), 1, LINE_AA);
        Point2f vtx[4];
        box.points(vtx);
        for( int j = 0; j < 4; j++ )
            line(cimage, vtx[j], vtx[(j+1)%4], Scalar(0,255,0), 1, LINE_AA);
	}

	// show results in a sizable window
	namedWindow("Result", WINDOW_NORMAL);
	imshow("Result", cimage);
}

/** @function main */
int main( int argc, char** argv ){
	// Load source image and convert it to gray
	Mat src,src_gray;
	src = imread( argv[1], 1 );

	// Convert image to gray and blur it
	cvtColor( src, src_gray, COLOR_BGR2GRAY );
	for(int j = 0; j < 5; j++){
		for ( int i = 1; i < 5; i = i + 2 ){
		medianBlur( src_gray, src_gray, i );
		}
	}
	// binarize image
	Mat binary;
	threshold(src_gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU );

	// find ellipse
	findEllipse(binary);

	waitKey(0);
	return(0);
}
