# boot-script

To produce boot.scr:

mkimage -A arm -O linux -T script -C none -a 0 -e 0 -n "Panda SD Boot" -d boot.script boot.scr
