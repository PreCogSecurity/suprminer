# suprminer (ccminer sp-mod)

[![CI](https://github.com/PreCogSecurity/suprminer/actions/workflows/ci.yml/badge.svg)](https://github.com/PreCogSecurity/suprminer/actions/workflows/ci.yml)
[![Security Scan](https://github.com/PreCogSecurity/suprminer/actions/workflows/ci.yml/badge.svg?branch=main&job=security-scan)](https://github.com/PreCogSecurity/suprminer/actions/workflows/ci.yml)
[![License: GPL-2.0](https://img.shields.io/badge/License-GPL%202.0-blue.svg)](https://opensource.org/licenses/GPL-2.0)

**suprminer** is an optimized multi-algorithm CUDA cryptocurrency miner, forked from ccminer with SP's x16r optimizations and no developer fee. This variant was originally released in August 2018 and has been updated with modern build tooling, CI/CD, and security hardening.

## Table of Contents

- [Features](#features)
- [Supported Algorithms](#supported-algorithms)
- [Architecture](#architecture)
- [Requirements](#requirements)
- [Building](#building)
- [Configuration](#configuration)
- [Environment Variables](#environment-variables)
- [Running](#running)
- [API](#api)
- [Monitoring](#monitoring)
- [Security](#security)
- [Contributing](#contributing)
- [License](#license)
- [Changelog](#changelog)

## Features

- **Multi-algorithm support**: 50+ mining algorithms implemented in CUDA
- **Stratum & GetWork protocols**: Full support for modern mining pools
- **Failover & load balancing**: Automatic pool switching with configurable policies
- **GPU monitoring**: Temperature, fan speed, power consumption, clock speeds via NVML
- **REST API**: Built-in HTTP API for monitoring and control (default port 4068)
- **Autotune**: Automatic intensity tuning for optimal performance
- **Cross-platform**: Linux (primary) and Windows support
- **No developer fee**: 0% dev fee, all hashrate goes to your pool

## Supported Algorithms

| Algorithm | Variants |
|-----------|----------|
| X11 family | x11, x11evo, x13, x14, x15, x16r, x16s, x17, phi, c11, s3, sib, veltor, timetravel, bitcore, hmq17 |
| SHA-256 | sha256d, sha256t, lbry |
| Scrypt | scrypt, scrypt-jane, scrypt-n |
| CryptoNight | cryptonight, cryptonight-light, cryptonight-heavy, cryptonight-v7, cryptonight-v8, cryptolight |
| Equihash | equihash (144/5, 192/7, 210/9) |
| Lyra2 | lyra2re, lyra2rev2, lyra2z, lyra2rev3 |
| Others | neoscrypt, quark, qubit, tribus, skunk, pentablake, skein, groestl, fugue, jha, heavy, myriad-groestl, polytimos, zr5, hsr, sm3, bastion, decred, vanilla, keccak, blake, bmw, cubehash, echo, luffa, shavite, simd, hamsi, whirlpool, shabal, streebog, haval, ripemd |

## Architecture

```
suprminer/
├── ccminer.cpp          # Main entry point, CLI parsing, global state
├── miner.h              # Core types, structures, global declarations
├── util.cpp             # Utilities: logging, JSON-RPC, networking, config
├── pools.cpp            # Pool management, failover, stratum protocol
├── api.cpp              # REST API server (monitoring/control)
├── bench.cpp            # Benchmark mode
├── stats.cpp            # Hashrate statistics
├── sysinfos.cpp         # System/GPU information
├── nvml.cpp             # NVML wrapper for GPU monitoring
├── nvsettings.cpp       # GPU clock/voltage control
├── cuda.cpp             # CUDA device management
├── algos.h              # Algorithm registration & dispatch
├── compat/              # Platform compatibility layer
│   ├── jansson/         # JSON library (vendored)
│   ├── curl-for-windows/# libcurl + OpenSSL (vendored, Windows)
│   ├── pthreads/        # pthreads for Windows
│   ├── nvapi/           # NVIDIA API for Windows
│   └── getopt/          # getopt_long for Windows
├── algo/                # Algorithm implementations (CUDA kernels)
│   ├── Algo256/         # 256-bit algorithms (x11 family, etc.)
│   ├── crypto/          # CryptoNight family
│   ├── equi/            # Equihash
│   ├── heavy/           # Heavy algorithms
│   ├── lyra2/           # Lyra2 family
│   ├── neoscrypt/       # NeoScrypt
│   ├── quark/           # Quark
│   ├── qubit/           # Qubit
│   ├── scrypt/          # Scrypt kernels
│   ├── sha256/          # SHA-256 variants
│   ├── sia/             # Sia
│   ├── skunk/           # Skunk
│   ├── sph/             # SPHlib hash functions
│   ├── tribus/          # Tribus
│   ├── x11/             # X11 family
│   ├── x13/             # X13
│   ├── x15/             # X15
│   ├── x16r/            # X16R/X16S
│   └── x17/             # X17
└── build system/
    ├── configure.ac     # Autoconf configuration
    ├── Makefile.am      # Automake configuration
    ├── autogen.sh       # Bootstrap script
    ├── configure.sh     # Configure wrapper
    ├── build.sh         # Linux build script
    └── build.cmd        # Windows MSBuild script
```

### Key Design Points

- **Single-process, multi-threaded**: One CPU thread per GPU, plus auxiliary threads (stratum, API, monitor)
- **Global state**: Heavy use of global variables for configuration and runtime state (legacy design)
- **CUDA kernels**: Each algorithm has hand-optimized CUDA kernels with autotune support
- **Stratum implementation**: Custom stratum protocol handler with job queue and difficulty tracking
- **JSON-RPC**: Uses Jansson for JSON parsing, libcurl for HTTP/Stratum transport

## Requirements

### Linux
- **OS**: Ubuntu 20.04+, Debian 11+, Fedora 35+, RHEL 8+, Arch Linux
- **GPU**: NVIDIA GPU with Compute Capability 5.0+ (Maxwell) — 6.0+ (Pascal) recommended
- **CUDA Toolkit**: 11.0+ (12.2 recommended)
- **NVIDIA Driver**: 450+ (525+ recommended)
- **Build tools**: gcc/g++ 9+, make, autoconf, automake, libtool, pkg-config
- **Libraries**: libcurl4-openssl-dev, libjansson-dev, libssl-dev, zlib1g-dev

### Windows
- **OS**: Windows 10/11 (64-bit)
- **GPU**: NVIDIA GPU with Compute Capability 5.0+
- **CUDA Toolkit**: 11.0+ (12.2 recommended)
- **Visual Studio**: 2019/2022 with C++ workload
- **Dependencies**: Prebuilt in `compat/curl-for-windows/` (OpenSSL, libcurl, pthreads)

## Building

### Linux (Autotools)

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get update && sudo apt-get install -y \
  build-essential autoconf automake libtool pkg-config \
  libcurl4-openssl-dev libjansson-dev libssl-dev zlib1g-dev \
  cuda-toolkit-12-2

# Clone with submodules
git clone --recurse-submodules https://github.com/PreCogSecurity/suprminer.git
cd suprminer

# Build
./autogen.sh
./configure.sh
make -j$(nproc)

# Binary: ./ccminer
```

#### Configure Options

```bash
./configure.sh --help
# Key options:
#   --with-cuda=PATH       CUDA installation prefix [/usr/local/cuda]
#   --with-nvml=PATH       NVML library path [/usr/lib]
#   --enable-openmp        Enable OpenMP support
```

### Windows (Visual Studio)

```cmd
# Clone with submodules
git clone --recurse-submodules https://github.com/PreCogSecurity/suprminer.git
cd suprminer

# Build (Release x64)
msbuild ccminer.vcxproj /m /p:Configuration=Release /p:Platform=x64

# Binary: x64\Release\ccminer.exe
```

### Docker (Experimental)

```dockerfile
# Dockerfile.example
FROM nvidia/cuda:12.2-devel-ubuntu22.04
RUN apt-get update && apt-get install -y \
    build-essential autoconf automake libtool pkg-config \
    libcurl4-openssl-dev libjansson-dev libssl-dev zlib1g-dev \
    git
WORKDIR /suprminer
COPY . .
RUN ./autogen.sh && ./configure.sh && make -j$(nproc)
ENTRYPOINT ["./ccminer"]
```

## Configuration

suprminer uses a JSON configuration file. By default it looks for:
- **Linux**: `~/.ccminer/ccminer.conf`
- **Windows**: `%APPDATA%\ccminer\ccminer.conf`

### Configuration File Structure

```json
{
  "_comment": "All keys correspond to long command-line options (ccminer --help)",

  "algo": "x16r",
  "intensity": 22,

  "api-bind": "127.0.0.1:4068",
  "api-remote": false,
  "api-allow": "127.0.0.1/24",

  "max-log-rate": 60,
  "quiet": false,
  "debug": false,
  "protocol": false,
  "cpu-priority": 3,

  "url": "stratum+tcp://pool.example.com:3333",
  "user": "YOUR_WALLET_ADDRESS.WORKER_NAME",
  "pass": "x",

  "pools": [
    {
      "name": "Primary Pool",
      "url": "stratum+tcp://pool1.example.com:3333",
      "user": "WALLET.WORKER1",
      "pass": "x",
      "max-diff": 0.0,
      "scantime": 10,
      "time-limit": 0,
      "disabled": false
    },
    {
      "name": "Failover Pool",
      "url": "stratum+tcp://pool2.example.com:3333",
      "user": "WALLET.WORKER2",
      "pass": "x",
      "time-limit": 300
    }
  ],

  "timeout": 60,
  "retries": -1,
  "failover-pause": 30,
  "no-gbt": true,
  "no-longpoll": false,
  "no-stratum": false
}
```

### Configuration Priority

1. Command-line arguments (highest)
2. Configuration file (`-c /path/to/config.json`)
3. Default configuration file locations
4. Built-in defaults (lowest)

## Environment Variables

| Variable | Description | Default |
|----------|-------------|---------|
| `CUDA_VISIBLE_DEVICES` | Comma-separated list of GPU IDs to use | All GPUs |
| `CCMINER_CONFIG` | Path to configuration file | Auto-detect |
| `HOME` | User home directory (Linux config path) | System-dependent |
| `APPDATA` | Application data directory (Windows config path) | System-dependent |
| `TZ` | Timezone for log timestamps | System timezone |
| `NVIDIA_VISIBLE_DEVICES` | NVIDIA container toolkit GPU selection | All GPUs |

## Running

### Basic Usage

```bash
# Single pool, command-line args
./ccminer -a x16r -o stratum+tcp://pool.example.com:3333 -u WALLET.WORKER -p x

# Using config file
./ccminer -c /path/to/config.json

# Benchmark mode (no pool connection)
./ccminer -a x16r --benchmark

# List supported algorithms
./ccminer --help
```

### Common Options

| Option | Description |
|--------|-------------|
| `-a, --algo=ALGO` | Mining algorithm (default: auto-detect) |
| `-o, --url=URL` | Pool URL (stratum+tcp://, http://) |
| `-u, --user=USER` | Username/wallet address |
| `-p, --pass=PASS` | Password (usually `x`) |
| `-c, --config=FILE` | Configuration file path |
| `-d, --devices=IDS` | Comma-separated GPU device IDs (e.g., `0,1`) |
| `-i, --intensity=N` | Mining intensity (8-25, default: auto) |
| `--api-bind=HOST:PORT` | API bind address (default: 127.0.0.1:4068) |
| `--api-remote` | Allow remote API connections (default: false) |
| `--api-allow=CIDR` | Allowed IP ranges for API (default: 127.0.0.1/24) |
| `-D, --debug` | Enable debug logging |
| `-P, --protocol` | Log protocol-level communication |
| `-q, --quiet` | Suppress non-error output |
| `--benchmark` | Run benchmark and exit |
| `--time-limit=SEC` | Stop mining after N seconds |
| `--shares-limit=N` | Stop after N accepted shares |

### Pool Failover

Multiple pools can be configured in the JSON file under `"pools"`. The miner will:
1. Start with the first enabled pool
2. On connection failure, switch to the next enabled pool after `failover-pause` seconds
3. Rotate back to higher-priority pools when they become available

## API

The built-in REST API provides monitoring and control capabilities.

### Enable API

```json
{
  "api-bind": "0.0.0.0:4068",
  "api-remote": true,
  "api-allow": "192.168.1.0/24"
}
```

### Endpoints

| Endpoint | Description | Privilege |
|----------|-------------|-----------|
| `GET /` | API version & status | Read |
| `GET /summary` | Global mining summary | Read |
| `GET /pools` | Pool status & statistics | Read |
| `GET /devs` | GPU device details | Read |
| `GET /stats` | Hashrate statistics | Read |
| `GET /config` | Current configuration | Read |
| `POST /control` | Control commands (restart, quit) | Write |

### Example API Calls

```bash
# Summary
curl http://127.0.0.1:4068/summary

# GPU details
curl http://127.0.0.1:4068/devs

# Restart mining
curl -X POST http://127.0.0.1:4068/control -d '{"command":"restart"}'
```

### API Groups

Commands are organized into privilege groups (A-Z):
- **R** (Read): `summary`, `pools`, `devs`, `stats`, `config`, `version`
- **W** (Write): `restart`, `quit`, `addpool`, `removepool`, `enablepool`, `disablepool`

Configure allowed groups per IP range in `api-allow` with group suffix: `192.168.1.0/24:R`

## Monitoring

### NVML GPU Monitoring

When built with NVML support (`--with-nvml`), the miner reports:
- GPU temperature (°C)
- Fan speed (%)
- Power consumption (mW)
- Core/memory clock speeds (MHz)
- Power limit (% or mW)
- GPU utilization

### Logging

Log levels (controlled by `--debug`, `--protocol`, `--max-log-rate`):
- `ERROR`: Critical failures
- `WARNING`: Recoverable issues
- `NOTICE`: Important events (pool connect, block found)
- `INFO`: General progress (accepted shares)
- `DEBUG`: Verbose debugging (with `-D`)

### External Monitoring

- **Prometheus**: Use API `/summary` + `/devs` endpoints
- **Grafana**: Dashboard templates available in `contrib/grafana/`
- **Custom**: Parse JSON API output

## Security

### Threat Model

suprminer is designed to run in trusted environments (dedicated mining rigs). It:
- Connects to mining pools over TCP (Stratum) or HTTP (GetWork)
- Exposes a local REST API (bind to `127.0.0.1` by default)
- Reads/writes configuration files in user directory
- Loads CUDA kernels at runtime

### Security Practices

1. **Bind API to localhost only** (`api-bind: 127.0.0.1:4068`) unless remote monitoring is required
2. **Use `api-allow` CIDR restrictions** for remote API access
3. **Never commit real credentials** to version control (use `.env` or config file outside repo)
4. **Run as non-root user** on Linux
5. **Keep CUDA driver & toolkit updated** for security patches
6. **Verify pool URLs** — avoid typo-squatted pool domains

### Reporting Vulnerabilities

See [SECURITY.md](SECURITY.md) for vulnerability disclosure process.

### Known Security Considerations

- **No TLS for Stratum**: Standard Stratum protocol is unencrypted. Use VPN/tunnel for untrusted networks.
- **API authentication**: API uses IP-based ACL only. No token/auth by default.
- **Configuration files**: Store wallet credentials in config file with restricted permissions (`chmod 600`).
- **Vendored dependencies**: OpenSSL, libcurl, Jansson are vendored. Update via submodule pulls.

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Development Setup

```bash
# Install development tools
sudo apt-get install -y clang-format cppcheck catch2

# Format code
find . -name "*.cpp" -o -name "*.h" -o -name "*.cu" -o -name "*.c" -o -name "*.hpp" | \
  grep -v compat/ | xargs clang-format -i

# Static analysis
cppcheck --enable=warning,style,performance,portability -I compat -I . .
```

### Pull Request Process

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make changes with tests if applicable
4. Run `clang-format` and `cppcheck`
5. Ensure CI passes
6. Submit PR with clear description

## License

suprminer is licensed under the **GNU General Public License v2.0**.

```
This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
more details.
```

See [LICENSE.txt](LICENSE.txt) for full license text.

### Third-Party Licenses

| Component | License | Location |
|-----------|---------|----------|
| Jansson | MIT | `compat/jansson/LICENSE` |
| libcurl | curl license | `compat/curl-for-windows/` |
| OpenSSL | Apache-2.0 | `compat/curl-for-windows/openssl/` |
| pthreads-w32 | LGPL-2.1 | `compat/pthreads/` |
| SPHLib | Public Domain | `sph/` |
| CUDA kernels | Various (see headers) | `algo/*/` |

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for version history.

### Recent Changes

- **2026-10-10**: Modern CI/CD, linting, security hardening, documentation overhaul
- **2018-08**: Original SP-mod release (x16r optimization, no dev fee)

## Support

- **Issues**: [GitHub Issues](https://github.com/PreCogSecurity/suprminer/issues)
- **Discussions**: [GitHub Discussions](https://github.com/PreCogSecurity/suprminer/discussions)
- **Security**: See [SECURITY.md](SECURITY.md)

## Acknowledgments

- Original ccminer by tpruvot, pooler, Jeff Garzik
- SP's x16r optimizations
- NVIDIA for CUDA toolkit
- All algorithm contributors in the cryptocurrency mining community