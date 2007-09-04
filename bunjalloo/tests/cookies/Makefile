
CFILES:=$(wildcard *.cpp)
OFILES:=$(CFILES:%.cpp=%.o)
CPPFLAGS += -I../../arm9/source/ -I$(LIBBWT)/include -g

all : tester run

LINKFILES:=\
	    ParameterSet.o \
	    CookieJar.o \
	    URI.o

clean:
	rm -f *.o tester

tester: $(OFILES) $(LINKFILES:%.o=../../arm9/build.sdl/%.o) ../Main.o
	$(CXX) -o $@ $^  -L$(LIBBWT)/lib -L.. -lcppunit -ldl -lpcbwt

run: tester
	@./$<
