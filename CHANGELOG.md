# Changelog

All notable changes to suprminer will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- GitHub Actions CI/CD pipeline with linting, building, and security scanning
- clang-format configuration for consistent code style
- cppcheck configuration for static analysis
- Comprehensive README with architecture, configuration, and API documentation
- `.env.example` with documented environment variables
- SECURITY.md with vulnerability reporting and security practices
- CONTRIBUTING.md with development guidelines
- Unit test framework with tests for utilities, algorithms, and config parsing
- Input validation for config parsing (URL format, numeric ranges, string lengths)
- Template config files (ccminer.conf, pools.conf) without hardcoded credentials
- dependabot.yml for automated dependency updates

### Changed
- Removed hardcoded credentials from ccminer.conf and pools.conf
- Enhanced parse_config() with validation for intensity, timeout, scantime, URL format
- Enhanced parse_pool_array() with validation for pool-specific options

### Security
- Added secret scanning in CI pipeline
- Added Trivy vulnerability scanning
- Default API bind address restricted to localhost (127.0.0.1)
- API CIDR allowlist support

## [1.6.0] - 2018-08-15

### Added
- SP's x16r algorithm optimizations
- No developer fee (0% dev fee)
- Support for 50+ mining algorithms
- Stratum and GetWork protocol support
- Pool failover and load balancing
- NVML GPU monitoring (temperature, fan, power, clocks)
- REST API for monitoring and control
- Autotune for automatic intensity optimization
- Cross-platform support (Linux, Windows)

### Algorithms Supported
- X11 family: x11, x11evo, x13, x14, x15, x16r, x16s, x17, phi, c11, s3, sib, veltor, timetravel, bitcore, hmq17
- SHA-256: sha256d, sha256t, lbry
- Scrypt: scrypt, scrypt-jane, scrypt-n
- CryptoNight: cryptonight, cryptonight-light, cryptonight-heavy, cryptonight-v7, cryptonight-v8, cryptolight
- Equihash: equihash (144/5, 192/7, 210/9)
- Lyra2: lyra2re, lyra2rev2, lyra2z, lyra2rev3
- Others: neoscrypt, quark, qubit, tribus, skunk, pentablake, skein, groestl, fugue, jha, heavy, myriad-groestl, polytimos, zr5, hsr, sm3, bastion, decred, vanilla, keccak, blake, bmw, cubehash, echo, luffa, shavite, simd, hamsi, whirlpool, shabal, streebog, haval, ripemd

### Changed
- Forked from ccminer (tpruvot branch)
- Optimized x16r algorithm for maximum performance
- Removed all developer fees

## [1.5.0] - 2017-12-10

### Added
- Initial ccminer release with multi-algorithm support
- CUDA kernel implementations for major algorithms
- Basic stratum protocol implementation
- Simple configuration file support

---

## Version History Summary

| Version | Date | Key Changes |
|---------|------|-------------|
| 1.6.0 | 2018-08-15 | SP-mod x16r optimizations, no dev fee |
| 1.5.0 | 2017-12-10 | Initial multi-algorithm ccminer release |
| Unreleased | 2026-10-10 | Modern CI/CD, security hardening, documentation, testing |

---

## Upgrade Guide

### From 1.5.x to 1.6.x

1. **Configuration**: Config file format unchanged, but new options available
2. **Algorithms**: x16r now uses SP's optimized implementation
3. **Performance**: Expect 5-15% hashrate improvement on x16r
4. **No dev fee**: All hashrate goes to your pool

### From 1.6.x to Next Release

1. **Config files**: Use new template files (ccminer.conf, pools.conf)
2. **Environment**: Consider using `.env` file for configuration
3. **API**: Default bind address changed to 127.0.0.1 for security
4. **CI/CD**: GitHub Actions workflow available for automated builds

---

## Contributors

- **SP**: x16r algorithm optimizations
- **tpruvot**: Original ccminer author and maintainer
- **pooler**: Original cpuminer author
- **Jeff Garzik**: Original Bitcoin miner author
- **PreCogSecurity**: Modern tooling, CI/CD, security hardening, documentation

---

*For detailed commit history, see `git log --oneline --graph`*