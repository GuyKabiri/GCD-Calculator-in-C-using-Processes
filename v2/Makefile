EXEC = v2_parent v2_child

all: clean $(EXEC)

v2_parent:
	gcc -o v2_parent v2_parent.c

v2_child:
	gcc -o v2_child v2_child.c

clean:
	rm -f $(EXEC)