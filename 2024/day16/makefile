CXX=g++
TARGET=main
DEBUG_FLAG=-fdiagnostics-color=always -g
STD_FLAG=-std=c++20
CXXFLAGS= -Wno-psabi 

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%cpp=%o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	${CXX} $(STD_FLAG) $(DEBUG_FLAG) $(CXXFLAGS) -c $< -o $@

run:
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o