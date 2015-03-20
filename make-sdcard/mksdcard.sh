#! /bin/sh

VERSION="[make-sdcard v0.3]"

execute () {
    $* >/dev/null
    if [ $? -ne 0 ]; then
        echo
        echo ">> ERROR: executing $*"
        echo
        exit 1
    fi
}

version () {
  echo
  echo ">> $VERSION"
  echo
  echo ">> This script creates partitions for Pandaboard ES"
  echo ">> and copies boot images and rootfs."
  echo

  exit 0
}

usage () {
  echo "
Usage: `basename $1` <options>

Mandatory options:
  --device <device>     SD block device node (e.g /dev/mmcblk0)
  --pes <version>       Which pandaboard es version wanted

Optional options:
  --version             Print version.
  --help                Print this help message.
  
Example usage:
  ./`basename $1` --device /dev/mmcblk0 --pes 5
"
  exit 1
}

while [ $# -gt 0 ]; do
  case $1 in
    --help | -h)
      usage $0
      ;;
    --device) shift; device=$1; shift;
      ;;
    --pes) shift; pes=$1; shift;
      ;;
    --version) version $0
      ;;
  esac
done

# Open help, if no device or pes selected.
test -z $device || test -z $pes && usage $0

if [ ! -b $device ]; then
   echo ">> ERROR: $device is not a block device file"
   exit 1;
fi

# Directory path
dir_main="../pandaboardes-v${pes}/"
dir_sdk="/tmp/sdk"
dir_tmp="${dir_sdk}/$$"

# Boot images
file_bootscript=boot.scr
file_mlo=MLO
file_bootloader=${dir_main}u-boot.img
file_kernel=${dir_main}uImage
file_dtb=${dir_main}omap4-panda-es.dtb

# Rootfs 
file_rootfs=${dir_main}rootfs-extended.ext2

# Partition names
firstPartitionName=BOOT
secondPartitionName=ROOTFS

for i in `ls -1 ${device}* | grep -E ${device}.+`; do
 echo ">> Unmounting '$i'"
 umount $i 2>/dev/null
done

execute "dd if=/dev/zero of=$device bs=1024 count=1024"

echo ">> Calculating size and number of cylinders"
total_size=`fdisk -l $device | grep Disk | awk '{print $5}'`
total_cyln=`echo $total_size/255/63/512 | bc`
echo ">> Size: $total_size, #cylinder: $total_cyln"

# For boot partition, number of cylinder:
cylnSizeOfBoot=10

# Change partition tables, first for boot, second for rootfs.
# sfdisk options:
# -H    : number of heads,
# -S    : number of sectors per track,
# -C    : number of cylinders,
# sfdisk input format:
# <start>,<size>,<id>,<bootable>

{
    echo ,$cylnSizeOfBoot,0x0C,*
    echo ,,,-
} | sfdisk --force -D -H 255 -S 63 -C $total_cyln $device

if [ $? -ne 0 ]; then
    echo ">> Error message:"
    exit 1;
fi

execute "mkdir -p ${dir_sdk}"

# Format boot partition as FAT.
echo ">> Formating ${device}p1"
execute "mkfs.vfat -F 32 -n ${firstPartitionName} ${device}p1"

# Mount boot partition.
echo ">> Mounting ${device}p1"
execute "mkdir -v -p ${dir_tmp}"
execute "mount ${device}p1 ${dir_tmp}"

# Copy the boot files.
echo ">> Copying on ${device}p1"
execute "cp ${file_bootscript} ${dir_tmp}/"
execute "cp ${file_mlo} ${dir_tmp}/"
execute "cp ${file_bootloader} ${dir_tmp}/"
execute "cp ${file_kernel} ${dir_tmp}/"
execute "cp ${file_dtb} ${dir_tmp}/"

sync
echo ">> Unmounting ${device}p1"
execute "umount ${dir_tmp}"

# Format second partition as ext4.
echo ">> Formating ${device}p2"
execute "mkfs.ext4 -L ${secondPartitionName} ${device}p2"

# Mount second partition.
echo ">> Mounting ${device}p2"
execute "mkdir -v -p ${dir_tmp}"
execute "mount ${device}p2 ${dir_tmp}"

# Mount rootfs to local area.
echo ">> Mounting ${file_rootfs}"
execute "mkdir -v -p ${dir_tmp}1"
execute "mount ${file_rootfs} ${dir_tmp}1"

# Copy the rootfs to second partition.
echo ">> Copying on ${device}p2"
execute "cp -r ${dir_tmp}1/* ${dir_tmp}/"

sync
echo ">> Unmounting ${device}p2"
execute "umount ${dir_tmp}"
echo ">> Unmounting ${file_rootfs}"
execute "umount ${dir_tmp}1"

echo ">> Success."

