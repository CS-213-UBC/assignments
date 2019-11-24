#include <stdio.h>

int q2(int av, int bv, int cv){
	int i = av;
	int j;
	switch(i) {
	case 10:
		j = bv + cv;
		break;
	case 12:
		j = bv - cv;
		break;
	case 14:
		if((bv - cv) <= 0){
			j = 0;
			break;
		} else {
			j = 1;
			break;
		}
	case 16:
		if((cv-bv) <= 0){
			j = 0;
			break;
		} else {
			j = 1;
			break;
		}
	case 18:
		if((bv-cv) == 0){
			j = 1;
			break;
		} else {
			j = 0;
			break;
		}
	default:
		j = 0;
		break;
	}
	return j;
}

int main()
{
	int a = 20;
	int b = 10;
	int c = 10;

	int d = q2(a, b , c);

    printf("Hello World d = %d\n", d);

    return 0;
}

