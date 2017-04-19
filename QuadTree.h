//----------------------------------------------------------------------------
//							BINGYANG LIU
//							OPENCV 3.0.0
//----------------------------------------------------------------------------


//		enumerate four quadrants
//		UL(0)		|		UR(3)
//		------------------------
//		LL(1)		|		LR(2)
//

#ifndef QUADTREE_H
#define QUADTREE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <opencv2\highgui\highgui.hpp>
#include "Vector2.h"

using namespace std;
using namespace cv;

const int Intensity_Threshold = 40;

typedef enum{
      UL = 0,	// UR Quadrant
      LL = 1,	// UL Quadrant
      LR = 2,	// LL Quadrant
      UR = 3	// LR Quadrant
}QuadrantEnum;

class BoundaryBox{
	public:
		Vector2 LowerBound;							// bottom and left
		Vector2 UpperBound;							// top and right
	public:
		BoundaryBox();
		BoundaryBox(Vector2 const, Vector2 const);
		Vector2 GetCenter() const;
		int GetLength() const;
		double GetDims() const;
		BoundaryBox GetUR();						// return the UP quadrant of this rect area
		BoundaryBox GetUL();						// return the UL quadrant of this rect area
		BoundaryBox GetLL();						// return the LL quadrant of this rect area
		BoundaryBox GetLR();						// return the LR quadrant of this rect area
		double LBx();								// return lowerboundary x coordinate
		double LBy();								// return lowerboundary y coordinate
		double UBx();								// return upperboundary x coordinate
		double UBy();								// return upperboundary y coordinate
		double Box_Area();
		void Print();
};

/**
 * QuadNode describe a spatial node in the tree
 */
class QuadNode{
	public:
		// Node information
		BoundaryBox rect;							// This node area
		bool HasChildren;							// Check if this node has children
		// Node Link
		QuadNode* Children[4];						// Four children address
		int Label;
		int Aver;
	public:
		QuadNode();
		QuadNode(BoundaryBox&);
		void SubdivideQuadTree();					// Subdivide
//		Vector2 Max_Min_Intensity();				// For gray scale
};

/**
 * QuadTree Management
 */
class QuadTree{
	public:
		Mat Img;
		QuadNode* Root;							// The root of the tree
		int depth;								// The depth of the tree
		BoundaryBox Area;						// The whole image
		vector<QuadNode*> Leaf_Node_List;		// Store leaf nodes
	public:
		QuadTree();
		QuadTree(BoundaryBox, Mat&);
		void CreateQuadTree();						// Create QuadTree
		bool Sub_Or_Not(QuadNode*);					// Decide to subdivide or not
		void Print_Tree(QuadNode*);					// Print QuadTree
		void Segment_Of_Image(QuadNode*);			// Draw the segment with average intensity
		void Mark_The_LeafNode(QuadNode*);			// Mark the leaf node with condition
		void Mark_the_Boundary(QuadNode*);			// Mark the boundary pixel
		void Link_The_Component();					// Link the leaf nodes with same by
};

#endif