#include <stdio.h>
#include <stdlib.h>

int main () {
	int seed;
	scanf("%d", &seed);
	srand(seed);
	int n = rand() % 1000;
	for (int i=0; i<n; i++) {
		if (n % 57 == 0)
			printf("\n");
		int m = rand() % 1000;
		for (int j=0; j<m; j++) {
			int k = rand() % 5;
			if (k == 0) 
				printf(" ");
			else if (k == 1)
				printf("a");
			else if (k == 2)
				printf("b");
			else if (k == 3)
				printf("c");
			else
				printf("d");
		}
		printf("\n");
	}
	return 0;
}
