# linux-tools

Some basic tools I used, while developing, compiling and testing Linux related works.
List is here:

# make-sdcard

Basically, partition sdcard devices as boot & rootfs,
and copy bootloader, kernel, device tree blob
images to boot partition. And copy rootfs image
to rootfs partition.

# boot-script

boot.scr is used in boot partitions of disks to load kernel image, device tree blobs.
Also bootloader environment variables can be set, e.g. bootargs.

# preempt_rt-patch-script

I use this program to apply preempt_rt patch to linux kernel, and 
log the results. 
It analyze patches/series text file for patch file names, 
generate shell script file to do patches, do patches, 
report in log file that patch is succeeded, hunked, failed.

# gpio-interrupt

Waits for a change in the INT pin or input on stdin.
Then set GPIO pin, wait delay, then reset it.

# matrix-multiplication

For a given priority, allocate heap & stack area, then do matrix multiplication in heap.

# swave-pandaboard

Generate square wave on Pandaboard ES gpios'.
