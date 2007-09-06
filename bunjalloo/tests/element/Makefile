
CFILES:=$(wildcard *.cpp)
OFILES:=$(CFILES:%.cpp=%.o)
CPPFLAGS += -I../../arm9/source/ -I$(LIBBWT)/include -g

all : tester run

LINKFILES:=HtmlElement.o\
           HtmlConstants.o \
           ElementFactory.o \
	   HtmlMetaElement.o \
	   HtmlBodyElement.o \
	   HtmlPreElement.o \
	   HtmlOptionElement.o \
	   HtmlFormElement.o \
	   HtmlInputElement.o \
	   HtmlTextAreaElement.o \
	   HtmlImageElement.o \
	   HtmlAnchorElement.o
#Document.o UTF8.o Entity.o HeaderParser.o HtmlParser.o HtmlDocument.o  ElementFactory.o 

clean:
	rm -f *.o tester

tester: $(OFILES) $(LINKFILES:%.o=../../arm9/build.sdl/%.o) ../Main.o
	$(CXX) -o $@ $^ -L.. -lcppunit -ldl -L$(LIBBWT)/lib -lpcbwt

run: tester
	@./$<
