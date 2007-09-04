
CFILES:=$(wildcard *.cpp)
OFILES:=$(CFILES:%.cpp=%.o)
CPPFLAGS += -I../../arm9/source/ -I$(LIBBWT)/include -g

all : tester run

clean:
	rm -f *.o tester

tester: $(OFILES) ../Main.o
	$(CXX) -o $@ $^ -L.. -lcppunit -ldl -L$(LIBBWT)/lib -lpcbwt

run: tester
	@./$<
