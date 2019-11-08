# This is the makefile that generates the executable

# Files to compile
FILES_C = plantilla-menu.c red-black-tree.c read_tools.c tree_operations.c

# Exectuable to generate
TARGET = practica3



FILES_O = $(subst .c,.o,$(FILES_C))

$(TARGET): $(FILES_O) Makefile
	gcc $(FILES_O) -o $(TARGET)

%.o: %.c Makefile
	gcc -c $<

all: $(TARGET) 

clean:
	rm -f *.o *~ 
