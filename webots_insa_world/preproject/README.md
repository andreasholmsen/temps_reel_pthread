
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