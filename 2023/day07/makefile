CXX=g++
TARGET=main
DEBUG_FLAG=-fdiagnostics-color=always -g
CXXFLAGS= -Wno-psabi

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%cpp=%o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp %.h
	${CXX} $(DEBUG_FLAG) $(CXXFLAGS) -c $< -I ./ -o $@

run:
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o