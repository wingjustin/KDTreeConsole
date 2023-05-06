#pragma once

#include <stdlib.h>
#include "KDTreeCoordinates.h"

namespace Tree {
	struct KDTreeCoordinatesContainer {
		KDTreeCoordinates* coordinates = NULL;

		KDTreeCoordinatesContainer* prev = NULL;
		KDTreeCoordinatesContainer* next = NULL;
	};
	class KDTreeCoordinatesContainerCollection {
		int length;
		KDTreeCoordinatesContainer* firstCoordinatesContainer;
		KDTreeCoordinatesContainer* lastCoordinatesContainer;
	public:
		KDTreeCoordinatesContainerCollection();
		~KDTreeCoordinatesContainerCollection();

		bool IsEmpty();
		int GetLength();

		KDTreeCoordinatesContainer* GetFirstCoordinatesContainer();
		KDTreeCoordinatesContainer* GetLastCoordinatesContainer();

		KDTreeCoordinatesContainer* InsertCoordinatesContainer(KDTreeCoordinates* newCoordinates);

		void Clear();
	};
}
