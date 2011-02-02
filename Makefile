#Customisable stuff here
LINUX32_COMPILER = i686-pc-linux-gnu-gcc
LINUX64_COMPILER = x86_64-pc-linux-gnu-gcc
WIN32_COMPILER = /usr/bin/i586-mingw32-gcc
WIN32_WINDRES = i586-mingw32-windres
#LINUX_ARM_COMPILER = arm-pc-linux-gnu-gcc
LINUX_ARM_COMPILER = arm-none-linux-gnueabi-gcc
LINUX_PPC_COMPILER = powerpc-unknown-linux-gnu-gcc
FREEBSD60_COMPILER = i686-pc-freebsd6.0-gcc
MACPORT_COMPILER = i686-apple-darwin9-gcc-4.0.1

LIBPURPLE_CFLAGS = -I/usr/include/libpurple -I/usr/local/include/libpurple 
GLIB_CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/gtk-2.0/include/ -I/usr/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -I/usr/local/include
WIN32_DEV_DIR = /root/pidgin/win32-dev
WIN32_PIDGIN_DIR = /root/pidgin/pidgin-2.3.0_win32
WIN32_CFLAGS = -I${WIN32_DEV_DIR}/gtk_2_0/include/glib-2.0 -I${WIN32_PIDGIN_DIR}/libpurple/win32 -I${WIN32_PIDGIN_DIR}/pidgin/win32 -I${WIN32_DEV_DIR}/gtk_2_0/include -I${WIN32_DEV_DIR}/gtk_2_0/include/glib-2.0 -I${WIN32_DEV_DIR}/gtk_2_0/lib/glib-2.0/include -I${WIN32_DEV_DIR}/gtk_2_0/lib/gtk-2.0/include -Wno-format
WIN32_LIBS = -L${WIN32_DEV_DIR}/gtk_2_0/lib -L${WIN32_PIDGIN_DIR}/libpurple -L${WIN32_PIDGIN_DIR}/pidgin -lglib-2.0 -lgobject-2.0 -lintl -lpidgin -lpurple -lws2_32 -L. -lgtk-win32-2.0
MACPORT_CFLAGS = -I/opt/local/include/libpurple -I/opt/local/include/glib-2.0 -I/opt/local/lib/glib-2.0/include -I/opt/local/include -arch i386 -arch ppc -dynamiclib -L/opt/local/lib -lpidgin -lpurple -lglib-2.0 -lgobject-2.0 -lintl -lz -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4

DEB_PACKAGE_DIR = ./debdir

SOURCES = \
	icon_override.c
	
#Standard stuff here
.PHONY:	all clean install sourcepackage

all:	icon_override.dll icon_override.so icon_override64.so

install:
	cp icon_override.so /usr/lib/purple-2/
clean:
	rm -f icon_override.dll icon_override.so icon_override64.so

icon_override.so:	${SOURCES}
	${LINUX32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall ${GLIB_CFLAGS} -I. -g -O2 -pipe ${SOURCES} -o $@ -shared -fPIC -DPIC

icon_override64.so:	${SOURCES}
	${LINUX64_COMPILER} ${LIBPURPLE_CFLAGS} -Wall ${GLIB_CFLAGS} -I. -g -O2 -pipe ${SOURCES} -o $@ -shared -fPIC -DPIC

icon_override.dll:	${SOURCES}
	${WIN32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall -I. -g -O0 -pipe ${SOURCES} -o $@ -shared -mno-cygwin ${WIN32_CFLAGS} ${WIN32_LIBS}
	upx $@
