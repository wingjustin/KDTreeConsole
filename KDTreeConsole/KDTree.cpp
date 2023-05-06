#include "KDTree.h"

using namespace Tree;

KDTree::KDTree(int k) {
	KDTree::k = k < MIN_K_INT ? MIN_K_INT : k;
	KDTree::root = new KDTreeNode(); // empty node
}

KDTree::~KDTree() {
	delete root;
	root = NULL;
}

void KDTree::Clear() {
	delete root;
	root = NULL;
}

int KDTree::Get_K() {
	return k;
}

//calculate variance
//define mean = sum / n;
// define mx = mean of sum(x)
// variance = sum { (x[i]- mx)^2 } = sum { x[i]^2 + mx^2 - 2*mx*x[i] } / (n - 1)
// variance = [ sum (x[i]^2) + sum (mx^2) -  2*mx * sum (x[i]) ] / (n - 1)
// variance = [ sum (x[i]^2) + n * mx^2 -  2*mx * sum (x[i]) ] / (n - 1)
// float variance = (sqdSum + n * mean * mean - 2 * mean * sum) / (n - 1);
// because mx = sum(x[i]) / n
// so
// variance = [ sum (x[i]^2) + sum(x[i])^2 / n -  2*sum(x[i]) * sum (x[i]) / n ] / (n - 1)
// variance = [ n * sum (x[i]^2) + sum(x[i])^2 -  2*sum(x[i])^2 ] / (n^2 - n)
// variance = [ n * sum (x[i]^2) -  sum(x[i])^2 ] / (n^2 - n)
// float variance = (n * sqdSum - sum * sum) / (n * (n - 1));
int KDTree::FindMaxVarianceDimension(const int currentDimension, KDTreeCoordinates* coordinates[], const int startIndex, const int endIndex) {
	int maxD = 0;
	float maxVariance = -1;
	float coordinate, variance, sum, sqdSum;
	const float n = static_cast<float>(endIndex - startIndex + 1);

	for (int d = 0; d < k; d++) {
		sum = 0, sqdSum = 0, coordinate = 0;
		for (int i = startIndex; i <= endIndex; i++) {
			coordinate = (*coordinates[i])[d];
			sum += coordinate;
			sqdSum += coordinate * coordinate;
		}
		variance = n * sqdSum - sum * sum; // no need to divide (n * (n - 1)) owing to same divisor during comparison
		if (variance > maxVariance) {
			maxVariance = variance;
			maxD = d;
		}
		else if (variance == maxVariance && d != currentDimension) {
			maxD = d; // avoid same dimension if same wih last dimension
		}
	}
	return maxD;
}

//// variance[i] = variance[i - 1] + x^2 - n * mean[i]^2 + (n - 1) * mean[i - 1]^2
//int KDTree::FindMaxVarianceDimension(const int currentDimension, KDTreeCoordinates* coordinates[], const int startIndex, const int endIndex) {
//	int maxD = 0;
//	float maxVariance = -1;
//	float coordinate, variance, currentMean, lastMean;
//
//	for (int d = 0; d < k; d++) {
//		// avoid the denominator (n - 1) being 0, 
//		float n = 1, nMinus1 = 1;
//		int i = startIndex;
//		coordinate = (*coordinates[i])[d];
//		currentMean = coordinate;
//		variance = 0;
//		lastMean = currentMean;
//		i++, n++;
//		float temp;
//		for (; i <= endIndex; i++, n++, nMinus1++) {
//			coordinate = (*coordinates[i])[d];
//			currentMean = (nMinus1 * lastMean + coordinate) / n;
//			temp = coordinate * coordinate;
//			temp -= n * currentMean * currentMean;
//			variance *= nMinus1 - 1;
//			variance += temp;
//			variance /= nMinus1;
//			variance += lastMean * lastMean;
//			lastMean = currentMean;
//		}
//		if (variance > maxVariance) {
//			maxVariance = variance;
//			maxD = d;
//		}
//		else if (variance == maxVariance && d != currentDimension) {
//			maxD = d; // avoid same dimension if same wih last dimension
//		}
//	}
//	return maxD;
//}

