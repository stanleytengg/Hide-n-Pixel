bmp_steganography: bmp_steganography.o bmp.o
	gcc -o bmp_steganography bmp_steganography.o bmp.o

bmp_steganography.o: bmp_steganography.c
	gcc -c bmp_steganography.c

bmp.o: bmp.c
	gcc -c bmp.c