#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

//GLOBAL VARIABLES
int array[9][9]; // input sudoku values in 9x9 array format
int validity[11] = {1,1,1,1,1,1,1,1,1,1,1}; // is sudoku puzzle valid or not? (thread checking)
bool parentvalid = true; // check thread then determine overall validity
/////////////////////

typedef struct Parameters
 {
   int row_start = 0;
   int row_end = 0;
   int column_start = 0;
   int column_end = 0;
   int subgridnum = 0;
 } Parameters; // structure used to pass 3x3 subgrid data



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
	cout << "\nColumn Check Thread Successfully Created!" << endl;
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
				validity[0] = 0; // mismatch, in comparing 1-9 puzzle not valid
			}
		

		}
	}
	

	cout << "\nColumn Check successfully completed...exiting thread" << endl;
	return 0;
	
}

DWORD WINAPI RowCheck(void *data) {
	cout << "\nRow Check Thread Successfully Created!" << endl;
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
				validity[1] = 0; // mismatch, in comparing 1-9 puzzle not valid
			}
		

		}
	}

	cout << "\nRow Check successfully completed...exiting thread" << endl;
	return 0;
}

DWORD WINAPI SubgridCheck(void *data) {
	int sortedarray[9] = {1,2,3,4,5,6,7,8,9}; // array 1-9 sorted for compare purposes
	int TEMP[9]; // to hold actual input data and sort before comoparing
	int count = 0; // used for TEMP[k] to load values
	Parameters* TEMPdata = (Parameters*)data;
	int TEMProw_start = TEMPdata->row_start;
	int TEMProw_end = TEMPdata->row_end;
	int TEMPcolumn_start = TEMPdata->column_start;
	int TEMPcolumn_end = TEMPdata->column_end;
	int TEMPsubgridnum = TEMPdata->subgridnum;

	/*cout << "TEMProw_start: " << TEMProw_start;
	cout << "TEMProw_end: " << TEMProw_end;
	cout << "TEMPcolumn_start: " << TEMPcolumn_start;
	cout << "TEMPcolumn_end: " << TEMPcolumn_end;*/

	TEMProw_end++;
	TEMPcolumn_end++;

	for (int k = TEMProw_start; k < TEMProw_end; k++) { // input values into temp array
		for (int j = TEMPcolumn_start; j < TEMPcolumn_end; j++) {
			TEMP[count] = array[k][j];
			count++;
			//cout << "Count: " << count << endl;
		}
	}

		/*do {
			do {
				cout << "Count: " << count << endl;
				TEMP[count] = array[TEMProw_start][TEMPcolumn_start];
				count++;
				TEMPcolumn_start++;
			}while(TEMPcolumn_start < TEMPcolumn_end);

			cout << "Temprow++times: " << TEMProw_start << endl;
			TEMProw_start++;

		}while(TEMProw_start < TEMProw_end);*/

		/*cout << "\nUnSorted TEMP Array: ";
		for (int y=0; y < 9; y++) {
			cout << TEMP[y] << " ";
		}
		cout << endl;*/

		sort(TEMP, TEMP+9); // Sort TEMP array

		/*cout << "\nSorted TEMP Array: ";
		for (int y=0; y < 9; y++) {
			cout << TEMP[y] << " ";
		}
		cout << endl;*/


		for (int m = 0; m < 9; m++) {
			if (TEMP[m] == sortedarray[m]) { // detect MATCH 
				// do nothing
			}
			else {
				validity[TEMPsubgridnum] = 0; // mismatch, INVALID. Change validity array value
			}
		}

	cout << "\nA subgrid check has successfully completed...exiting thread" << endl;
	return 0;
}