//int KDTree::FindMaxVarianceDimension(const int currentDimension, KDTreeCoordinates* coordinates[], const int startIndex, const int endIndex) {
//	int maxD = 0;
//	float maxVariance = -1;
//	float coordinate, variance, mean;
//	const float n = static_cast<float>(endIndex - startIndex + 1);
//
//	for (int d = 0; d < k; d++) {
//		variance = 0, mean = 0, coordinate = 0;
//		for (int i = startIndex; i <= endIndex; i++) {
//			coordinate = (*coordinates[i])[d];
//			mean += coordinate;
//		}
//		mean /= n;
//		float temp;
//		for (int i = startIndex; i <= endIndex; i++) {
//			coordinate = (*coordinates[i])[d];
//			temp = coordinate - mean;
//			variance += temp * temp;
//		}
//		//variance /= (n - 1);
//		if (variance > maxVariance) {
//			maxVariance = variance;
//			maxD = d;
//		}
//		else if (variance == maxVariance && d != currentDimension) {
//			maxD = d; // avoid same dimension if same wih last dimension
//		}
//	}
//	return maxD;
//}

// find median
KDTreeCoordinates*& KDTree::GetPivotByMedian3(const int axis, KDTreeCoordinates*& start, KDTreeCoordinates*& middle, KDTreeCoordinates*& end) { //return pivot
	// first find max and switch to the end
	if ((*start)[axis] > (*end)[axis] && (*start)[axis] >= (*middle)[axis])
		Swap(start, end);
	else if ((*middle)[axis] > (*end)[axis] && (*middle)[axis] >= (*start)[axis])
		Swap(middle, end);
	//second find min
	if ((*start)[axis] > (*middle)[axis])
		Swap(start, middle);
	//return median as pivot
	return middle;
}

void KDTree::Swap(KDTreeCoordinates*& left, KDTreeCoordinates*& right) {
	if (left == right)
		return;
	KDTreeCoordinates* const temp = left;
	left = right;
	right = temp;
}

int KDTree::GetLeftSplitIndex(int axis, KDTreeCoordinates* coordinates[], int startIndex, int endIndex) {
	if (startIndex >= endIndex)
		return endIndex;

	const int medianIndex = (startIndex + endIndex) >> 1;

	KDTreeCoordinates* pivot;
	int lowPivotIndex, highPivotIndex, current;

	while (startIndex < endIndex) {
		// like 3 way quick sort, but no need to do recursion
		pivot = GetPivotByMedian3(axis, coordinates[startIndex], coordinates[medianIndex], coordinates[endIndex]); // reorder first, middle and last number and get median as pivot
		lowPivotIndex = startIndex;
		highPivotIndex = endIndex;
		current = lowPivotIndex;

		while (current <= highPivotIndex) {
			if ((*coordinates[current])[axis] < (*pivot)[axis])
				Swap(coordinates[current++], coordinates[lowPivotIndex++]);
			else if ((*coordinates[current])[axis] > (*pivot)[axis])
				Swap(coordinates[current], coordinates[highPivotIndex--]);
			else
				current++;
		}

		if (lowPivotIndex > medianIndex)
			endIndex = lowPivotIndex - 1;
		else if (highPivotIndex < medianIndex)
			startIndex = highPivotIndex + 1;
		//all "lesser" points are on left side and "greater" points are on right side
		else
			return lowPivotIndex; // "equal" points are on the right side.
	}
	return medianIndex;
}

