ASM         = nasm
ASMFLAGS    = -felf64 -g -Isrc/ 
LINKER      = ld

all: bin/forthress
	
bin/forthress: obj/forthress.o
	mkdir -p bin 
	$(LINKER) -o bin/forthress -o bin/forthress obj/forthress.o

obj/forthress.o: src/forthress.asm src/macro.inc src/outer-interpreter.inc src/utility-words.inc
	mkdir -p obj
	$(ASM) $(ASMFLAGS) src/forthress.asm -o obj/forthress.o

clean: 
	rm -rf bin obj
