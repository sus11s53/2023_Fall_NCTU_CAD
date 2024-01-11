CC = g++ -std=c++11

STA.out : STA.o Liberty.o NetList.o Wire.o Gate.o Pattern.o
	$(CC) -o STA STA.o Liberty.o NetList.o Wire.o Gate.o Pattern.o

STA.o : STA.cpp NetList.h
	$(CC) -c STA.cpp

Liberty.o : Liberty.cpp Liberty.h
	$(CC) -c Liberty.cpp

NetList.o : NetList.cpp NetList.h
	$(CC) -c NetList.cpp

Wire.o : Wire.cpp Wire.h Gate.h
	$(CC) -c Wire.cpp

Gate.o : Gate.cpp Gate.h Wire.h
	$(CC) -c Gate.cpp

Pattern.o : Pattern.cpp Pattern.h
	$(CC) -c Pattern.cpp

clean :
	rm *.o
	rm STA
