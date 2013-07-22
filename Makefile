CC            = gcc
CFLAGS        =
TARGET_VER    = 319.32# just set to a valid ver eg. one of:  325.08 319.32 319.23
TARGET        = libnvidia-ml.so.1
DESTDIR       = /
PREFIX        = $(DESTDIR)usr/local
libdir        = $(PREFIX)/lib
INSTALL       = /usr/bin/install -D

all: $(TARGET)

${TARGET:1=${TARGET_VER}}: empty.c 
	${CC} ${CFLAGS} -shared -fPIC $(<) -o $(@) 

$(TARGET): ${TARGET:1=${TARGET_VER}}
	${CC} ${CFLAGS} -shared -fPIC -o $(@) -DNVML_PATCH_319 -DNVML_VERSION=\"$(TARGET_VER)\" $< nvml_fix.c


clean: 
	rm -f $(TARGET)
	rm -f empty.o 

install: libnvidia-ml.so.1
	$(INSTALL) -Dm644 $(^) $(libdir)/$(^)

.PHONY: clean install all
