.PHONY: clean default

CXX = clang++
CC = $(CXX)
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -Werror -pedantic
CXXFLAGS += -fsanitize=address -fsanitize=undefined
LDFLAGS = $(CXXFLAGS)

objs = $(patsubst %.cc,%.o,$(wildcard *.cc))
bin = test

$(bin): $(objs) 

interval.o: interval.h util.h
util.o: util.h

clean:
	-rm -f $(bin) $(objs)
