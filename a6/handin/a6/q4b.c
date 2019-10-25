#include <stdio.h>
#include <stdlib.h>

int x[] = {1,2,3,-1,-2,0,184,340057058};
int y[] = {0,0,0,0,0,0,0,0};

int f(int a) {
	int i = 0;
	while(a != 0) {
		if ((a & 0x80000000) != 0)
			i++;
		a = a << 1;	
	}
	return i;
}

int main (int argc, char* argv[]) {

    for (int i = 7; i>=0; i--){
        y[i] = f(x[i]);
    }

	for(int i = 0; i < 8; i++){
        printf("%d\n", x[i]);
    }

	for(int i = 0; i < 8; i++){
        printf("%d\n", y[i]);
    }
}
