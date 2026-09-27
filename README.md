#  Docker PID Limit & Zombie Simulator

This repository demonstrates what happens when a Docker container runs out of Process IDs (PIDs) due to orphaned "zombie" processes. It uses a small C program to intentionally neglect child processes, causing them to stack up until the container hits a hard process limit and crashes.

It proves why running apps as `PID 1` in Docker can be dangerous without an init system.

##  How to Run the Simulation

### 1. The Crash (Without an Init System)
Build the image and run it with a strict limit of 10 processes:

```bash
docker build -t zombie-sim .
docker run --rm -it --pids-limit 10 --name zombie-test zombie-sim# zombie-factory
```
Watch the terminal spawn children until fork() fails with a Resource temporarily unavailable error.

## 2. Inspect the Zombies
While the first terminal is paused on the crash screen, open a new terminal pane and inspect the process table:
```
docker exec -it zombie-test ps -ef
```

You will see the dead child processes tagged as <defunct> (zombies) eating up your process limit.


## 3. The Fix (With --init)
Docker has a built-in fix. Run the exact same container, but add the --init flag. This injects a tiny init system (like tini) as PID 1, which automatically reaps the dead children.

```
docker run --rm -it --init --pids-limit 10 --name zombie-test zombie-sim
```
The process table will remain clean, and the container will run indefinitely without crashing.

What's inside?
zombie.c: The C program that forks children, dynamically renames them to zombie_proc_X, and forces them to exit without a parent waiting.

Dockerfile: Compiles the C code and runs it directly as PID 1 using Alpine Linux.
