WEATHER_SRCS := $(wildcard Weather/*.cpp)
CLIENT_SRCS := $(wildcard Client/*.cpp)
PORTAL_SRCS := $(wildcard Portal/*.cpp)
FIFO_SRCS := $(wildcard Fifo/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
CC := g++ -std=c++11

all: weather client portal

weather: $(OBJS)
	$(CC) $(WEATHER_SRCS) $(FIFO_SRCS) -o $@

client: $(OBJS)
	$(CC) $(CLIENT_SRCS) $(FIFO_SRCS) -o $@

portal: $(OBJS)
	$(CC) $(PORTAL_SRCS) $(FIFO_SRCS) -o $@

%.o: %.cpp
	$(CC) $(SRCS) -c $<

clean:
	rm -f *.o weather client portal
