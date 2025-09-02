CC = g++
CFLAGS = -Wall -Wextra -O2 -MMD -MP
TARGET = bin/countup
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	@$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/%.cpp | obj
	@$(CC) $(CFLAGS) -c $< -o $@

bin:
	@mkdir -p bin

obj:
	@mkdir -p obj

clean:
	@rm -rf obj bin

-include $(DEPS)

.PHONY: all clean