# I am terrible at make, but here goes anyway:

CC=gcc
CFLAGS=

all: 325.08 319.32 319.23

empty.so:
	mkdir -p .tmp
	touch .tmp/empty.c
	${CC} ${CFLAGS} -shared -fPIC -o .tmp/empty.so .tmp/empty.c

325.08: empty.so
	cp .tmp/empty.so libnvidia-ml.so.325.08
	mkdir -p built/325.08
	${CC} ${CFLAGS} -shared -fPIC -o built/325.08/libnvidia-ml.so.1 -DNVML_PATCH_319 -DNVML_VERSION=\"325.08\" libnvidia-ml.so.325.08 nvml_fix.c
	rm libnvidia-ml.so.325.08

319.32: empty.so
	cp .tmp/empty.so libnvidia-ml.so.319.32
	mkdir -p built/319.32
	${CC} ${CFLAGS} -shared -fPIC -o built/319.32/libnvidia-ml.so.1 -DNVML_PATCH_319 -DNVML_VERSION=\"319.32\" libnvidia-ml.so.319.32 nvml_fix.c
	rm libnvidia-ml.so.319.32

319.23: empty.so
	cp .tmp/empty.so libnvidia-ml.so.319.23
	mkdir -p built/319.23
	${CC} ${CFLAGS} -shared -fPIC -o built/319.23/libnvidia-ml.so.1 -DNVML_PATCH_319 -DNVML_VERSION=\"319.23\" libnvidia-ml.so.319.23 nvml_fix.c
	rm libnvidia-ml.so.319.23

clean:
	rm -rf empty.c *.so.* *.so built/ .tmp
