// KDTreeConsole.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <random>
#include <string>
#include<conio.h>
#include "KDTree.h"

#define DEBUG_MODE false
#define BBF_SEARCH true
#define BBF_MAX_SEARCHTIMES (unsigned int) 0

#define K_INT 2

#define INSERT_COUNT 80

#define DEBUG_ROUND 42

using namespace std;
using namespace Tree;

int main()
{
	const int insertCount = INSERT_COUNT;

	const int round = DEBUG_ROUND;

	bool showEachHealthStatus = true;

	bool showInsertDetails = true;

	random_device dev;
	mt19937 rng(dev());
	uniform_real_distribution<float> insertRandom(-99.99f, 99.99f);

	KDTree tree(K_INT);

	cout << "K = " << to_string(tree.Get_K()) << endl;
	cout << "=======================================================" << endl;

	bool totHealth = true;

	char operKey = 'R';

	for (int r = 0; !DEBUG_MODE || r < round; r++) {
		if (operKey == 'E' || operKey == 'e')
			break;

		cout << "ROUND : " << r + 1 << "\n";

		KDTreeCoordinates* coordinatesArr[insertCount];
		KDTreeCoordinatesContainerCollection* nearestNeighbourCollection;
		bool isHealth = false;

		//insert into tree
		if (insertCount > 0) {
			if (showInsertDetails) cout << "Inserted : { ";

			coordinatesArr[0] = new KDTreeCoordinates(tree.Get_K());

			if (showInsertDetails) cout << "(";
			(*coordinatesArr[0])[0] = insertRandom(rng);
			if (showInsertDetails) cout << to_string((*coordinatesArr[0])[0]);
			for (int k = 1; k < tree.Get_K(); k++) {
				(*coordinatesArr[0])[k] = insertRandom(rng);
				if (showInsertDetails) cout << ", " << to_string((*coordinatesArr[0])[k]);
			}
			if (showInsertDetails) cout << ")";

			for (int i = 1; i < insertCount; i++) {
				coordinatesArr[i] = new KDTreeCoordinates(tree.Get_K());

				if (showInsertDetails) cout << ", (";
				(*coordinatesArr[i])[0] = insertRandom(rng);
				if (showInsertDetails) cout << to_string((*coordinatesArr[i])[0]);
				for (int k = 1; k < tree.Get_K(); k++) {
					(*coordinatesArr[i])[k] = insertRandom(rng);
					if (showInsertDetails) cout << ", " << to_string((*coordinatesArr[i])[k]);
				}
				if (showInsertDetails) cout << ")";
			}
			if (showInsertDetails) cout << " }";
		}
		if (showInsertDetails) cout << " \n";

		isHealth = true;
		tree.Build(coordinatesArr, insertCount);

		for (int i = 0; i < insertCount; i++) {
			KDTreeCoordinates* target = new KDTreeCoordinates(tree.Get_K());
			for (int k = 0; k < tree.Get_K(); k++) {
				(*target)[k] = (*coordinatesArr[i])[k];
			}
			float distance = tree.FindNearestNeighbour_BBF(target, 1, nearestNeighbourCollection);

			if (nearestNeighbourCollection->GetLength() > 0) {
				KDTreeCoordinatesContainer* currentCoordinatesContainer = nearestNeighbourCollection->GetFirstCoordinatesContainer();
				KDTreeCoordinates* currentCoordinates = currentCoordinatesContainer->coordinates;
				for (int k = 0; k < tree.Get_K(); k++) {
					isHealth = isHealth && (*currentCoordinates)[k] == (*target)[k];
				}
				while (currentCoordinatesContainer = currentCoordinatesContainer->next) {
					currentCoordinates = currentCoordinatesContainer->coordinates;
					for (int k = 0; k < tree.Get_K(); k++) {
						isHealth = isHealth && (*currentCoordinates)[k] == (*target)[k];
					}
				}
			}
			else
				isHealth = false;

			isHealth = isHealth && distance == 0.0f;

			delete target;
			delete nearestNeighbourCollection;
			nearestNeighbourCollection = NULL;
		}

		//for (int i = 0; i < insertCount; i++) {
		//	delete coordinatesArr[i];
		//}

		totHealth = totHealth && isHealth;

		//Check bugs
		if (showEachHealthStatus) cout << "Inserted Tree Health : " << (isHealth ? "OK, no BUG!" : "Warning! Error occured!") << "\n";
		if (showEachHealthStatus) cout << " \n";

		if (!DEBUG_MODE) {
			operKey = 'A';
			while (operKey != 'R' && operKey != 'r' && operKey != 'E' && operKey != 'e') {

				cout << "Nearest Neighbour Searching" << (BBF_SEARCH ? " (BBF)" : " ") << "\n";
				cout << "----------------------------------------------------------------------------------------------------" << "\n";

				KDTreeCoordinates* target = new KDTreeCoordinates(tree.Get_K());
				for (int k = 0; k < tree.Get_K(); k++) {
					(*target)[k] = 0.0f;
				}
				int k_i = 0;
				cout << "Please input Coordinates ( Pattern = x y z ... kn ) : " << "\n";
				string inputWord = "";
				char temp;
				while (cin.get(temp) && temp != '\n' && k_i < tree.Get_K()) {
					if ((temp == ' ') && (static_cast<int>(inputWord.length()) != 0)) {
						float coor = stof(inputWord);
						(*target)[k_i++] = coor;
						inputWord = "";
					}
					else if (temp >= 0x30 && temp <= 0x39 || temp == 0x2E || temp == 0x2D) // (0-9 .-)
						inputWord += temp;
				}
				if (static_cast<int>(inputWord.length()) != 0 && k_i < tree.Get_K()) {
					float coor = stof(inputWord);
					(*target)[k_i] = coor;
					inputWord = "";
				}

				cout << "-----------------------------------------------------------------------------" << endl;
				cout << "Target = (";
				cout << to_string((*target)[0]);
				for (int k = 1; k < tree.Get_K(); k++) {
					cout << ", " << to_string((*target)[k]);
				}
				cout << ")";

				cout << endl;
				cout << "-----------------------------------------------------------------------------" << endl;
				// show Nearest Neighbour
				float distance = BBF_SEARCH
					? tree.FindNearestNeighbour_BBF(target, BBF_MAX_SEARCHTIMES, nearestNeighbourCollection)
					: tree.FindNearestNeighbour(target, nearestNeighbourCollection);


				cout << "Distance = " << to_string(distance) << "\n";
				cout << "Count = " << to_string(nearestNeighbourCollection->GetLength()) << "\n";

				if (nearestNeighbourCollection->GetLength() > 0) {
					cout << "{ ";
					KDTreeCoordinatesContainer* currentCoordinatesContainer = nearestNeighbourCollection->GetFirstCoordinatesContainer();
					KDTreeCoordinates* currentCoordinates = currentCoordinatesContainer->coordinates;
					cout << "(";
					cout << to_string((*currentCoordinates)[0]);
					for (int k = 1; k < tree.Get_K(); k++) {
						cout << ", " << to_string((*currentCoordinates)[k]);
					}
					cout << ")";
					while (currentCoordinatesContainer = currentCoordinatesContainer->next) {
						currentCoordinates = currentCoordinatesContainer->coordinates;
						cout << "(";
						cout << to_string((*currentCoordinates)[0]);
						for (int k = 1; k < tree.Get_K(); k++) {
							cout << ", " << to_string((*currentCoordinates)[k]);
						}
						cout << ")";
					}
					cout << " }";
				}

				delete target;
				delete nearestNeighbourCollection;

				cout << endl << endl;

				//check continue
				cout << "Press Any Key to Search other target, \"R\" to refresh; \"E\" for Exit : ";
				operKey = _getch();
				cout << "\n=========================================================================" << endl;
			}
		}
	}

	tree.Clear();

	cout << " \n";
	cout << "All Tree Health : " << (totHealth ? "OK, no BUG!" : "Warning! Error occured!") << "\n";
	cout << " \n";

	system("pause");
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
