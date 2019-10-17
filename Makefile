PROG=ms5611test

$(PROG): main.o ms5611.o i2c_ms5611.o
	gcc -o $(PROG) main.o ms5611.o i2c_ms5611.o

main.o: main.c ms5611.h i2c_ms5611.h
	gcc -c main.c ms5611.h i2c_ms5611.h
	
ms5611.o: ms5611.c ms5611.h i2c_ms5611.h
	gcc -c ms5611.c ms5611.h i2c_ms5611.h

i2c_ms5611.o: i2c_ms5611.c i2c_ms5611.h
	gcc -c i2c_ms5611.c i2c_ms5611.h
	
clean:
	rm -rf *.o
	rm -rf *.h.gch
	rm -rf $(PROG)