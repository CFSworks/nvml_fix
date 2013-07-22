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
The makefile can be used with `make` to build shims the selected NVML version `make TARGET_VER=325.08`
To install, delete the `libnvidia-ml.so.1` symlink currently in your
`/usr/lib` and run `make install PREFIX=/usr/lib`. Currently valid versions are: 325.08, 319.32 and 319.23.

You can build 32-bit versions with `make CFLAGS=-m32`.

Note: The nVidia drivers are not a dependency for building the shims.

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
