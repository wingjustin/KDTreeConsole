#pragma once

#include "KDTreeNode.h"

using namespace std;
using namespace Tree;

namespace BinaryTree {
	class HeapNode {
		float key; // distance
		KDTreeNode* data; // sibling node

		HeapNode* parent;
		HeapNode* left;
		HeapNode* right;

		//friend class MaxHeap;
		friend class MinHeap;
	public:
		HeapNode(float key, KDTreeNode* data);
		~HeapNode();

		float GetKey();
		KDTreeNode* GetData();
	};
}
