# client_algos

Client-supplied image processing algorithms compiled to **wasm32-wasi** and executed by the edge host.
Each algorithm must export a function with the signature:

```c
int process(uint8_t* buf, size_t len /*, optional params */);
```

The host provides a mutable buffer containing the tile/image; the algorithm transforms it in place.
Execution is bounded by time and memory limits enforced by the host.

## Layout
- `grayscale/plugin.c`: example algorithm
- `build_wasm.sh`: helper script to compile C to `wasm32-wasi` (requires `clang`)

## Build example
```bash
./build_wasm.sh grayscale/grayscale.wasm
```
