# make-sdcard

This script creates partitions on disk and copies boot images and rootfs.
User has to set copied files directory in script.
See comments to edit.

# Usage:

./mksdcard *OPTION*

# Options:

--device *device*     SD block device node (e.g /dev/mmcblk0)
--pes *version*      Which pandaboard es version wanted

Example:

./mksdcard --device /dev/mmcblk0 --pes 5

# Version history:

v0.3
- Instead of dd, first mount rootfs then copy the content.
- Partition names capitalized.
- Format second partiton as ext4.
