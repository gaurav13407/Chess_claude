# Chess Engine

A complete, functional, reasonably strong chess engine in modern C++20.

## Features

- UCI protocol support
- Legal move generation
- Castling, en passant, promotion
- Alpha-beta search with iterative deepening
- Transposition table
- Quiescence search
- Move ordering (killer moves, history heuristic)
- Classical evaluation (material, piece-square tables, pawn structure, king safety, etc.)
- Zobrist hashing
- Time management
- Perft testing
- Extensive automated tests
- Benchmarking

## Build

Requires CMake and a C++20 compatible compiler (GCC or Clang).

```bash
cmake -S . -B build
cmake --build build -j
```

## Running

```bash
./build/chess_engine uci
```

## Testing

```bash
./build/tests/test_runner  # or ctest
```

## Documentation

See the `docs/` directory for detailed documentation.

## License

MIT