int KDTree::GetRightSplitIndex(int axis, KDTreeCoordinates* coordinates[], int startIndex, int endIndex) {
	if (startIndex >= endIndex)
		return endIndex;

	const int medianIndex = (startIndex + endIndex) >> 1;

	KDTreeCoordinates* pivot;
	int lowPivotIndex, highPivotIndex, current;

	while (startIndex < endIndex) {
		// like 3 way quick sort, but no need to do recursion
		pivot = GetPivotByMedian3(axis, coordinates[startIndex], coordinates[medianIndex], coordinates[endIndex]); // reorder first, middle and last number and get median as pivot
		lowPivotIndex = startIndex;
		highPivotIndex = endIndex;
		current = highPivotIndex;

		while (current >= lowPivotIndex) {
			if ((*coordinates[current])[axis] > (*pivot)[axis])
				Swap(coordinates[current--], coordinates[highPivotIndex--]);
			else if ((*coordinates[current])[axis] < (*pivot)[axis])
				Swap(coordinates[current], coordinates[lowPivotIndex++]);
			else
				current--;
		}

		if (lowPivotIndex > medianIndex)
			endIndex = lowPivotIndex - 1;
		else if (highPivotIndex < medianIndex)
			startIndex = highPivotIndex + 1;
		//all "lesser" points are on left side and "greater" points are on right side
		else
			return highPivotIndex; // "equal" points are on the left side.
	}
	return medianIndex;
}

int KDTree::GetMedianSplitIndex(const int axis, KDTreeCoordinates* coordinates[], int startIndex, int endIndex) {
	if (startIndex >= endIndex)
		return endIndex;

	const int medianIndex = (startIndex + endIndex) >> 1;

	KDTreeCoordinates* pivot;
	int leftIndex, rightIndex;

	while (startIndex < endIndex) {
		// like quick sort, but no need to do recursion
		pivot = GetPivotByMedian3(axis, coordinates[startIndex], coordinates[(startIndex + endIndex) >> 1], coordinates[endIndex]); // reorder first, middle and last number and get median as pivot
		leftIndex = startIndex;
		rightIndex = endIndex;

		while (true) {
			while (leftIndex < endIndex && (*coordinates[++leftIndex])[axis] <= (*pivot)[axis]);
			while (rightIndex > startIndex && (*coordinates[--rightIndex])[axis] >= (*pivot)[axis]);

			if (leftIndex < rightIndex)
				Swap(coordinates[leftIndex], coordinates[rightIndex]);
			else
				break;
		}

		if (rightIndex >= medianIndex)
			endIndex = rightIndex;
		else if (leftIndex <= medianIndex)
			startIndex = leftIndex;
		//all "lesser" points are on left side and "greater" points are on right side
		// "equal" points may be on left or right side
		else
			return medianIndex;
	}
	return medianIndex;
}

//build tree
KDTreeNode* KDTree::Build(KDTreeNode* const parent, const int currentDimension, KDTreeCoordinates* coordinates[], const int startIndex, const int endIndex) {
	if (startIndex == endIndex) {
		KDTreeNode* node = new KDTreeNode(parent, 0, coordinates[startIndex]);
		node->SetLeftChild(new KDTreeNode(node)); // empty node
		node->SetRightChild(new KDTreeNode(node)); // empty node
		return node;
	}

	int maxD = FindMaxVarianceDimension(currentDimension, coordinates, startIndex, endIndex);
	//get the splitting planes
	//const int splitIndex = GetMedianSplitIndex(maxD, coordinates, startIndex, endIndex); // left <= parent, right >= parent
	const int splitIndex = GetRightSplitIndex(maxD, coordinates, startIndex, endIndex); // left <= parent, right > parent
	KDTreeCoordinates* splitCoordinate = coordinates[splitIndex];

	//create new node
	KDTreeNode* node = new KDTreeNode(parent, maxD, splitCoordinate);

	//create new left child node
	KDTreeNode* leftChildNode = startIndex < splitIndex
		? Build(node, maxD, coordinates, startIndex, splitIndex - 1)
		: new KDTreeNode(node); // empty node
	node->SetLeftChild(leftChildNode);

	//create new right child node
	KDTreeNode* rightChildNode = endIndex > splitIndex
		? Build(node, maxD, coordinates, splitIndex + 1, endIndex)
		: new KDTreeNode(node); // empty node
	node->SetRightChild(rightChildNode);

	return node;
}

void KDTree::Build(KDTreeCoordinates* coordinates[], const int length) {
	if (length < 1)
		return;
	Clear();
	int startIndex = 0;
	int endIndex = length - 1;

	root = Build(NULL, -1, coordinates, startIndex, endIndex);
}

