
CFILES:=$(wildcard *.cpp)
OFILES:=$(CFILES:%.cpp=%.o)
CPPFLAGS += -I../../arm9/source/ -g 

all : tester run

clean:
	rm -f *.o tester

tester: $(OFILES) ../../arm9/build.sdl/UTF8.o ../Main.o
	$(CXX) -o $@ $^ -L.. -lcppunit -ldl

run: tester
	@./$<
