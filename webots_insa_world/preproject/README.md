
# Preproject

                gcc main.c -o main -O0 -pthread && ./main 
                1 Done (response time: 2878 ms)
                0 Done (response time: 2885 ms)
                0 Done (response time: 1724 ms)
                1 Done (response time: 2704 ms)
                0 Done (response time: 605 ms)
                1 Done (response time: 1169 ms)
                0 Done (response time: 2638 ms)
                1 Done (response time: 2369 ms)
                0 Done (response time: 2826 ms)
                0 Done (response time: 2126 ms)
                1 Done (response time: 2677 ms)

                gcc main.c -o main -O0 -pthread && taskset -c 0 ./main 
                1 Done (response time: 4878 ms)
                0 Done (response time: 4891 ms)
                0 Done (response time: 2928 ms)
                1 Done (response time: 3876 ms)
                0 Done (response time: 931 ms)
                1 Done (response time: 1941 ms)
                0 Done (response time: 3417 ms)
                1 Done (response time: 4051 ms)
                0 Done (response time: 4502 ms)
                0 Done (response time: 3724 ms)
                1 Done (response time: 4304 ms)
                0 Done (response time: 1647 ms)
                1 Done (response time: 1206 ms)
                0 Done (response time: 3020 ms)
                1 Done (response time: 3181 ms)
                0 Done (response time: 3970 ms)

**Remarks**:

Longer reponse times when limiting to one processor, notabily some processes taking over 4000 ms. **Max response times**: `2826ms` and `4502ms`.

Parameters used:

``` C
int loads[NBTHREADS] = {4, 4};
int delays[NBTHREADS] = {1,2};
```

### Sched_RR, same priority

                gcc main.c -o main -O0 -pthread && ./main 
                0 Done (response time: 2890 ms)
                1 Done (response time: 2931 ms)
                0 Done (response time: 1642 ms)
                1 Done (response time: 80 ms)
                0 Done (response time: 404 ms)
                1 Done (response time: 1255 ms)
                0 Done (response time: 2626 ms)
                1 Done (response time: 2324 ms)
                0 Done (response time: 2706 ms)
                0 Done (response time: 2137 ms)
                1 Done (response time: 2758 ms)
                0 Done (response time: 975 ms)
                1 Done (response time: 755 ms)
                0 Done (response time: 2517 ms)
                1 Done (response time: 2049 ms)
                0 Done (response time: 2822 ms)
                0 Done (response time: 2588 ms)
                1 Done (response time: 2845 ms)

                gcc main.c -o main -O0 -pthread && taskset -c 0 ./main 
                0 Done (response time: 3051 ms)
                1 Done (response time: 3922 ms)
                0 Done (response time: 906 ms)
                1 Done (response time: 1934 ms)
                0 Done (response time: 3409 ms)
                1 Done (response time: 3858 ms)
                0 Done (response time: 4361 ms)
                0 Done (response time: 3773 ms)
                1 Done (response time: 4301 ms)
                0 Done (response time: 1552 ms)
                1 Done (response time: 1271 ms)
                0 Done (response time: 3051 ms)
                1 Done (response time: 3093 ms)
                0 Done (response time: 3978 ms)
                0 Done (response time: 4525 ms)
                1 Done (response time: 4708 ms)
                0 Done (response time: 2383 ms)
                1 Done (response time: 417 ms)
                0 Done (response time: 363 ms)
                1 Done (response time: 2352 ms)


Parameters:
``` C
int loads[NBTHREADS] = {4, 4};
int delays[NBTHREADS] = {1,2};
int sched_pri_vals[NBTHREADS] = {1,1};

```




### Sched_RR, different priorties

With different priorities (Task 0 higher priority):

                gcc main.c -o main -O0 -pthread && ./main 
                1 Done (response time: 2863 ms)
                0 Done (response time: 2881 ms)
                0 Done (response time: 1702 ms)
                1 Done (response time: 2712 ms)
                0 Done (response time: 484 ms)
                1 Done (response time: 1164 ms)
                0 Done (response time: 2534 ms)
                1 Done (response time: 2420 ms)
                0 Done (response time: 2851 ms)

                gcc main.c -o main -O0 -pthread && taskset -c 0 ./main 
                0 Done (response time: 1512 ms)
                0 Done (response time: 2420 ms)
                0 Done (response time: 2454 ms)
                1 Done (response time: 2856 ms)
                0 Done (response time: 2352 ms)
                0 Done (response time: 2440 ms)
                0 Done (response time: 2402 ms)
                1 Done (response time: 2849 ms)
                0 Done (response time: 2369 ms)
                0 Done (response time: 2440 ms)
                0 Done (response time: 2448 ms)
                1 Done (response time: 2883 ms)
                0 Done (response time: 2421 ms)
                0 Done (response time: 2403 ms)
                0 Done (response time: 2439 ms)

Parameters: 
``` C
int loads[NBTHREADS] = {4, 4};
int delays[NBTHREADS] = {1,2};
int sched_pri_vals[NBTHREADS] = {10,1};
```

# 3 Threads