int main() {
	ReadInput(array);
	PrintInput(array);

	//DWORD WorkerCheck[2] = {0,0};
	cout << "\nCreating Column Check Thread..." << endl;
 	HANDLE thread1 = CreateThread(NULL, 0, ColumnCheck, NULL, 0, NULL); // All Column Check
 	cout << "\nCreating Row Check Thread..." << endl;
  	HANDLE thread2 = CreateThread(NULL, 0, RowCheck, NULL, 0, NULL); // All Row Check


  	Parameters *data = (Parameters *) malloc(sizeof (Parameters)); 
	data->row_start = 0; // set parameters to check 3x3 subgrid
	data->row_end = 2;
	data->column_start = 0;
	data->column_end = 2;
	data->subgridnum = 2; // change depending on validity array

  	HANDLE thread3 = CreateThread(NULL, 0, SubgridCheck, (void*)data, 0, NULL); // Top Left 3x3 

  	data->row_start = 0; // set parameters to check 3x3 subgrid
	data->row_end = 2;
	data->column_start = 3;
	data->column_end = 5;
	data->subgridnum = 3; // change depending on validity array

  	HANDLE thread4 = CreateThread(NULL, 0, SubgridCheck, (void*)data, 0, NULL); // Top Middle 3x3

  	data->row_start = 0; // set parameters to check 3x3 subgrid
	data->row_end = 2;
	data->column_start = 6;
	data->column_end = 8;
	data->subgridnum = 4; // change depending on validity array

  	HANDLE thread5 = CreateThread(NULL, 0, SubgridCheck, (void*)data, 0, NULL); // Top Right 3x3

  	data->row_start = 3; // set parameters to check 3x3 subgrid
	data->row_end = 5;
	data->column_start = 0;
	data->column_end = 2;
	data->subgridnum = 5; // change depending on validity array

  	HANDLE thread6 = CreateThread(NULL, 0, SubgridCheck, (void*)data, 0, NULL); // Middle Left 3x3 

  	data->row_start = 3; // set parameters to check 3x3 subgrid
	data->row_end = 5;
	data->column_start = 3;
	data->column_end = 5;
	data->subgridnum = 6; // change depending on validity array

  	HANDLE thread7 = CreateThread(NULL, 0, SubgridCheck, (void*)data, 0, NULL); // Middle 3x3

  	data->row_start = 3; // set parameters to check 3x3 subgrid
	data->row_end = 5;
	data->column_start = 6;
	data->column_end = 8;
	data->subgridnum = 7; // change depending on validity array

  	HANDLE thread8 = CreateThread(NULL, 0, SubgridCheck, (void*)data, 0, NULL); // Middle Right 3x3

  	data->row_start = 6; // set parameters to check 3x3 subgrid
	data->row_end = 8;
	data->column_start = 0;
	data->column_end = 2;
	data->subgridnum = 8; // change depending on validity array

  	HANDLE thread9 = CreateThread(NULL, 0, SubgridCheck, (void*)data, 0, NULL); // Bottom Left 3x3 

  	data->row_start = 6; // set parameters to check 3x3 subgrid
	data->row_end = 8;
	data->column_start = 3;
	data->column_end = 5;
	data->subgridnum = 9; // change depending on validity array

  	HANDLE thread10 = CreateThread(NULL, 0, SubgridCheck, (void*)data, 0, NULL); // Bottom Middle 3x3

  	data->row_start = 6; // set parameters to check 3x3 subgrid
	data->row_end = 8;
	data->column_start = 6;
	data->column_end = 8;
	data->subgridnum = 10; // change depending on validity array

  	HANDLE thread11 = CreateThread(NULL, 0, SubgridCheck, (void*)data, 0, NULL); // Bottom Right 3x3
  
 	WaitForSingleObject(thread1, INFINITE); // wait for each thread to finish
  	WaitForSingleObject(thread2, INFINITE);
  	WaitForSingleObject(thread3, INFINITE);
  	WaitForSingleObject(thread4, INFINITE);
  	WaitForSingleObject(thread5, INFINITE);
  	WaitForSingleObject(thread6, INFINITE);
  	WaitForSingleObject(thread7, INFINITE);
  	WaitForSingleObject(thread8, INFINITE);
  	WaitForSingleObject(thread9, INFINITE);
  	WaitForSingleObject(thread10, INFINITE);
  	WaitForSingleObject(thread11, INFINITE);

	/*HANDLE *threads = new HANDLE[2]; // handler for threads
	threads[0] = thread1;
	threads[1] = thread2;*/

/*
  GetExitCodeThread(threads[0], &WorkerCheck[0]); // testing wait on thread functions
  GetExitCodeThread(threads[1], &WorkerCheck[1]);

  cout << "\nWorkerCheck 1: " << WorkerCheck[0] << endl;
  cout << "WorkerCheck 2: " << WorkerCheck[1] << endl;
*/
  	cout << "\n-------------------------";
 	cout << "ALL THREADS EXITED";
 	cout << "-------------------------" << endl;

 	for (int l = 0; l < 11; l++) {
 		if (validity[l] == 1) {} // all valid do nothing...
 			else {
 				parentvalid = false; // invalid for at least 1 thread change parentvalid to false
 			}
 	}

 	if (parentvalid) {
 		cout << "\nSudoku Puzzle is VALID!" << endl;
 	}
 	else {
 		cout << "\nSudoku Puzzle is INVALID!" << endl;
 	}
	
	return 0;
}