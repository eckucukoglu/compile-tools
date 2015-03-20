#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sched.h>
#include <string.h> /* memset */

/* Heap matrix dimension */
int dimension = 500;
/* Max safe stack area */
int max_safe_stack = 8 * 1024;

struct timeval tv; 

double timestamp () {
    double t;
    gettimeofday(&tv, NULL);
    t = tv.tv_sec + (tv.tv_usec/1000000.0);
    return t;
}

void stack_prefault () {
    unsigned char dummy[max_safe_stack];
    memset(dummy, 0, max_safe_stack);
    printf("stack: %lu bytes\n", sizeof(dummy));
    return;
}

int main(int argc, char *argv[]) {
    int i, j, k;
    double *A, *B, *C, start, end;
    unsigned long size;
    struct sched_param param;

	if (argc != 4 || atoi(argv[1]) < 0 || atoi(argv[3]) < 0) {
		printf("usage: %s [dim] [prio] [stack]\n", argv[0]);
		exit(-1);
	}
	
	/* Change priority of a task */
	if (atoi(argv[2]) > 0) {
		printf("using priority: %d\n",atoi(argv[2]));
		param.sched_priority = atoi(argv[2]);
		if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
			perror("sched_setscheduler failed");
			exit(-1);
		}
	}
	
	/* Allocate stack area */
	if ((max_safe_stack = atoi(argv[3])) != 0)
        stack_prefault();
        
	/* Allocate heap area */
	dimension = atoi(argv[1]);
	size = dimension*dimension*sizeof(double);
    A = (double*)malloc(size);
    B = (double*)malloc(size);
    C = (double*)malloc(size);
	printf("heap: %lu bytes\n", size*3);

    srand(292);

    for (i = 0; i < dimension; i++)
        for(j = 0; j < dimension; j++) {   
            A[dimension*i+j] = (rand()/(RAND_MAX + 1.0));
            B[dimension*i+j] = (rand()/(RAND_MAX + 1.0));
            C[dimension*i+j] = 0.0;
        }   

    start = timestamp();
    for(i = 0; i < dimension; i++)
        for(j = 0; j < dimension; j++)
            for(k = 0; k < dimension; k++)
                C[dimension*i+j] += A[dimension*i+k] *
                    B[dimension*k+j];

    end = timestamp();
    printf("secs:%f\n", end-start);

    free(A);
    free(B);
    free(C);

    return 0;
}
