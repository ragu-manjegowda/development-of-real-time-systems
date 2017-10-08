# Coursera Real-Time systems

## Used FreeRTOS Simulator for projects (Linux Ubuntu 16.04)

Reference - https://github.com/megakilo/FreeRTOS-Sim.git

Might need gcc multilib support to compile (if using 64 bit machine).
- $ sudo apt-get install gcc-multilib

Directory description
- Source: FreeRTOS kernel and POSIX simulator source files
- Project: the project directory that includes main() and FreeRTOS settings for
  the POSIX port
- Demo: demo tasks from the official FreeRTOS release

### Replace Project/main.c with main.c inside Assignments folder to test.

### Build
- make

### Execute
- ./FreeRTOS-Sim

### Clean
- make clean
