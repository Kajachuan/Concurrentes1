WEATHER_SRCS := $(wildcard Weather/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
CC := g++

weather: $(OBJS)
	$(CC) $(WEATHER_SRCS) -o $@

%.o: %.cpp
	$(CC) $(WEATHER_SRCS) -c $<

clean:
	rm -f *.o weather
