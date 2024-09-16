# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Source files
SRCS = Process.c SchedulerQueue.c BlockedQueue.c Scheduler.c ReadyQueue.c main.c

# Object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Output executable
OUT = a.out

# Default target: build the executable
all: $(OUT)

# Rule to link object files and create the executable
$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build (remove object files and the executable)
clean:
	rm -f $(OBJS) $(OUT)

# Phony targets to avoid conflicts with filenames
.PHONY: all clean