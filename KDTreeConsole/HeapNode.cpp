#include "HeapNode.h"

using namespace BinaryTree;

HeapNode::HeapNode(const float key, KDTreeNode* const data) {
	HeapNode::key = key;
	HeapNode::data = data;

	HeapNode::parent = NULL;
	HeapNode::left = NULL;
	HeapNode::right = NULL;
}

HeapNode::~HeapNode() {
	delete left;
	delete right;

	data = NULL;

	parent = NULL;
	left = NULL;
	right = NULL;
}

float HeapNode::GetKey() {
	return key;
}

KDTreeNode* HeapNode::GetData() {
	return data;
}
