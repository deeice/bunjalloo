
CFILES:=$(wildcard *.cpp)
OFILES:=$(CFILES:%.cpp=%.o)
CPPFLAGS += -I../../arm9/source/ -I$(LIBBWT)/include -g

all : tester run

LINKFILES:=Document.o \
	   URI.o \
	   Entity.o \
	   HeaderParser.o \
	   ParameterSet.o \
	   CookieJar.o \
	   HtmlParser.o \
	   HtmlDocument.o \
	   HtmlConstants.o \
	   ElementFactory.o \
	   HtmlElement.o \
	   HtmlBodyElement.o \
	   HtmlOptionElement.o \
	   HtmlFormElement.o \
	   HtmlInputElement.o \
	   HtmlPreElement.o \
	   HtmlMetaElement.o \
	   HtmlImageElement.o \
	   HtmlTextAreaElement.o \
	   HtmlAnchorElement.o

clean:
	rm -f *.o tester

tester: $(OFILES) $(LINKFILES:%.o=../../arm9/build.sdl/%.o) ../Main.o
	$(CXX) -o $@ $^ -L.. -lcppunit -ldl -L$(LIBBWT)/lib -lpcbwt

run: tester
	@./$<
