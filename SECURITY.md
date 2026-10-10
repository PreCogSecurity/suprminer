# Security Policy

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 1.6.x   | :white_check_mark: |
| < 1.6   | :x:                |

## Reporting a Vulnerability

We take security vulnerabilities seriously. If you discover a security issue, please report it responsibly.

### How to Report

**DO NOT** create a public GitHub issue for security vulnerabilities.

Instead, please report vulnerabilities via:

1. **Email**: security@precogsecurity.com (PGP key available on request)
2. **GitHub Security Advisories**: Use the "Report a vulnerability" tab on the repository

### What to Include

Please provide as much detail as possible:

- Description of the vulnerability
- Steps to reproduce
- Potential impact
- Affected versions
- Any proof-of-concept code (if safe to share)
- Suggested fix (if known)

### Response Timeline

- **Acknowledgment**: Within 48 hours
- **Initial Assessment**: Within 7 days
- **Fix Development**: Within 30 days (for critical issues)
- **Public Disclosure**: After fix is released, coordinated with reporter

## Security Best Practices

### For Users

1. **Never run as root** - Run suprminer as a dedicated non-privileged user
2. **Bind API to localhost** - Keep `api-bind: 127.0.0.1:4068` unless remote monitoring is required
3. **Restrict API access** - Use `api-allow` CIDR restrictions for any remote access
4. **Use strong pool credentials** - Unique passwords per pool, rotate regularly
5. **Keep software updated** - Update CUDA drivers, OS, and suprminer regularly
6. **Verify downloads** - Check GPG signatures on release binaries
7. **Use VPN/tunnel** - Stratum protocol is unencrypted; use VPN on untrusted networks
8. **Secure config files** - `chmod 600 ccminer.conf` to protect credentials
9. **Monitor API access** - Check logs for unauthorized API access attempts
10. **Isolate mining rigs** - Run on dedicated hardware/network segment

### For Developers

1. **Input validation** - All external input (config, API, network) must be validated
2. **Bounds checking** - Use safe string functions, check array bounds
3. **Memory safety** - Check malloc/realloc returns, free on error paths
4. **No hardcoded secrets** - Credentials must come from config/environment only
5. **Dependency updates** - Keep vendored dependencies (OpenSSL, curl, Jansson) current
6. **Static analysis** - Run cppcheck and clang-tidy before committing
7. **Fuzzing** - Test JSON parsing with malformed inputs
8. **Code review** - All changes require review before merge

## Threat Model

### Assets to Protect

- Wallet credentials (pool username/password)
- Mining hashrate (theft via pool hijacking)
- GPU hardware (overclocking damage)
- System integrity (remote code execution)
- Network traffic (credential sniffing)

### Attack Vectors

| Vector | Likelihood | Impact | Mitigation |
|--------|------------|--------|------------|
| Malicious pool server | Medium | High | URL validation, certificate pinning |
| Config file injection | Low | High | Input validation, length limits |
| API exposure | Medium | Medium | Bind to localhost, CIDR restrictions |
| Stratum MITM | High | Medium | VPN/tunnel, SSL pools |
| CUDA kernel exploit | Low | High | Driver updates, sandbox |
| Supply chain (deps) | Medium | High | Dependency pinning, verification |

### Trust Boundaries

```
┌─────────────────────────────────────────────────────────────┐
│                    Mining Rig (Trusted)                     │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │  suprminer  │  │   Config    │  │      GPU Driver     │  │
│  │  (Process)  │  │   Files     │  │   (Kernel Module)   │  │
│  └──────┬──────┘  └──────┬──────┘  └──────────┬──────────┘  │
│         │                │                    │             │
│         ▼                ▼                    ▼             │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              Local Network (Semi-Trusted)           │   │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  │   │
│  │  │   API       │  │  Monitoring │  │  Firewall   │  │   │
│  │  │  (127.0.0.1)│  │  (Prometheus)│  │  (iptables) │  │   │
│  │  └─────────────┘  └─────────────┘  └─────────────┘  │   │
│  └──────────────────────────┬──────────────────────────┘   │
│                             │                                │
│                             ▼                                │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              Internet (Untrusted)                   │   │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  │   │
│  │  │   Pool      │  │  DNS        │  │  Attackers  │  │   │
│  │  │  (Stratum)  │  │  (Resolver) │  │  (MITM)     │  │   │
│  │  └─────────────┘  └─────────────┘  └─────────────┘  │   │
│  └─────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
```

## Security Features

### Implemented

- ✅ Input validation on config parsing (URL format, numeric ranges, string lengths)
- ✅ API bind address restriction (default: 127.0.0.1)
- ✅ API CIDR allowlist (`api-allow`)
- ✅ API privilege groups (Read/Write separation)
- ✅ No hardcoded credentials in source
- ✅ Template config files with placeholders
- ✅ `.env.example` for environment-based config
- ✅ Static analysis in CI (cppcheck, clang-format)
- ✅ Dependency vulnerability scanning (Trivy)
- ✅ Secret scanning in CI

### Planned

- [ ] TLS support for Stratum (stratum+ssl://)
- [ ] API authentication tokens
- [ ] Rate limiting on API
- [ ] Certificate pinning for pools
- [ ] Sandboxed CUDA kernel execution
- [ ] Signed releases with GPG
- [ ] SBOM (Software Bill of Materials) generation

## Vulnerability History

| CVE | Date | Severity | Description | Fixed |
|-----|------|----------|-------------|-------|
| None reported | - | - | - | - |

## Security Contacts

- **Primary**: security@precogsecurity.com
- **Backup**: CTO@precogsecurity.com
- **PGP**: Available on request

## Acknowledgments

We thank the following researchers for responsible disclosure:

- (None yet - be the first!)

---

*This policy is based on industry best practices and will be updated as the project evolves.*