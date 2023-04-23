CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2
LDLIBS = -lm

TARGET = SearchClient.exe

all: $(TARGET)

SRCS := $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
