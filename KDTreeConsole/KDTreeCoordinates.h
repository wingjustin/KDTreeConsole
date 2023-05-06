#pragma once

#include <math.h>

namespace Tree {
	class KDTreeCoordinates {
		int dimension;
		float* coordinates;
	public:
		KDTreeCoordinates(int dimension);
		~KDTreeCoordinates();

		int GetDimension();

		float& operator[](int axis);

		KDTreeCoordinates operator+(KDTreeCoordinates& otherCoordinates);
		float operator-(KDTreeCoordinates& otherCoordinates); // calculate Euclidean Distance

		bool operator==(KDTreeCoordinates& otherCoordinates);
		bool operator!=(KDTreeCoordinates& otherCoordinates);
	};
}
