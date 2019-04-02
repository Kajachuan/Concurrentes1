SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
CC := g++

weather: $(OBJS)
	$(CC) $(SRCS) -o $@

%.o: %.cpp
	$(CC) $(SRCS) -c $<

clean:
	rm -f *.o ej
