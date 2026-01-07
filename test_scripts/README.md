# test_scripts

Helper scripts to run and exercise the system locally.

## Scripts
- `run_local.sh`: start the edge host with default settings and process tiles from `img_samples/in`
- `feed_tiles.sh`: copy or generate tiles into `img_samples/in` to simulate capture windows
- `fault_inject.sh`: introduce delays, drop tiles, or restart the algorithm host to test recovery

Scripts are POSIX shell where possible. Adjust paths as needed for your environment.
