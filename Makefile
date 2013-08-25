CC            = gcc
CFLAGS        =
TARGET_VER    = 325.15# just set to a valid ver eg. one of:  325.08 325.15 319.32 319.23
TARGET        = libnvidia-ml.so.1
DESTDIR       = /
PREFIX        = $(DESTDIR)usr
libdir        = $(PREFIX)/lib
INSTALL       = /usr/bin/install -D

all: $(TARGET)

${TARGET:1=${TARGET_VER}}: empty.c 
	${CC} ${CFLAGS} -shared -fPIC $(<) -o $(@) 

$(TARGET): ${TARGET:1=${TARGET_VER}}
	${CC} ${CFLAGS} -shared -fPIC -o $(@) -DNVML_PATCH_319 -DNVML_VERSION=\"$(TARGET_VER)\" $< nvml_fix.c


clean: 
	rm -f $(TARGET)
	rm -f ${TARGET:1=${TARGET_VER}}

install: libnvidia-ml.so.1
	$(INSTALL) -Dm755 $(^) $(libdir)/$(^)

.PHONY: clean install all
