run:
	gcc main.c -o main -O0 -pthread && ./main 

run_single:
	gcc main.c -o main -O0 -pthread && taskset -c 0 ./main 

run_2:
	gcc main_2threads.c -o main_2threads -O0 -pthread && ./main_2threads

run_2_single:
	gcc main_2threads.c -o main_2threads -O0 -pthread && taskset -c 0 ./main_2threads 

clean:
	rm main