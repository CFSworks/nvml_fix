About
=====

Rationale
---------
This is a workaround for a fairly annoying bug in nVidia's Linux drivers.
A long time ago, you could see all sorts of helpful information on your GeForce GPU simply by typing
`nvidia-smi` - however, in recent drivers, the output is largely dominated by "N/A". This is
because nVidia no longer supports the GeForce series GPUs in `nvidia-smi`.

The real bug, however, resides in the nVidia Management Library (NVML), which incorrectly reports
that the GPU isn't supported. This repository provides a fix for that bug.

How it works
------------
This workaround is a shim that sits between the program trying to use NVML (Ganglia plugin, pyNVML,
nvidia-smi, ...) and the actual NVML library itself. Whenever a device handle is requested from
NVML, the shim flips an internal "supported" flag before returning it to the hosted program.
Therefore, when the handle is used in subsequent calls to the library, NVML correctly sees that
the device is in fact supported, and returns information properly.

How to use
----------
The Makefile can be used to build shims for a given NVML version with `make TARGET_VER=major.minor`.
The full *major.minor* value must be specified, so `TARGET_VER=470` isn't sufficient, but
`TARGET_VER=470.42.01` is:  
  * `make TARGET_VER=470.42.01`  

Currently supported versions are: 470.x, 460.x, 450.x, 440.x, 430.x, 418.x, 415.x, 410.x, 396.x, 390.x,
331.x, 325.x (x86_64 and i386), and 319.x (x86_64 and i386), with the latest being the default.  

To install, delete the `libnvidia-ml.so.1` symlink currently in your `libdir` and run
`make install libdir=/path/to/lib`:  
  * `sudo make install TARGET_VER=470.42.01 libdir=/usr/lib/x86_64-linux-gnu`  

It is necessary to supply `TARGET_VER` during *both* `make` and `make install` if not using the default
version.  

Some common values for `libdir` are `/usr/lib`, `/usr/lib64` (32-bit and 64-bit rpm-based distros),
`/usr/lib/i386-linux-gnu`, `/usr/lib/x86_64-linux-gnu` (32-bit and 64-bit Ubuntu-based distros), and 
`/usr/lib/i386-linux-gnu/nvidia/current`, `/usr/lib/x86_64-linux-gnu/nvidia/current` 
(32-bit and 64-bit Debian).  

On Debian-based distros an alternative to deleting the symlink is to use `dpkg-divert` to rename it
(before running `make install`):  
  * `sudo dpkg-divert --add --local --divert /usr/lib/x86_64-linux-gnu/libnvidia-ml.so.1.orig --rename
/usr/lib/x86_64-linux-gnu/libnvidia-ml.so.1`  

The current Makefile defaults are `TARGET_VER=470.42.01 libdir=/usr/lib/x86_64-linux-gnu`.  

If you are on a 64-bit system, you can build 32-bit versions with `make CFLAGS=-m32`.  

Legal
=====

Disclaimer
----------
I am not to be held responsible if something breaks down on you. Use at your own risk.

Copyright
---------
The nVidia driver license forbids distributing modified binary files. This repository contains no
binary files from nVidia, and use of the shim makes no modifications to binary files either.

License
-------
Gosh I dunno... Public domain, I guess. :)
