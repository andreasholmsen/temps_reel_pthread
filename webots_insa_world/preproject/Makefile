run:
	gcc main.c -o main -O0 -pthread && ./main 

run_single:
	gcc main.c -o main -O0 -pthread && taskset -c 0 ./main 

clean:
	rm main