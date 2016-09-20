.PHONY: clean default

CXX = clang++
CC = $(CXX)
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -Werror -pedantic
CXXFLAGS += -fsanitize=address -fsanitize=undefined
LDFLAGS = $(CXXFLAGS)

objs = $(patsubst %.cc,%.o,$(wildcard *.cc))
bin = test

$(bin): $(objs) 

interval.o: interval.h five_bit.h
five_bit.o: five_bit.h

clean:
	-rm -f $(bin) $(objs)
