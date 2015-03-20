# gpio-interrupt

Waits for a change in the INT pin or input on stdin.
Then set GPIO pin, wait delay, then reset it.

Usage:

./int2gpio *int-pin* *gpio-pin* *udelay* *prio*
