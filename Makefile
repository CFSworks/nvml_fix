CC            = gcc
CFLAGS        =
# just set TARGET_VER to a valid ver eg. one of:  390.48 325.08 325.15 319.32 319.23
TARGET_VER    = 430.40
TARGET_MAJOR := $(shell echo ${TARGET_VER} | cut -d . -f 1)
TARGET        = libnvidia-ml.so.1
# change libdir below based on where libnvidia-ml.so.1 resides.
# some common values are: /usr/lib, /usr/lib64, /usr/lib/i386-linux-gnu, /usr/lib/x86_64-linux-gnu
libdir        = /usr/lib/x86_64-linux-gnu
INSTALL       = /usr/bin/install -D

.PHONY: check_supported clean install all

all: check_supported $(TARGET)

check_supported:
ifeq ($(TARGET_MAJOR),319)
else ifeq ($(TARGET_MAJOR),325)
else ifeq ($(TARGET_MAJOR),331)
else ifeq ($(TARGET_MAJOR),390)
else ifeq ($(TARGET_MAJOR),396)
else ifeq ($(TARGET_MAJOR),410)
else ifeq ($(TARGET_MAJOR),415)
else ifeq ($(TARGET_MAJOR),418)
else ifeq ($(TARGET_MAJOR),430)
else
	$(error Driver major version $(TARGET_MAJOR) is not supported!)
endif

${TARGET:1=${TARGET_VER}}: empty.c 
	${CC} ${CFLAGS} -shared -fPIC -s $(<) -o $(@) 

$(TARGET): ${TARGET:1=${TARGET_VER}}
	${CC} ${CFLAGS} -Wl,--no-as-needed -shared -fPIC -s -o $(@) -DNVML_PATCH_${TARGET_MAJOR} -DNVML_VERSION=\"$(TARGET_VER)\" $< nvml_fix.c

clean: 
	rm -f $(TARGET)
	rm -f ${TARGET:1=${TARGET_VER}}

install: libnvidia-ml.so.1
	$(INSTALL) -Dm755 $(^) $(libdir)/$(^)

