#pragma once

#include <stdlib.h>
#include "KDTreeCoordinates.h"

namespace Tree {
	class KDTreeNode {
		int axis;
		KDTreeCoordinates* coordinates;

		KDTreeNode* parent;
		KDTreeNode* left;
		KDTreeNode* right;
	public:
		KDTreeNode(); //empty node
		KDTreeNode(int axis, KDTreeCoordinates* coordinate);
		KDTreeNode(KDTreeNode* parent); //empty node
		KDTreeNode(KDTreeNode* parent, int axis, KDTreeCoordinates* coordinate);
		~KDTreeNode();

		bool IsEmpty();

		int GetAxis();
		KDTreeCoordinates* GetCoordinates();

		KDTreeNode* GetParent();

		KDTreeNode* GetLeftChild();
		void SetLeftChild(KDTreeNode* child);
		KDTreeNode* GetRightChild();
		void SetRightChild(KDTreeNode* child);
	};
}
