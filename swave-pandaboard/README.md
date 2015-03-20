# swave-pandaboard

Generate square wave on Pandaboard ES gpios'.

Compilation:
gcc -o swave_panda swave_panda.c -lrt -Wall

Usage:
./swave_panda 
./swave_panda <priority>
./swave_panda <priority> <interval>

If interval is not set, default value: 50000000. (50000ns = 50us, cycle duration = 100us)
Priority is set to change scheduler as FIFO with value.

