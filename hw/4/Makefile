.PHONY: clean default

CXX = clang++
CC = $(CXX)
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -Werror -pedantic
#CXXFLAGS += -fsanitize=address -fsanitize=undefined
LDFLAGS = $(CXXFLAGS)

objs = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
bin = test

$(bin): $(objs) 

clean:
	-rm -f $(bin) $(objs)
