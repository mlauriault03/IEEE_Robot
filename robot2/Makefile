CFLAGS = -Wall -Wextra

test: override CFLAGS += -DDEBUG
test: test.o mechanics.o mock.o
	g++ $(CFLAGS) -o test $^

test.o: test.cpp robot2.h mechanics.h mock.h
	g++ $(CFLAGS) -c test.cpp

mechanics.o: mechanics.cpp mock.h robot2.h mechanics.h
	g++ $(CFLAGS) -c mechanics.cpp

mock.o: mock.cpp mock.h robot2.h
	g++ $(CFLAGS) -c mock.cpp

clean:
	rm -f test *.o