float KDTree::CalculateSquaredEuclideanDistance(KDTreeCoordinates& coordinate1, KDTreeCoordinates& coordinate2) {
	float sqdEcldDist = 0;
	float temp;
	for (int d = 0; d < k; d++) {
		temp = coordinate1[d] - coordinate2[d];
		sqdEcldDist += temp * temp;
	}
	return sqdEcldDist;
}

float KDTree::CalculateManhattanDistance(KDTreeCoordinates& coordinate1, KDTreeCoordinates& coordinate2) {
	float mhtDist = 0;
	float tempF;
	for (int d = 0; d < k; d++) {
		tempF = coordinate1[d] - coordinate2[d];
		mhtDist += *(float*)&(_FLOAT_WORD(tempF) &= _ABSOLUTE_FLOAT_WORD); //absolute, set the sign bit as 0
	}
	return mhtDist;
}

float KDTree::CalculateDomainDistance(KDTreeNode& pivot, KDTreeCoordinates& target) {
	int axis = pivot.GetAxis();
	float tempF = (*pivot.GetCoordinates())[axis] - target[axis];
	return *(float*)&(_FLOAT_WORD(tempF) &= _ABSOLUTE_FLOAT_WORD); //absolute, set the sign bit as 0
}

void KDTree::FindNearestNeighbour(KDTreeCoordinates* const target, KDTreeNode* const currentNode, KDTreeCoordinatesContainerCollection*& nearestNeighbourCollection, float& nearestDistance) {
	if (currentNode->IsEmpty())
		return;
	//go to leaf and get the current nearest neighbour first
	int splitAxis = currentNode->GetAxis();
	bool currentSideIsLeft;
	if (currentSideIsLeft = (*target)[splitAxis] <= (*currentNode->GetCoordinates())[splitAxis])
		FindNearestNeighbour(target, currentNode->GetLeftChild(), nearestNeighbourCollection, nearestDistance); //go to left side
	else
		FindNearestNeighbour(target, currentNode->GetRightChild(), nearestNeighbourCollection, nearestDistance); // go to right side

	KDTreeNode* pivot = currentNode;
	float currentDistance = CalculateSquaredEuclideanDistance(*(pivot->GetCoordinates()), *target);

	//check pivot is nearest neighbour
	if (currentDistance <= nearestDistance) {
		//clear old neighbours if it is nearer neighbour
		if (currentDistance < nearestDistance)
			nearestNeighbourCollection->Clear();
		KDTreeCoordinates* currentNeighbour = pivot->GetCoordinates();
		nearestNeighbourCollection->InsertCoordinatesContainer(currentNeighbour);
		nearestDistance = currentDistance;
	}
	//check other side may has nearest neighbour
	KDTreeNode* otherSideNode = currentSideIsLeft
		? pivot->GetRightChild()
		: pivot->GetLeftChild();
	if (!otherSideNode->IsEmpty()) {
		float currentDomainDistance = CalculateDomainDistance(*pivot, *target);
		if (currentDomainDistance <= nearestDistance)
			FindNearestNeighbour(target, otherSideNode, nearestNeighbourCollection, nearestDistance);
	}
}

float KDTree::FindNearestNeighbour(KDTreeCoordinates* const target, KDTreeCoordinatesContainerCollection*& nearestNeighbourCollection) {
	if (root->IsEmpty())
		return NULL;
	nearestNeighbourCollection = new KDTreeCoordinatesContainerCollection();
	float nearestDistance;
	_FLOAT_WORD(nearestDistance) = _POS_INFINITY_FLOAT_WORD; // set to positive infinity
	KDTreeNode* currentNode = root;
	FindNearestNeighbour(target, currentNode, nearestNeighbourCollection, nearestDistance);

	return sqrtf(nearestDistance);
}

