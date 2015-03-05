default:
	gcc -g *.c -lm -lpthread -o algo

clean:
	rm -rf algo

