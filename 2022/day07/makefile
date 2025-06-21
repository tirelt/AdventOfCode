CXX=g++ -std=c++17
TARGET=main
DEBUG_FLAG=-fdiagnostics-color=always -g

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%cpp=%o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	${CXX} $(DEBUG_FLAG) -c $< -o $@

run:
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o