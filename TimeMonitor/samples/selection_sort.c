#include <stdio.h>
#include <stdlib.h>

#include "timemonitor.h"

#define ARRAY_SIZE 100000

void swap(int *a, int *b) {
	int c;
	c = *a;
	*a = *b;
	*b = c;
}

void selection_sort(int *a, int n) {
	int i, j;
	int min;
	for (i = 0; i < n-1; i++) {
		min = i;
		for ( j = i + 1; j < n; j++) {
			if (a[j] < a[min]) {
				min = j;
			}
		}
		if ( min != i ) {
			swap(&a[i], &a[min]);
		}
	}
}

int main(int argc, char *argv[]) {
	TimeMonitor *monitor = NULL;
	int a[ARRAY_SIZE];
	int i;
	
    // init profiler
	monitor = tm_init();
	
	if ( !monitor ) {
		fprintf(stderr, "can't init profiler\n");
		return 1;
	}
	
    // profiling
	tm_start_profiling(monitor);
	
	for (i = 0; i < ARRAY_SIZE; i++) {
		a[i] = (int) rand() % ARRAY_SIZE;
	}
	
    tm_add_timepoint(monitor, "input-case generation completed");
    
    fprintf(stdout, "running selection sort...\n");
    
	selection_sort(a, ARRAY_SIZE);
	
    tm_add_timepoint(monitor, "selection sort completed");
    
    for (i = 1; i < ARRAY_SIZE; i++) {
		if ( a[i-1] > a[i] ) {
            fprintf(stdout, "sort failed!");
            break;
        }
	}
    
    tm_add_timepoint(monitor, "check %d results from %s completed", ARRAY_SIZE, "selection sort algorithm");
    
	tm_stop_profiling(monitor);

	tm_print_info(monitor);

	tm_cleanup(&monitor);
		
	return 0;
}