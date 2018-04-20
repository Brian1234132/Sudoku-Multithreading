#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

//GLOBAL VARIABLES
int array[9][9]; // input sudoku values in 9x9 array format
bool validity = true; // is sudoku puzzle valid or not?
/////////////////////

void ReadInput(int array[9][9]) // Takes 81 ints in a file and reads into array. 
{
	ifstream ifs;

	string name;
	cout << "Input be 81 integers.\nPlease Input Test File Name: (.txt file without extension)" << endl;
	cin >> name;
	name = name + ".txt";


	ifs.open(name.c_str());
	if (ifs.fail ()) {
		cout << "input file has failed to open, please check location for file." << endl;
		exit(1);
	}
	for (int i=0; i < 9; i++) {
		for (int j=0; j < 9; j++) {
			ifs >> array[i][j]; // read input into array
		}
	}
}

void PrintInput(int array[9][9]) // print puzzle in readable format
{
	cout << "\nInput Sudoku Puzzle: " << endl;
	for (int i=0; i<9; i++)
	{
		for (int j=0; j<9; j++)
		{
			cout << array[i][j] << " ";
		}
		cout << endl;
	}
}

DWORD WINAPI ColumnCheck(void *data) {
	int sortedarray[9] = {1,2,3,4,5,6,7,8,9}; // array 1-9 sorted for compare purposes
	int TEMP[9]; // to hold actual input data and sort before comparing

	for (int x=0; x < 9; x++) { 
		for (int column=0; column < 9; column++) {
			TEMP[column] = array[x][column]; // load values into temp
		}

		sort(TEMP, TEMP+9); // SORT TEMP ARRAY

		/*cout << "\nSorted TEMP Array: ";
		for (int y=0; y < 9; y++) {
			cout << TEMP[y] << " ";
		}
		cout << endl;
		*/
		
		for (int column=0; column < 9; column++) {
			if (TEMP[column] == sortedarray[column]) { // detect MATCH 
				// do nothing
			}
			else {
				validity = FALSE; // mismatch, in comparing 1-9 puzzle not valid
			}
		

		}
	}
	

	cout << "Column Check successfully completed...exiting thread\n" << endl;
	return 0;
	
}

DWORD WINAPI RowCheck(void *data) {
	int sortedarray[9] = {1,2,3,4,5,6,7,8,9}; // array 1-9 sorted for compare purposes
	int TEMP[9]; // to hold actual input data and sort before comparing

	for (int x=0; x < 9; x++) { 
		for (int row=0; row < 9; row++) {
			TEMP[row] = array[row][x]; // load values into temp
		}

		sort(TEMP, TEMP+9); // SORT TEMP ARRAY

		for (int row=0; row < 9; row++) {
			if (TEMP[row] == sortedarray[row]) { // detect MATCH 
				// do nothing
			}
			else {
				validity = FALSE; // mismatch, in comparing 1-9 puzzle not valid
			}
		

		}
	}

	cout << "Row Check successfully completed...exiting thread\n" << endl;
	return 0;
}

DWORD WINAPI SubgridCheck(void* data) {
  // Do stuff.  This will be the first function called on the new thread.
  // When this function returns, the thread goes away.  See MSDN for more details.
	cout << "task1 says: hello" << endl;
	Sleep(10000);
	cout << "task 1 says: hello again" << endl;

	cout << "Subgrid Check successfully completed...exiting thread\n" << endl;
	return 0;
}

typedef struct 
 {
   int row_start;
   int row_end;
   int column_start;
   int column_end;
 } Parameters; // structure used to pass 3x3 subgrid data



int main() {
	ReadInput(array);
	PrintInput(array);

	DWORD WorkerCheck[2] = {0,0};

 	HANDLE thread1 = CreateThread(NULL, 0, ColumnCheck, NULL, 0, NULL);
  	HANDLE thread2 = CreateThread(NULL, 0, RowCheck, NULL, 0, NULL);

  	Parameters *data = (Parameters *) malloc (sizeof(Parameters)); 
	data->row_start = 0; // set parameters to check 3x3 subgrid
	data->row_end = 2;
	data->column_start = 0;
	data->column_end = 2;

  /*HANDLE thread3 = CreateThread(NULL, 0, SubgridCheck, data, 0, NULL);

  if (thread3) 
  {
  	cout << "Subgrid Check Thread Succesfully Created" << endl;
  }
  else
  {
  	cout << "Subgrid Check Thread ERROR" << endl;
  }*/


  
 	WaitForSingleObject(thread1, INFINITE); // wait for each thread to finish
  	WaitForSingleObject(thread2, INFINITE);

	HANDLE *threads = new HANDLE[2]; // handler for threads
	threads[0] = thread1;
	threads[1] = thread2;

/*
  GetExitCodeThread(threads[0], &WorkerCheck[0]); // testing wait on thread functions
  GetExitCodeThread(threads[1], &WorkerCheck[1]);

  cout << "\nWorkerCheck 1: " << WorkerCheck[0] << endl;
  cout << "WorkerCheck 2: " << WorkerCheck[1] << endl;
*/

 	cout << "ALL THREADS EXITED" << endl;

	if (validity)
	{
		cout << "\nSudoku Puzzle is VALID!" << endl;
	}
	else 
	{
		cout << "\nSudoku Puzzle is INVALID!" << endl;
	}

	return 0;
}