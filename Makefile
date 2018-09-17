CC := g++

PROG := test

OBJECTS := easycurl.o test.o

CXXFLAGS := -Wall -g -O0

LDLIBS += -lcurl


default: ${PROG}

all: ${PROG}


$(PROG): $(OBJECTS)

$(OBJECTS): easycurl.h

.PHONY : clean
clean:
	rm -f core.* ${PROGS} ${OBJECTS} *~
