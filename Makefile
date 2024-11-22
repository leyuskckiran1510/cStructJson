

BIN = app.exe

run:main.c
	gcc -o $(BIN) main.c 
	./$(BIN)

debug:main.c
	gcc -o $(BIN) main.c -ggdb  -O0
	gf2 ./$(BIN)

mem:main.c
	gcc -o $(BIN) main.c -fsanitize-recover=all -fsanitize=address -ggdb  -O0 
	./$(BIN)  
