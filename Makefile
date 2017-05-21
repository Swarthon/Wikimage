CFLAGS=-fopenmp -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16 -I/usr/include/ImageMagick-6 -Wall
LDFLAGS=-lcurl -lMagickCore-6.Q16 -lX11
EXEC=wikimage

all : wikimage.o
	gcc wikimage.o -o $(EXEC) $(LDFLAGS)

wikimage.o : wikimage.c
	gcc -c wikimage.c $(CFLAGS)

run : all
	./$(EXEC)

clean :
	@rm wikimage.o $(EXEC)

mrproper :
	@rm $(EXEC) *.png *.html
