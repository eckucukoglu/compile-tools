# preempt_rt-patch-script

I use this program to apply preempt_rt patch to linux kernel, and 
log the results.

rt_patch_script does:
Analyze patches/series text file for patch file names,
Generate shell script file to do patches,
Do patches,
Report in log file that patch is succeeded, hunked, failed.

Compilation:

./make

Usage:

./rt_patch_script *OPTION*

Options:

- -p: Do patches without logging
- -l: Report results in a log file
- -g: Generate shell script file

Example usage:

./rt_patch_script -gl
