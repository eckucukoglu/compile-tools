#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/io.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define NSEC_PER_SEC    1000000000

/* using clock_nanosleep of librt */
extern int clock_nanosleep(clockid_t __clock_id, int __flags,
      __const struct timespec *__req,
      struct timespec *__rem);

static inline void tsnorm(struct timespec *ts) {
    while (ts->tv_nsec >= NSEC_PER_SEC) {
        ts->tv_nsec -= NSEC_PER_SEC;
        ts->tv_sec++;
    }
}

int main( int argc, char** argv )
{
    struct timespec t, u;
    struct sched_param param;
    int interval=50000000;  // 50000ns = 50us, cycle duration = 100us
	int retValSleep;
	
    int fd;
    char zero_string[] = "0";
    char one_string[] = "1";
    char buffer[52];
    unsigned char value = 0;

    // Enable GPIO_41, which comes out pin 18 of J6. A list of available GPIOs
    // can be found in the PandaBoard System Reference Manual.
    {
        if ((fd = open("/sys/class/gpio/export", O_WRONLY | O_NDELAY, 0)) == 0) {
            printf("Error: Can't open /sys/class/gpio/export.\n");
            exit(1);
        }   
        strcpy( buffer, "41" );
        write( fd, buffer, strlen(buffer) );
        close(fd);
        printf("Added GPIO 41.\n");
    }

    // Set the direction of the GPIO to "out."
    {
        if ((fd = open("/sys/class/gpio/gpio41/direction", O_WRONLY | O_NDELAY, 0)) == 0) {
            printf("Error: Can't open /sys/class/gpio/gpio41/direction.\n");
            exit(1);
        }
        strcpy( buffer, "out" );
        write( fd, buffer, strlen(buffer) );
        close(fd);
        printf("Direction set to out.\n");
    }

    // Open the "value" node. We will write to it later.
    {
        if ((fd = open("/sys/class/gpio/gpio41/value", O_WRONLY | O_NDELAY, 0)) == 0) {
            printf("Error: Can't open /sys/class/gpio/gpio41/value.\n");
            exit(1);
        }   
        printf("Value opened for writing.\n");
    }

    if ( argc >= 2 && atoi(argv[1]) > 0 ) {
        printf("Using realtime, priority: %d.\n",atoi(argv[1]));
        param.sched_priority = atoi(argv[1]);
        // Enable realtime fifo scheduling.
        if(sched_setscheduler(0, SCHED_FIFO, &param)==-1){
            perror("Error: sched_setscheduler failed.");
            exit(-1);
        }
    }
    if ( argc >= 3 )
        interval=atoi(argv[2]);

    clock_gettime(0,&t); // Get current time.
    t.tv_sec++;          // Start after one second.

	
	
    while (1) {
    	//u.tv_sec = 0;
		//u.tv_nsec = interval;
		
		//retValSleep = usleep(interval/1000);
		
        // wait untill next shot.
        retValSleep = clock_nanosleep(0, TIMER_ABSTIME, &t, NULL);
			
        if(value) {
            write( fd, one_string, 1 );
            //printf("...value set to 1...\n");
        } else {
            write( fd, zero_string, 1 );
            //printf("...value set to 0...\n");
        }
        value = !value;
        // Calculate next shot.
        t.tv_nsec+=interval;
        tsnorm(&t);
   }
   return 0;
}
