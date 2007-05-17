
CFILES:=$(wildcard *.cpp)
OFILES:=$(CFILES:%.cpp=%.o)
CPPFLAGS += -I../../arm9/source/ -g

all : tester run

clean:
	rm -f *.o tester

LINKFILES:=Document.o \
	   UTF8.o \
	   URI.o \
	   ParameterSet.o \
	   CookieJar.o \
	   HtmlConstants.o \
	   UnicodeString.o \
	   Entity.o \
	   HeaderParser.o \
	   HtmlParser.o \
	   HtmlDocument.o \
	   ElementFactory.o \
	   HtmlElement.o \
	   HtmlBodyElement.o \
	   HtmlPreElement.o \
	   HtmlOptionElement.o \
	   HtmlFormElement.o \
	   HtmlInputElement.o \
	   HtmlMetaElement.o \
	   HtmlImageElement.o \
	   HtmlAnchorElement.o

tester: $(OFILES) $(LINKFILES:%.o=../../arm9/build.sdl/%.o)  ../Main.o
	$(CXX) -o $@ $^ -L.. -lcppunit -ldl

run: tester
	@./$<
