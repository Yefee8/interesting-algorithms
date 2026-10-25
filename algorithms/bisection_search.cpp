#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#define exp 1000
#define n 10

// aggregate the step from m
int estimate_step(int length, int m){
	int mid = (length-1)/2;
	// found it
	if(mid == m){
		return 1;
	}

	// target is on the left side
	// drop the right side
	if(mid > m){
		return 1 + estimate_step(mid, m);
	}

	// target is on the right side
	// drop the left side
	// determine the new index for the new array
	return 1 + estimate_step(length - mid - 1 , m - mid - 1);
}

int main()
{
	srand(time(NULL));
	int i, A[n] = {0}, x, d = 0;
	int max = -1, min = -1;

	for (i = 0; i < n; i++)
		A[i] = 11 * i;

	for (int j = 0; j <= exp; j++)
	{
		int l = 0, r = n - 1, m;
		x = (rand() % n)*11;
		while (l <= r)
		{
			d++;
			m = (l + r) / 2;
			if (A[m] == x)
			{
				// we could've just used a simple counter, but what's the joy in that?
				int step = estimate_step(n, m);
				if(max == -1 || step > max) max = step;
				else if(min == -1 || step < min) min = step;
				break;
			}
			else if (A[m] > x)
				r = m - 1;
			else
				l = m + 1;
		}
	}
	printf("\nmax: %d\nmin: %d\navg: %d ",max,min,d/exp);
	return 0;
}
