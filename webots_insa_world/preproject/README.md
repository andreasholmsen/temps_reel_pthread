
# Preproject

                gcc main.c -o main -O0 -pthread && ./main 
                0 Done (response time: 1374 ms)
                1 Done (response time: 1399 ms)
                0 Done (response time: 219 ms)
                1 Done (response time: 15 ms)
                0 Done (response time: 1205 ms)
                1 Done (response time: 1211 ms)
                0 Done (response time: 1410 ms)
                0 Done (response time: 485 ms)
                1 Done (response time: 1309 ms)
                0 Done (response time: 1173 ms)
                1 Done (response time: 1067 ms)
                0 Done (response time: 1425 ms)
                0 Done (response time: 659 ms)
                1 Done (response time: 1312 ms)
                0 Done (response time: 1183 ms)
                1 Done (response time: 765 ms)
                0 Done (response time: 1272 ms)
                0 Done (response time: 940 ms)
                1 Done (response time: 1413 ms)
                0 Done (response time: 1187 ms)
                ^Cmake: *** [Makefile:2: run] Interrupt

                gcc main.c -o main -O0 -pthread && taskset -c 0 ./main 
                1 Done (response time: 2478 ms)
                0 Done (response time: 2479 ms)
                0 Done (response time: 355 ms)
                1 Done (response time: 28 ms)
                0 Done (response time: 1216 ms)
                1 Done (response time: 2000 ms)
                0 Done (response time: 2163 ms)
                0 Done (response time: 612 ms)
                1 Done (response time: 1456 ms)
                0 Done (response time: 1173 ms)
                1 Done (response time: 1709 ms)
                0 Done (response time: 2042 ms)
                0 Done (response time: 997 ms)
                1 Done (response time: 1670 ms)
                0 Done (response time: 1190 ms)
                1 Done (response time: 1334 ms)
                0 Done (response time: 1861 ms)
                0 Done (response time: 1477 ms)
                1 Done (response time: 1952 ms)
                0 Done (response time: 1203 ms)
                1 Done (response time: 956 ms)
                0 Done (response time: 1680 ms)
                0 Done (response time: 1843 ms)
                1 Done (response time: 2116 ms)
                ^Cmake: *** [Makefile:5: run_single] Interrupt

**Remarks**:

Longer reponse times when limiting to one processor, notabily some processes taking over 2100 ms. **Max response times**: `1425ms` and `2163ms`.

Parameters used:

``` C
int loads[NBTHREADS] = {2, 2};
int delays[NBTHREADS] = {1,2};
```