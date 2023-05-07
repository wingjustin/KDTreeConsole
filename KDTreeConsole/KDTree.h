#pragma once

#include <math.h>
#include "MinHeap.h"
#include "KDTreeNode.h"
#include "KDTreeCoordinatesContainerCollection.h"

#define _POS_INFINITY_FLOAT_WORD 0x7F800000
#define _NEG_INFINITY_FLOAT_WORD 0xFF800000
#define _NAN_FLOAT_WORD 0xFFC00000
#define _ABSOLUTE_FLOAT_WORD  0x7FFFFFFF
#define _FLOAT_WORD(x)  *(int*)(&x)

#define MIN_K_INT 2
#define DEFAULT_BBF_MAX_SEARCHTIMES (unsigned int) 999

using namespace BinaryTree;

namespace Tree {
	class KDTree {
		int k;
		KDTreeNode* root;

		// calculate variance
		int FindMaxVarianceDimension(int currentDimension, KDTreeCoordinates* coordinates[], int startIndex, int endIndex); // return max dimension

		// find median
		KDTreeCoordinates*& GetPivotByMedian3(int axis, KDTreeCoordinates*& start, KDTreeCoordinates*& middle, KDTreeCoordinates*& end); //return pivot
		void Swap(KDTreeCoordinates*& left, KDTreeCoordinates*& right);

		int GetLeftSplitIndex(int axis, KDTreeCoordinates* coordinates[], int startIndex, int endIndex);
		int GetRightSplitIndex(int axis, KDTreeCoordinates* coordinates[], int startIndex, int endIndex);
		int GetMedianSplitIndex(int axis, KDTreeCoordinates* coordinates[], int startIndex, int endIndex);

		//build tree
		KDTreeNode* Build(KDTreeNode* parent, int currentDimension, KDTreeCoordinates* coordinates[], int startIndex, int endIndex);

		// distance calculation
		float CalculateSquaredEuclideanDistance(KDTreeCoordinates& coordinate1, KDTreeCoordinates& coordinate2);
		float CalculateSquaredDomainDistance(KDTreeNode& pivot, KDTreeCoordinates& target);
		float CalculateManhattanDistance(KDTreeCoordinates& coordinate1, KDTreeCoordinates& coordinate2);
		float CalculateDomainDistance(KDTreeNode& pivot, KDTreeCoordinates& target);

		// find nearest neighbour
		void FindNearestNeighbour(KDTreeCoordinates* target, KDTreeNode* currentNode
			, KDTreeCoordinatesContainerCollection*& nearestNeighbourCollection, float& nearestDistance); // recursion
		void FindNearestNeighbour_BBF(KDTreeCoordinates* target, unsigned int maxCheckTimes
			, KDTreeCoordinatesContainerCollection*& nearestNeighbourCollection, float& nearestDistance); // iteration, maxCheckTimes = 0 = no limit
	public:
		KDTree(int k);
		~KDTree();

		void Clear();

		int Get_K();

		void Build(KDTreeCoordinates* coordinates[], int length);

		//find nearest neighbour
		float FindNearestNeighbour(KDTreeCoordinates* target, KDTreeCoordinatesContainerCollection*& nearestNeighbourCollection); // return nearest distance
		float FindNearestNeighbour_BBF(KDTreeCoordinates* target, KDTreeCoordinatesContainerCollection*& nearestNeighbourCollection); // return nearest distance
		float FindNearestNeighbour_BBF(KDTreeCoordinates* target, unsigned int maxCheckTimes, KDTreeCoordinatesContainerCollection*& nearestNeighbourCollection); // return nearest distance, maxCheckTimes = 0 = no limit
	};
}
