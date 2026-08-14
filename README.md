# Real-Time Systems Labs — POSIX Threads & Webots Robot Control

> Pthreads lab work: scheduling/priority experiments and a multi-threaded real-time controller for a simulated Webots robot.

## About

Lab work for the *Systèmes Temps Réel* (Real-Time Systems) course, 4th year IR, INSA Toulouse, spring 2026. The project explores POSIX threads programming on Linux in two stages. First, a "preproject" measures thread response times under the `SCHED_RR` real-time scheduler with different priorities, CPU affinities (multi-core vs. pinned to a single core), a priority-inversion scenario, and the Priority Inheritance Protocol — measured results are recorded in [`webots_insa_world/preproject/README.md`](webots_insa_world/preproject/README.md). Second, the main project is a multi-threaded TCP client (`client.c`) that drives a robot simulated in Webots: periodic tasks (implemented with `clock_nanosleep` absolute sleeps) poll the battery level and distance sensors and drive the robot forward, while semaphore-triggered tasks handle obstacle-avoidance turns and battery recharging; mutexes protect the shared socket and motor access. The Webots side (`webots_insa_world/`) contains the simulation world and a robot controller that acts as a TCP server (port 10020) accepting simple text commands (`M,<left>,<right>` for motors, `B` for battery, `S` for distance sensors). The lab subject is included as `tp_pthreads.pdf`.

## Tech stack

- C (C99), POSIX threads (pthreads), semaphores, mutexes, `clock_nanosleep`
- Linux real-time scheduling (`SCHED_RR`, `pthread_setschedparam`, `taskset`)
- TCP/IP sockets (client/server between controller and Webots)
- Webots robot simulator (world + C controller)
- Python (small scripts to average measured response times)
- Bash helper scripts (`ps_enable.sh` / `ps_disable.sh`) to toggle CPU frequency-scaling settings for reproducible timing measurements

## Build & run

Main client (from the repository root):

```sh
gcc -Wall client.c -o client
./client
```

The client connects to `127.0.0.1:10020`, so the Webots simulation (world in `webots_insa_world/worlds/`, controller `my_controller` acting as the TCP server) must be running first. The controller is built by Webots' standard controller build system (requires `WEBOTS_HOME`).

Preproject experiments (from `webots_insa_world/preproject/`):

```sh
make run          # 3-thread experiment, all cores
make run_single   # 3-thread experiment pinned to core 0 (taskset -c 0)
make run_2        # 2-thread experiment, all cores
make run_2_single # 2-thread experiment pinned to core 0
```

Note: setting `SCHED_RR` priorities typically requires root or `CAP_SYS_NICE`.

## Project structure

```
client.c                       # Multi-threaded real-time TCP client controlling the robot
tp_pthreads.pdf                # Lab subject
ps_enable.sh / ps_disable.sh   # CPU frequency-scaling toggles for timing experiments
webots_insa_world/
  worlds/                      # Webots world (insa_tp_temps_reel_2025.wbt)
  controllers/my_controller/   # Webots C controller (TCP server side)
  preproject/                  # SCHED_RR scheduling experiments + measured results
```
