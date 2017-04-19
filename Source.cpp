//----------------------------------------------------------------------------
//							BINGYANG LIU
//							OPENCV 3.0.0
//----------------------------------------------------------------------------

// The image size should be 2^k * 2^k

#include "QuadTree.h"
#include <iostream>
#include <vector>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

int main(){

	// Load image to gray level and resize the image
	// For Gray Scale
	Mat Img = imread("house.tif", 0);
	
	// Get boundarybox area of the image
	int Rows = Img.rows;
	int Cols = Img.cols;
	BoundaryBox Area(Vector2(0, 0), Vector2(Rows, Cols));

	// Implement a quadtree
	QuadTree Tree(Area, Img);
	Tree.CreateQuadTree();
	
	Tree.Mark_The_LeafNode(Tree.Root);

	resize(Img, Img, Size(512, 512), 0, 0, 1);
	namedWindow("Segment Image");
	imshow("Segment Image", Img);
	imwrite("QuadTreeSegmentation.tif", Img);
	waitKey();
	
	return 1;
}