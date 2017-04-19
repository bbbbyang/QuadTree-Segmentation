//----------------------------------------------------------------------------
//							BINGYANG LIU
//							OPENCV 3.0.0
//----------------------------------------------------------------------------

//		enumerate four quadrants
//		UL(0)		|		UR(3)
//		------------------------
//		LL(1)		|		LR(2)
//

#include "QuadTree.h"
#include <queue>

//--------------------------BoundaryBox Part----------------------------------
/**
 * Default initialize the boundaryBox area
 */
BoundaryBox::BoundaryBox() : LowerBound(0.0, 0.0), UpperBound(0.0, 0.0){
}

/**
 * Initialize the boundaryBox area
 * @param LowerBound top and left coordinate of a boundary
 * @param UpperBound bottom and right coordinate of a boundary
 */
BoundaryBox::BoundaryBox(Vector2 const lower, Vector2 const upper) : LowerBound(lower), UpperBound(upper){
}

/**
 * Get UL area boundaryBox parameters
 */
BoundaryBox BoundaryBox::GetUL(){
	Vector2 lb;
	Vector2 ub;
	lb.set(LowerBound.x, LowerBound.y);
	ub.set((LowerBound.x + UpperBound.x) / 2.0, (LowerBound.y + UpperBound.y) / 2.0);
	return BoundaryBox(lb, ub);
}

/**
 * Get LL area boundaryBox parameters
 */
BoundaryBox BoundaryBox::GetLL(){
	Vector2 lb;
	Vector2 ub;
	lb.set((LowerBound.x + UpperBound.x) / 2.0, LowerBound.y);
	ub.set(UpperBound.x, (LowerBound.y + UpperBound.y) / 2.0);
	return BoundaryBox(lb, ub);
}

/**
 * Get LR area boundaryBox parameters
 */
BoundaryBox BoundaryBox::GetLR(){
	Vector2 lb;
	Vector2 ub;
	lb.set((LowerBound.x + UpperBound.x) / 2.0, (LowerBound.y + UpperBound.y) / 2.0);
	ub.set(UpperBound.x, UpperBound.y);
	return BoundaryBox(lb, ub);
}

/**
 * Get UR area boundaryBox parameters
 */
BoundaryBox BoundaryBox::GetUR(){
	Vector2 lb;
	Vector2 ub;
	lb.set(LowerBound.x, (LowerBound.y + UpperBound.y) / 2.0);
	ub.set((LowerBound.x + UpperBound.x) / 2.0, UpperBound.y);
	return BoundaryBox(lb, ub);
}

/**
 * @return lowerboundary x coordinate
 */
double BoundaryBox::LBx(){
	return LowerBound.x;
}

/**
 * @return lowerboundary y coordinate
 */
double BoundaryBox::LBy(){
	return LowerBound.y;
}

/**
 * @return upperboundary x coordinate
 */
double BoundaryBox::UBx(){
	return UpperBound.x;
}

/**
 * @return upperboundary y coordinate
 */
double BoundaryBox::UBy(){
	return UpperBound.y;
}

/**
 * Print the boundary of the area
 */
void BoundaryBox::Print(){
	cout<<LowerBound;
	cout<<UpperBound<<endl;
}

Vector2 BoundaryBox::GetCenter() const{
	return (LowerBound + UpperBound) / 2.0f;
}

/**
 * Get the length of the rectangle
 */
int BoundaryBox::GetLength() const{
	return (UpperBound.x - LowerBound.x);
}

/**
 * The condition to decide to subdivide or not
 */
double BoundaryBox::GetDims() const{
	double dis;
	dis = UpperBound.getDistanceSquared(LowerBound);
	return dis;
}

/**
 * Return the size this box
 */
double BoundaryBox::Box_Area(){
	double Height = UpperBound.x - LowerBound.x;
	double Width = UpperBound.y - LowerBound.y;

	return (Height * Width);
}
//----------------------------------------------------------------------------


//---------------------------QuadNode Part-----------------------------------
/**
 * Default initialize node
 */
QuadNode::QuadNode(){
	Children[0] = NULL;
	Children[1] = NULL;
	Children[2] = NULL;
	Children[3] = NULL;
	rect = BoundaryBox(Vector2(0, 0), Vector2(0, 0));
	HasChildren = false;
	Label = -1;
	Aver = -1;
}

/**
 * Initialize node
 */
QuadNode::QuadNode(BoundaryBox& re){
	Children[0] = NULL;
	Children[1] = NULL;
	Children[2] = NULL;
	Children[3] = NULL;
	rect = re;
	HasChildren = false;
	Label = -1;
	Aver = -1;
}

/**
 * Divide a quadrent into four equal parts ready for further
 * insertion of nodes.
 */
void QuadNode::SubdivideQuadTree(){
	BoundaryBox Box = rect;

	// Split the quadrant into four parts
	BoundaryBox URBox = Box.GetUR();
	BoundaryBox ULBox = Box.GetUL();
	BoundaryBox LLBox = Box.GetLL();
	BoundaryBox LRBox = Box.GetLR();
	// This node has children
	HasChildren = true;
	// Get children nodes initilized
	Children[0] = new QuadNode(ULBox);
	Children[1] = new QuadNode(LLBox);
	Children[2] = new QuadNode(LRBox);
	Children[3] = new QuadNode(URBox);
}
//----------------------------------------------------------------------------


