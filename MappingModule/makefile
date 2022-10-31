CC = gcc # Which compiler to use
LDFLAGS = # Which libraries to use -pthread -lm -lefence etc…
OBJFILES = main.o linkedlist.o # Which object files are part of the final program
TARGET = main # What to name the final prog

# Which flags to use 
# -Werror
CFLAGS += -ggdb -g -Wall -Wextra -Wsign-compare -Wunused-parameter -Wunused-variable -Wshadow

# How to use:
# To use our Makefile:
# • Just type “make”
# – It will figure out which .c files need to be
# recompiled and turned into .o files
# • If the .c file is newer than the .o file or
# • the .o file does not exist
# – Figures out if the program needs to be relinked
# • If any of the .o files changed or
# • If the program does not exist

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

# type “make clean”
# – Deletes:
# • all the .o files
# • all the ~ files (from emacs)
# • the program itself
# – Leaves:
# • .c files
# • .h files
# • Makefile
clean:
	rm -f $(OBJFILES) $(TARGET) *~
