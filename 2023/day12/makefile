CXX=g++
TARGET=main
DEBUG_FLAG=-fdiagnostics-color=always -g
CXXFLAGS= -Wno-psabi -std=c++0x

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%cpp=%o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	${CXX} $(DEBUG_FLAG) $(CXXFLAGS) -c $< -o $@

run:
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o