//---------------------------QuadTree Part-----------------------------------
/**
 * Constractor of QuadTree
 */
QuadTree::QuadTree(BoundaryBox re, Mat& Image){
	Area = re;
	Img = Image;
}

/**
 * Create QuadTree root and branches
 * Use queue to create branches of the tree
 */
void QuadTree::CreateQuadTree(){
	// Create root of the tree
	if(!(Root = new QuadNode()))
		cout<<"Memory Allocate Error";
	Root->rect = Area;
//	depth = 1;

	// Queue for creating branches
	queue<QuadNode*> TreeQueue;
	TreeQueue.push(Root);

	// Address of Root
	QuadNode* Record_Of_Root = Root;

	while(!TreeQueue.empty()){
		// Get front element
		Root = TreeQueue.front();
		TreeQueue.pop();
		if(Root->rect.GetDims() != 2){
			if(Sub_Or_Not(Root)){
				Root->SubdivideQuadTree();

				TreeQueue.push(Root->Children[0]);
				TreeQueue.push(Root->Children[1]);
				TreeQueue.push(Root->Children[2]);
				TreeQueue.push(Root->Children[3]);
			}
		}
	}
	Root = Record_Of_Root;
}

/**
 * The condition for subdivide
 */
bool QuadTree::Sub_Or_Not(QuadNode* Node){
	// Get the boundary of this node in the image
	// Get the region of interest image
	Mat NodeImage = Img(Range(Node->rect.LBx(), Node->rect.UBx()), Range(Node->rect.LBy(), Node->rect.UBy()));

	// Get the minimum and maximum intensity of the node image
	double Max_Value;
	double Min_Value;
	minMaxLoc(NodeImage, &Min_Value, &Max_Value);

	// Calculate the difference between maximum and minimum value
	double Diff = Max_Value - Min_Value;
//	cout<<Diff<<endl;
	// Return the result for sub
	if(Diff < Intensity_Threshold)
		return false;
	else return true;
}

/**
 * Preoder traversal
 */
void QuadTree::Print_Tree(QuadNode* Node){
	if(Node == NULL)
		return;
	Node->rect.Print();
	Print_Tree(Node->Children[0]);
	Print_Tree(Node->Children[1]);
	Print_Tree(Node->Children[2]);
	Print_Tree(Node->Children[3]);
}

/**
 * Mark the leaf node with condition and store the leaf nodes in vector
 */
void QuadTree::Mark_The_LeafNode(QuadNode* Node){
	if(Node == NULL)
		return;
	if(!Node->HasChildren){
		// Store the leaf node in vector
		Leaf_Node_List.push_back(Node);

		if(Node->rect.GetDims() == 2){
			// Mark the leaf node has only one pixel
			Mat ROI_Node_Img = Img(Range(Node->rect.LBx(), Node->rect.UBx()), Range(Node->rect.LBy(), Node->rect.UBy()));
			Node->Aver = ROI_Node_Img.at<uchar>(0, 0);
		//	ROI_Node_Img = 255;
		}
		else{
			// Get average intensity of every segment
			Segment_Of_Image(Node);
			Mark_the_Boundary(Node);
		}
	}
	Mark_The_LeafNode(Node->Children[0]);
	Mark_The_LeafNode(Node->Children[1]);
	Mark_The_LeafNode(Node->Children[2]);
	Mark_The_LeafNode(Node->Children[3]);
}

/**
 * Get the average intensity of very part of the leaf node
 * which is not only one pixel
 */
void QuadTree::Segment_Of_Image(QuadNode* Node){
	// Set this boundary into image
	Mat ROI_Node_Img = Img(Range(Node->rect.LBx(), Node->rect.UBx()), Range(Node->rect.LBy(), Node->rect.UBy()));
	
	// Get sum up of all intensity
	int Sum_Intensity = 0;
	int Aver_Intensity = 0;
	for(int i = 0; i < ROI_Node_Img.rows; i++){
		for(int j = 0; j < ROI_Node_Img.cols; j++){
			Sum_Intensity += ROI_Node_Img.at<uchar>(i, j);
		}
	}
	
	// Get average intensity
	Aver_Intensity = 1.0 * Sum_Intensity / Node->rect.Box_Area();
	
	// Set the average value
	Node->Aver = Aver_Intensity;
	ROI_Node_Img = Aver_Intensity;
}

void QuadTree::Mark_the_Boundary(QuadNode* Node){
	// Get the region of interest
	Mat ROI_Node_Img = Img(Range(Node->rect.LBx(), Node->rect.UBx()), Range(Node->rect.LBy(), Node->rect.UBy()));
	ROI_Node_Img.row(0) = 0;
	ROI_Node_Img.col(0) = 0;
//	ROI_Node_Img.row(ROI_Node_Img.rows - 1) = 0;
//	ROI_Node_Img.col(ROI_Node_Img.cols - 1) = 0;
}