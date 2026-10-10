# Contributing to suprminer

Thank you for your interest in contributing to suprminer! This document provides guidelines for contributing to the project.

## Code of Conduct

By participating in this project, you agree to abide by our [Code of Conduct](CODE_OF_CONDUCT.md). Please be respectful and constructive in all interactions.

## How to Contribute

### Reporting Bugs

1. Check existing issues first to avoid duplicates
2. Use the bug report template
3. Include:
   - OS and version
   - GPU model and driver version
   - CUDA toolkit version
   - Steps to reproduce
   - Expected vs actual behavior
   - Log output (with `-D` flag if possible)

### Suggesting Features

1. Check existing issues and discussions
2. Use the feature request template
3. Explain the use case and benefit
4. Consider implementation complexity

### Pull Requests

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature-name`
3. Make your changes
4. Run tests and linting
5. Submit PR with clear description

## Development Setup

### Prerequisites

- **Linux**: Ubuntu 20.04+, Debian 11+, Fedora 35+
- **Windows**: Windows 10/11 with Visual Studio 2019/2022
- **CUDA Toolkit**: 11.0+ (12.2 recommended)
- **NVIDIA Driver**: 450+ (525+ recommended)
- **Git**: For version control

### Linux Setup

```bash
# Install build dependencies
sudo apt-get update && sudo apt-get install -y \
  build-essential autoconf automake libtool pkg-config \
  libcurl4-openssl-dev libjansson-dev libssl-dev zlib1g-dev \
  cuda-toolkit-12-2 \
  clang-format cppcheck

# Clone with submodules
git clone --recurse-submodules https://github.com/PreCogSecurity/suprminer.git
cd suprminer

# Build
./autogen.sh
./configure.sh
make -j$(nproc)
```

### Windows Setup

```cmd
# Install Visual Studio 2022 with C++ workload
# Install CUDA Toolkit 12.2

# Clone with submodules
git clone --recurse-submodules https://github.com/PreCogSecurity/suprminer.git
cd suprminer

# Build
msbuild ccminer.vcxproj /m /p:Configuration=Release /p:Platform=x64
```

## Coding Standards

### C/C++ Style

We use **clang-format** with LLVM style (see `.clang-format`):

```bash
# Format all source files
find . -name "*.cpp" -o -name "*.h" -o -name "*.cu" -o -name "*.c" -o -name "*.hpp" | \
  grep -v compat/ | xargs clang-format -i

# Check formatting (CI does this)
find . -name "*.cpp" -o -name "*.h" -o -name "*.cu" -o -name "*.c" -o -name "*.hpp" | \
  grep -v compat/ | xargs clang-format --dry-run --Werror
```

Key style points:
- **Indentation**: 4 spaces, no tabs
- **Braces**: Allman style (braces on new lines)
- **Line length**: 100 columns max
- **Naming**: snake_case for variables/functions, PascalCase for types
- **Includes**: System headers first, then local headers, sorted alphabetically

### CUDA Style

- Kernel names: `kernel_name` (snake_case)
- Device functions: `device_function_name`
- Host functions: `host_function_name`
- Register pressure: Use `--maxrregcount` in Makefile.am for optimization
- Shared memory: Declare with `extern __shared__` when dynamic

### Commit Messages

Follow conventional commits:

```
type(scope): brief description

Longer description if needed.

Fixes #123
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`, `perf`, `security`

Examples:
```
feat(algo): add support for kawpow algorithm
fix(config): validate intensity bounds in parse_config
docs(readme): update build instructions for Ubuntu 22.04
security(api): add rate limiting to REST endpoints
```

## Testing

### Running Tests

```bash
# Build and run unit tests
cd tests
gcc -o test_runner test_runner.c -I.. -I../compat -std=c99 -lm
./test_runner
```

### Writing Tests

1. Add test cases to `tests/test_util.c`, `tests/test_algo.c`, or `tests/test_config.c`
2. Use the test framework macros: `TEST_SUITE`, `TEST_ASSERT`, `TEST_ASSERT_EQ`, etc.
3. Test both success and failure paths
4. Keep tests fast and deterministic

### Static Analysis

```bash
# Run cppcheck
cppcheck --enable=warning,style,performance,portability \
  --suppress=missingIncludeSystem \
  -I compat -I . .

# Run clang-tidy (if available)
clang-tidy -p build *.cpp *.cu -- -I. -Icompat
```

## Adding New Algorithms

1. Create directory under `algo/` (e.g., `algo/newalgo/`)
2. Implement CUDA kernel(s) in `.cu` files
3. Add host registration code
4. Update `algos.h` with algorithm ID and name
5. Add to `Makefile.am` sources list
6. Add autotune support if applicable
7. Write tests in `tests/`
8. Update README with algorithm documentation

### Algorithm Registration

```c
// In algos.h
#define ALGO_NEWALGO  99

// In your algorithm file
extern struct algo_gpu newalgo_gpu;
struct algo_gpu newalgo_gpu = {
    .name = "newalgo",
    .init = newalgo_init,
    .create = newalgo_create,
    .destroy = newalgo_destroy,
    .hash = newalgo_hash,
    .get_max64 = newalgo_get_max64,
};
```

## Submitting Changes

### Pull Request Checklist

- [ ] Code compiles on Linux and Windows
- [ ] All tests pass
- [ ] clang-format check passes
- [ ] cppcheck shows no new errors
- [ ] Commit messages follow convention
- [ ] Documentation updated (README, CHANGELOG)
- [ ] No hardcoded credentials or secrets
- [ ] Security considerations addressed

### Review Process

1. Automated CI checks must pass
2. At least one maintainer review required
3. Security-sensitive changes require security team review
4. Performance changes require benchmarks

## Release Process

1. Update version in `configure.ac`
2. Update `CHANGELOG.md`
3. Create release branch: `release/v1.x.y`
4. Run full test suite
5. Tag release: `git tag -a v1.x.y -m "Release v1.x.y"`
6. Build release binaries for Linux/Windows
7. Publish GitHub release with artifacts
8. Merge release branch to main

## Getting Help

- **Documentation**: See [README.md](README.md)
- **Discussions**: [GitHub Discussions](https://github.com/PreCogSecurity/suprminer/discussions)
- **Issues**: [GitHub Issues](https://github.com/PreCogSecurity/suprminer/issues)
- **Security**: See [SECURITY.md](SECURITY.md)

## License

By contributing, you agree that your contributions will be licensed under the GNU General Public License v2.0 (same as the project).

---

*Thank you for contributing to suprminer!*