#include <iostream>
#include <windows.h>

using namespace std;


DWORD WINAPI ThreadFunc(void* data) {
  // Do stuff.  This will be the first function called on the new thread.
  // When this function returns, the thread goes away.  See MSDN for more details.
	cout << "task1 says: hello" << endl;
	Sleep(10000);
	cout << "task 1 says: hello again" << endl;

	return 0;
}

int main() {
  HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
  if (thread) {
  	cout << "task1 says: bye" << endl;
    // Optionally do stuff, such as wait on the thread.
  }

  WaitForSingleObject(thread, INFINITE);
  cout << "we outta da loop" << endl;
  return 0;
}