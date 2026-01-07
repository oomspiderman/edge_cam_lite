# EdgeCam-Lite (v2)

A small, disciplined user-space pipeline that exercises **io_uring** for high-throughput I/O and runs untrusted image processing code as **WebAssembly (WASM)**. Designed to run on Ubuntu 22.04 in a VM.

## What it does
- Reads fixed-size image tiles from `img_samples/in/`.
- Batches reads/writes using **io_uring** to reduce syscalls/context switches.
- Calls a client algorithm `process(uint8_t* buf, size_t len, Params p)` compiled to **wasm32-wasi** via **clang** and executed with **Wasmtime**.
- Enforces **timeouts** and **bounded memory** for algorithm execution.
- Exposes **status/metrics** (tiles/s, p50/p95 latency, backlog, errors) over a UNIX socket (or HTTP).
- Includes simple **fault-injection** scripts (kill algorithm host, drop tiles) to demonstrate recovery.

## Repo layout
```
edgecam-lite/
├─ edge_host/
│  ├─ main.c
│  ├─ io_uring_io.c/.h
│  ├─ wasm_host.c/.h
│  ├─ metrics.c/.h
│  ├─ control.c/.h
│  └─ CMakeLists.txt
├─ client_algos/
│  ├─ grayscale/plugin.c
│  └─ build_wasm.sh
├─ test_scripts/
├─ img_samples/{in,out}/
├─ docs/
│  └─ DESIGN_architecture.md
└─ README.md

edge_host    => edge host service: io_uring pipeline + WASM runner + metrics + control
client_algos => client algorithms compiled to wasm32-wasi (e.g., grayscale)
test_scripts => run demo, feed tiles, fault injection, benchmarks
```

## Quick start (Ubuntu 22.04)
```bash
sudo apt update
sudo apt install -y build-essential cmake git pkg-config liburing-dev clang lld curl   valgrind strace linux-tools-common linux-tools-$(uname -r)
# Wasmtime runtime:
curl -sSf https://wasmtime.dev/install.sh | bash
exec $SHELL  # reload PATH for wasmtime
```

## Build the host
```bash
cd edge_host
cmake -S . -B build && cmake --build build -j
```

## Build a sample client algorithm (WASM)
```bash
cd ../client_algos
./build_wasm.sh grayscale/grayscale.wasm
```

## Run the demo
```bash
# prepare input/output dirs
mkdir -p ../img_samples/in ../img_samples/out
# (put some test tiles/images into img_samples/in)
../test_scripts/run_local.sh   # starts the host, processes tiles, prints metrics
```

## Design sketch
- **Producer**: file feeder (tiles) → **io_uring** batched reads → bounded queue.
- **Processor**: WASM host (Wasmtime). On timeout/stall, restart; enforce memory limit.
- **Writer**: **io_uring** batched writes → `img_samples/out/`
- **Control**: UNIX socket `status` → JSON (backlog, tiles/s, p50/p95, errors); `reload-algo` hot-swap.
- **Faults**: kill algorithm host, drop tiles, delay injector; system should recover cleanly.

## Notes
Kernel driver work is intentionally out-of-scope here. Later? Create a separate **kernel micro-lab** repo (UIO fake device or generic netlink shim).