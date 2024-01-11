CC = g++ -std=c++11

310513081.out : 310513081.o Liberty.o NetList.o Wire.o Gate.o
	$(CC) -o 310513081 310513081.o Liberty.o NetList.o Wire.o Gate.o

310513081.o : 310513081.cpp NetList.h
	$(CC) -c 310513081.cpp

Liberty.o : Liberty.cpp Liberty.h
	$(CC) -c Liberty.cpp

NetList.o : NetList.cpp NetList.h
	$(CC) -c NetList.cpp

Wire.o : Wire.cpp Wire.h Gate.h
	$(CC) -c Wire.cpp

Gate.o : Gate.cpp Gate.h Wire.h
	$(CC) -c Gate.cpp

clean :
	rm *.o
	rm 310513081
	rm *.v
	rm *.txt
	rm *.lib
