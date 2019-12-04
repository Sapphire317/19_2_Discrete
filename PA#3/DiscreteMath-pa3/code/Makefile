all:
	gcc `pkg-config --cflags --libs glib-2.0` -I../include trainer.c `pkg-config --libs glib-2.0` ../lib/libstemmer.o -lm -o trainer
	gcc -o predictor `pkg-config --cflags --libs glib-2.0` -I../include predictor.c `pkg-config --libs glib-2.0` ../lib/libstemmer.o -lm