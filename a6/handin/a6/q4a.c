#include <stdio.h>
#include <stdlib.h>

int array[] = {0,0,0,0,0,0,0,0,0,0};
int* arrayPointer = array;

void foo(int a, int b) {
	*(arrayPointer + b) = *(arrayPointer + b) + a;
}

int main (int argc, char* argv[]) {
	int x = 1;
	int y = 2;
	foo(3, 4);
	foo(x, y);
	for(int i = 0; i < 10; i++){
		printf("%d\n", * (arrayPointer + i));
	}
}
