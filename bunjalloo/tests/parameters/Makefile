
CFILES:=$(wildcard *.cpp)
OFILES:=$(CFILES:%.cpp=%.o)
CPPFLAGS += -I../../arm9/source/ -I$(LIBBWT)/include -g

all : tester run

LINKFILES:=\
	    ParameterSet.o 

clean:
	rm -f *.o tester

tester: $(OFILES) $(LINKFILES:%.o=../../arm9/build.sdl/%.o) ../Main.o
	$(CXX) -o $@ $^ -L.. -lcppunit -ldl -L$(LIBBWT)/lib -lpcbwt

run: tester
	@./$<
