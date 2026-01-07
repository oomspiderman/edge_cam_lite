# edge_host

Edge host service responsible for ingesting image tiles, running client algorithms (WASM),
and writing processed outputs. Uses an event-driven pipeline with io_uring and exposes a
minimal control/status interface.

## Contents
- `main.c`: program entry and pipeline orchestration
- `io_uring_io.c/.h`: batched asynchronous file/socket I/O
- `wasm_host.c/.h`: WASM runtime embedding and ABI bridge
- `metrics.c/.h`: counters, p50/p95 latency, throughput
- `control.c/.h`: UNIX-socket status/commands (JSON)
- `CMakeLists.txt`: build rules

## Build
```bash
cmake -S . -B build && cmake --build build -j
```

## Run
The binary expects input tiles under `../img_samples/in` and writes to `../img_samples/out` by default.
