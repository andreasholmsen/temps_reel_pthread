
# Preproject

Parameters used:

``` C
int loads[NBTHREADS] = {4, 4};
int delays[NBTHREADS] = {1,2};
```

**Multiple processors**
- Process 0 avg time:  832.7037037037037
- Process 1 avg time:  865.0277777777778

**Single processor**
- Process 0 avg time:  1187.2096774193549
- Process 1 avg time:  1384.142857142857



### Sched_RR, same priority

Parameters:
``` C
int loads[NBTHREADS] = {4, 4};
int delays[NBTHREADS] = {1,2};
int sched_pri_vals[NBTHREADS] = {1,1};

```
**Multiple processors**
- Process 0 avg time:  869.0
- Process 1 avg time:  808.170731707317

**Single processor**
- Process 0 avg time:  1209.1470588235295
- Process 1 avg time:  1381.5333333333333



### Sched_RR, different priorties

With different priorities (Task 1 higher priority):

Parameters: 
``` C
int loads[NBTHREADS] = {4, 4};
int delays[NBTHREADS] = {1,2};
int sched_pri_vals[NBTHREADS] = {1,10};
```

**Multiple processors**
- Process 0 avg time:  844.989010989011
- Process 1 avg time:  821.516129032258

**Single processor**
- Process 0 avg time:  1195.5416666666667
- Process 1 avg time:  1049.8775510204082



# 3 Threads

Parameters: 
``` C
int loads[NBTHREADS] = {4, 4,4};
int delays[NBTHREADS] = {1,2,2};
int sched_pri_vals[NBTHREADS] = {1,10,5};
```

**Multiple processors**
- Process 0 avg time:  2111.4444444444443
- Process 1 avg time:  1059.6527777777778
- Process 2 avg time:  862.7837837837837

**Single processor**
- Process 0 avg time:  1129.051282051282
- Process 1 avg time:  1202.7792207792209
- Process 2 avg time:  1096.7692307692307



### 3 threads, priority inversing

Parameters: 
``` C
int loads[NBTHREADS] = {4, 4, 4};
int delays[NBTHREADS] = {2,2, 1};
int sched_pri_vals[NBTHREADS] = {1,10, 5};
```

**Single processor**
- Process 0 avg time:  1089.6666666666667
- Process 1 avg time:  1667.8333333333333
- Process 2 avg time:  1091.7037037037037


### 3 threads, Priority Inheritance Protocol

Parameters: 
``` C
int loads[NBTHREADS] = {4, 4, 4};
int delays[NBTHREADS] = {2,2, 1};
int sched_pri_vals[NBTHREADS] = {1,10, 5};
```

**Single processor**
- Process 0 avg time:  1145.6
- Process 1 avg time:  1186.32
- Process 2 avg time:  1125.2