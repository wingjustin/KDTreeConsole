#include "KDTreeCoordinates.h"

using namespace Tree;

KDTreeCoordinates::KDTreeCoordinates(const int dimension) {
	KDTreeCoordinates::dimension = dimension;
	KDTreeCoordinates::coordinates = new float[dimension];
	//initial coordinate
	for (int i = 0; i < dimension; i++)
		coordinates[i] = 0;
}

KDTreeCoordinates::~KDTreeCoordinates() {
	delete[] coordinates;
}

int KDTreeCoordinates::GetDimension() {
	return dimension;
}

float& KDTreeCoordinates::operator[](const int axis) {
	return coordinates[axis];
}

KDTreeCoordinates KDTreeCoordinates::operator+(KDTreeCoordinates& otherCoordinates) {
	int k = dimension < otherCoordinates.dimension ? dimension : otherCoordinates.dimension;
	KDTreeCoordinates newCoord(dimension);
	for (int d = 0; d < k; d++) {
		newCoord[d] = coordinates[d] + otherCoordinates[d];
	}
	return newCoord;
}

float KDTreeCoordinates::operator-(KDTreeCoordinates& otherCoordinates) {
	//Euclidean Distance
	int k = dimension < otherCoordinates.dimension ? dimension : otherCoordinates.dimension;
	float sqdEcldDist = 0;
	float temp;
	for (int d = 0; d < k; d++) {
		temp = coordinates[d] - otherCoordinates[d];
		sqdEcldDist += temp * temp;
	}
	return sqrtf(sqdEcldDist);
}

bool KDTreeCoordinates::operator==(KDTreeCoordinates& otherCoordinates) {
	if (dimension != otherCoordinates.dimension)
		return false;
	for (int d = 0; d < dimension; d++) {
		if (coordinates[d] != otherCoordinates[d])
			return false;
	}
	return true;
}

bool KDTreeCoordinates::operator!=(KDTreeCoordinates& otherCoordinates) {
	return !operator==(otherCoordinates);
}
