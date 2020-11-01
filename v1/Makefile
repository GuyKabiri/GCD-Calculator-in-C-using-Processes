# Guy Kabiri 312252224

EXEC = v1_parent v1_child

all: clean $(EXEC)

v1_parent:
	gcc -o v1_parent v1_parent.c

v1_child:
	gcc -o v1_child v1_child.c

clean:
	rm -f $(EXEC)