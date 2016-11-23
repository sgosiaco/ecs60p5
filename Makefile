blood.out: blood.o bloodRunner.o braincell.o
	g++ -Wall -ansi -g -o blood.out blood.o bloodRunner.o braincell.o

blood.o: blood.cpp blood.h bloodRunner.h braincell.h
	g++ -Wall -ansi -g -c blood.cpp

bloodRunner.o: bloodRunner.cpp bloodRunner.h CPUTimer.h blood.h braincell.h
	g++ -Wall -ansi -g -c bloodRunner.cpp

braincell.o : braincell.cpp braincell.h
	g++ -Wall -ansi -g -c braincell.cpp

clean:
	rm -f blood.out blood.o bloodRunner.o
