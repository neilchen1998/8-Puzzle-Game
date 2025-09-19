# 8 Puzzle Game

## Summary

8 Puzzle problem is a simple version of [15 Puzzle problem](https://en.wikipedia.org/wiki/15_puzzle).
This project offers a pretty graphic user interface, allowing the players an engaging way to solve the scrambled title puzzle.

## Requirements

The requirements are:

- CMake 3.14 or better; 4.0+ highly recommended
- A C++23 compatible compiler ([gcc](https://gcc.gnu.org/) or [llvm](https://llvm.org/))
- Git
- Raylib (will be downloaded if not present)
- Slidr (will be downloaded if not present)
- Conda/Miniconda (optional, highly recommended)
- Doxygen (optional, highly recommended)

## Instructions

To configure:

```bash
cmake -S . -B build
```

Add `--toolchain=./<your_toolchain_file>.toolchain` if you want to use your own toolchain.

Add `-GNinja` if you have Ninja.

To build:

```bash
cmake --build build
```

To launch the app:

```bash
./build/apps/app
```

## 



## Notes

### Confetti



## Reference

- [raylib](https://www.raylib.com/)
