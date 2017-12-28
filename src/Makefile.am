bin_PROGRAMS = wikimage
wikimage_SOURCES = download.c  image.c  string_manipulate.c  wikimage.c
AM_CPPFLAGS = -I$(srcdir)/../include -DUSE_X11
wikimage_CFLAGS = $(MAGICKCORE_CFLAGS) $(MAGICKCORE_LIBS) -lcurl -lX11
