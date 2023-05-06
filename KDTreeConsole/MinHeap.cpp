#include "MinHeap.h"

using namespace BinaryTree;

MinHeap::MinHeap() {
	carry = 0;
	length = 0;
	root = NULL;
}

MinHeap::~MinHeap() {
	delete root;
	root = NULL;
}

bool MinHeap::IsEmpty() {
	//return length == 0;
	return root == NULL;
}

unsigned int MinHeap::GetLength() {
	return length;
}

HeapNode* MinHeap::GetRoot() {
	return root;
}

void MinHeap::Swap(HeapNode& node1, HeapNode& node2) {
	float tempKey = node1.key;
	KDTreeNode* tempData = node1.data;
	node1.key = node2.key;
	node1.data = node2.data;
	node2.key = tempKey;
	node2.data = tempData;
	tempData = NULL;
}

void MinHeap::InsertNode(HeapNode& newNode) {
	length++;
	if (!root) {
		root = &newNode;
		carry = 1;
		return;
	}
	if (length & (carry << 1))
		carry <<= 1;
	//find the parent of new node using bit matching
	HeapNode* parent = root;
	int parentIndex = length >> 1;
	int bitMatch = carry >> 1;
	while (bitMatch >>= 1) {
		if (parentIndex & bitMatch) // go to right child
			parent = parent->right;
		else // go to left child
			parent = parent->left;
	}
	newNode.parent = parent;
	if (length & 1)
		parent->right = &newNode;
	else
		parent->left = &newNode;
}

void MinHeap::Swim(HeapNode& target) {
	if (root == &target)
		return;
	HeapNode* current = &target;
	while (current->parent
		&& current->key < current->parent->key) {
		Swap(*current, *current->parent);
		current = current->parent;
	}
}

HeapNode* MinHeap::RemoveRoot() {
	if (!root)
		return NULL;
	//get last node using bit matching
	HeapNode* tail = root;
	int bitMatch = carry;
	while (bitMatch >>= 1) {
		if (length & bitMatch) // go to right child
			tail = tail->right;
		else // go to left child
			tail = tail->left;
	}
	Swap(*root, *tail);
	//remove tail
	if (tail == root)
		root = NULL;
	else {
		if (length & 1) // is right child
			tail->parent->right = NULL;
		else // is left child
			tail->parent->left = NULL;
		tail->parent = NULL;
	}
	length--;
	if (!(length & carry))
		carry >>= 1;
	return tail;
}

void MinHeap::Sink(HeapNode& target) {
	HeapNode* current = &target;
	while (current  && current->left) {
		if (current->right
			&& current->right->key < current->key
			&& current->right->key < current->left->key) {
			Swap(*current->right, *current);
			current = current->right; // go to rigth child
		}
		else if (current->left->key < current->key) {
			Swap(*current->left, *current);
			current = current->left; // go to left child
		}
		else
			return;
	}
}

void MinHeap::Push(float key, KDTreeNode& data) {
	HeapNode* newNode = new HeapNode(key, &data);
	InsertNode(*newNode);
	Swim(*newNode);
}

HeapNode* MinHeap::Pop() {
	HeapNode* node = RemoveRoot();
	Sink(*root);
	return node;
}

void MinHeap::Clear() {
	delete root;
	root = NULL;
	//initialize
	carry = 0;
	length = 0;
	//root = NULL;
}

// for debug
bool MinHeap::HealthCheck(HeapNode* current, int& currentHeight, int& diff) {
	if (current->right && current->left) {
		if (current->key > current->right->key)
			return false;
		if (current->key > current->left->key)
			return false;

		int currentLeftHeight = 0;
		int currentLeftDiff = 0;
		int currentRightHeight = 0;
		int currentRightDiff = 0;
		bool health = HealthCheck(current->right, currentRightHeight, currentRightDiff)
			&& HealthCheck(current->left, currentLeftHeight, currentLeftDiff);
		diff = currentLeftHeight - currentRightHeight;

		if (!health)
			return false;

		if (!(diff == 1 || diff == 0))
			return false;
		
		if (currentLeftDiff == 1 && currentRightDiff == 1)
			return false;
		
		currentHeight = currentLeftHeight + 1;

		return true;
	}
	else if (current->left && !current->right) {
		if (current->key > current->left->key)
			return false;

		int currentLeftHeight = 0;
		int currentLeftDiff = 0;
		bool health = HealthCheck(current->left, currentLeftHeight, currentLeftDiff);
		diff = 1;

		if (!health)
			return false;

		if (currentLeftDiff == 1)
			return false;

		currentHeight = currentLeftHeight + 1;

		return true;
	}
	else if (current->right) {
		return false;
	}

	currentHeight++;
	return true;
}

bool MinHeap::HealthCheck() {
	int height = 0;
	while (carry >> height)
		height++;
	int currentHeight = 0;
	int currentDiff = 0;
	return (!root && currentHeight == height)
		|| (HealthCheck(root, currentHeight, currentDiff) && currentHeight == height);
}