void KDTree::FindNearestNeighbour_BBF(KDTreeCoordinates* const target, const unsigned int maxCheckTimes, KDTreeCoordinatesContainerCollection*& nearestNeighbourCollection, float& nearestDistance) {
	if (root->IsEmpty())
		return;

	MinHeap priorityQueue;
	priorityQueue.Push(0, *root);
	float currentDistance;
	KDTreeNode* pivot;
	KDTreeNode* child;
	KDTreeNode* sibling;
	bool childSideIsLeft;
	int splitAxis;
	HeapNode* pQueueNode;
	unsigned int times = 0;
	while ((!maxCheckTimes || times++ < maxCheckTimes) && !priorityQueue.IsEmpty()) { // times will never change if maxCheckTimes is 0
		pQueueNode = priorityQueue.Pop();
		pivot = pQueueNode->GetData();
		delete pQueueNode;
		//go to leaf
		splitAxis = pivot->GetAxis();
		// calculate distance
		currentDistance = CalculateSquaredEuclideanDistance(*(pivot->GetCoordinates()), *target);
		//check current is nearest neighbour
		if (currentDistance <= nearestDistance) {
			//clear old neighbours if it is nearer neighbour
			if (currentDistance < nearestDistance)
				nearestNeighbourCollection->Clear();
			KDTreeCoordinates* currentNeighbour = pivot->GetCoordinates();
			nearestNeighbourCollection->InsertCoordinatesContainer(currentNeighbour);
			nearestDistance = currentDistance;
		}
		//get child and sibling
		child = (childSideIsLeft = (*target)[splitAxis] <= (*pivot->GetCoordinates())[splitAxis])
			? pivot->GetLeftChild()
			: pivot->GetRightChild();
		sibling = childSideIsLeft
			? pivot->GetRightChild()
			: pivot->GetLeftChild();
		//go to child and do it again
		while (!child->IsEmpty()) {
			currentDistance = CalculateSquaredEuclideanDistance(*(child->GetCoordinates()), *target);
			//check current is nearest neighbour
			if (currentDistance <= nearestDistance) {
				//clear old neighbours if it is nearer neighbour
				if (currentDistance < nearestDistance)
					nearestNeighbourCollection->Clear();
				KDTreeCoordinates* currentNeighbour = child->GetCoordinates();
				nearestNeighbourCollection->InsertCoordinatesContainer(currentNeighbour);
				nearestDistance = currentDistance;
			}
			//check sibling may has nearest neighbour
			if (!sibling->IsEmpty()) {
				float currentDomainDistance = CalculateDomainDistance(*pivot, *target);
				if (currentDomainDistance <= nearestDistance)
					priorityQueue.Push(currentDomainDistance, *sibling); // push into priority queue, order by domain Distance
			}
			//update pivot, split dimension,  child and sibling
			pivot = child;
			splitAxis = pivot->GetAxis();
			child = (childSideIsLeft = (*target)[splitAxis] <= (*pivot->GetCoordinates())[splitAxis])
				? pivot->GetLeftChild()
				: pivot->GetRightChild();
			sibling = childSideIsLeft
				? pivot->GetRightChild()
				: pivot->GetLeftChild();
		}
	}
}

float KDTree::FindNearestNeighbour_BBF(KDTreeCoordinates* const target, KDTreeCoordinatesContainerCollection*& nearestNeighbourCollection) {
	return FindNearestNeighbour_BBF(target, DEFAULT_BBF_MAX_SEARCHTIMES, nearestNeighbourCollection);
}

float KDTree::FindNearestNeighbour_BBF(KDTreeCoordinates* const target, const unsigned int maxCheckTimes, KDTreeCoordinatesContainerCollection*& nearestNeighbourCollection) {
	if (root->IsEmpty())
		return NULL;
	nearestNeighbourCollection = new KDTreeCoordinatesContainerCollection();
	float nearestDistance;
	_FLOAT_WORD(nearestDistance) = _POS_INFINITY_FLOAT_WORD; // set to positive infinity
	FindNearestNeighbour_BBF(target, maxCheckTimes, nearestNeighbourCollection, nearestDistance);

	return sqrtf(nearestDistance);
}
