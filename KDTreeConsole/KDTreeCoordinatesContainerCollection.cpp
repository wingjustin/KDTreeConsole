#include "KDTreeCoordinatesContainerCollection.h"

using namespace Tree;

KDTreeCoordinatesContainerCollection::KDTreeCoordinatesContainerCollection() {
	length = 0;
	firstCoordinatesContainer = NULL;
	lastCoordinatesContainer = NULL;
}

KDTreeCoordinatesContainerCollection::~KDTreeCoordinatesContainerCollection() {
	KDTreeCoordinatesContainer* current = firstCoordinatesContainer;
	if (current) {
		KDTreeCoordinatesContainer* temp = current;
		do {
			temp = current;
			current = temp->next;
			temp->coordinates = NULL;
			temp->prev = NULL;
			temp->next = NULL;
			delete temp;
		} while (current);
		temp = NULL;
	}
	firstCoordinatesContainer = NULL;
	lastCoordinatesContainer= NULL;
}

bool KDTreeCoordinatesContainerCollection::IsEmpty() {
	return length == 0;
}

int KDTreeCoordinatesContainerCollection::GetLength() {
	return length;
}

KDTreeCoordinatesContainer* KDTreeCoordinatesContainerCollection::GetFirstCoordinatesContainer() {
	return firstCoordinatesContainer;
}

KDTreeCoordinatesContainer* KDTreeCoordinatesContainerCollection::GetLastCoordinatesContainer() {
	return lastCoordinatesContainer;
}

KDTreeCoordinatesContainer* KDTreeCoordinatesContainerCollection::InsertCoordinatesContainer(KDTreeCoordinates* const newCoordinates) {
	KDTreeCoordinatesContainer* newCoordinatesContainer = new KDTreeCoordinatesContainer();
	newCoordinatesContainer->coordinates = newCoordinates;
	if (lastCoordinatesContainer) {
		lastCoordinatesContainer->next = newCoordinatesContainer;
		newCoordinatesContainer->prev = lastCoordinatesContainer;
		lastCoordinatesContainer = newCoordinatesContainer;
		lastCoordinatesContainer->next = NULL;
		length++;

		return lastCoordinatesContainer;
	}
	else {
		lastCoordinatesContainer = firstCoordinatesContainer = newCoordinatesContainer;
		firstCoordinatesContainer->prev = NULL;
		lastCoordinatesContainer->next = NULL;
		length++;

		return lastCoordinatesContainer;
	}
}

void KDTreeCoordinatesContainerCollection::Clear() {
	KDTreeCoordinatesContainer* current = firstCoordinatesContainer;
	if (current) {
		KDTreeCoordinatesContainer* temp = current;
		do {
			temp = current;
			current = temp->next;
			temp->coordinates = NULL;
			temp->prev = NULL;
			temp->next = NULL;
			delete temp;
		} while (current);
		temp = NULL;
	}
	firstCoordinatesContainer = NULL;
	lastCoordinatesContainer = NULL;

	length = 0;
}
