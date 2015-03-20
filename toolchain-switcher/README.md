# toolchain-switcher

Thomas Petazzoni's shell code to switch various toolchains from shell easily.
I made a few changes on it. 

Reference: http://free-electrons.com/blog/switching-toolchains/

Script sets and unsets these environment variables:

PATH, CC, CROSS_COMPILE, ARCH, CXX, STAGING_LIBDIR

Usage:

Append content of xtools to your ~/.bashrc.

- xtoolsadd *toolchain* 
- xtoolsdel *toolchain*
