#include "KDTreeNode.h"

using namespace Tree;

KDTreeNode::KDTreeNode() {
	KDTreeNode::axis = INT_MIN;
	KDTreeNode::coordinates = NULL;

	KDTreeNode::parent = NULL;
	KDTreeNode::left = NULL;
	KDTreeNode::right = NULL;
}

KDTreeNode::KDTreeNode(int axis, KDTreeCoordinates* coordinate) : KDTreeNode() {
	KDTreeNode::axis = axis;
	KDTreeNode::coordinates = coordinate;
}

KDTreeNode::KDTreeNode(KDTreeNode* parent) : KDTreeNode() {
	KDTreeNode::parent = parent;
}

KDTreeNode::KDTreeNode(KDTreeNode* parent, int axis, KDTreeCoordinates* coordinate) : KDTreeNode(parent) {
	KDTreeNode::axis = axis;
	KDTreeNode::coordinates = coordinate;
}

KDTreeNode::~KDTreeNode() {
	delete coordinates;

	delete left;
	delete right;

	coordinates = NULL;

	parent = NULL;
	left = NULL;
	right = NULL;
}

bool KDTreeNode::IsEmpty() {
	return coordinates == NULL;
}

int KDTreeNode::GetAxis() {
	return axis;
}

KDTreeCoordinates* KDTreeNode::GetCoordinates() {
	return coordinates;
}

KDTreeNode* KDTreeNode::GetParent() {
	return parent;
}

KDTreeNode* KDTreeNode::GetLeftChild() {
	return left;
}

void KDTreeNode::SetLeftChild(KDTreeNode* child) {
	left = child;
}

KDTreeNode* KDTreeNode::GetRightChild() {
	return right;
}

void KDTreeNode::SetRightChild(KDTreeNode* child) {
	right = child;
}
