# Square.hpp

## Purpose
Represents a square on the chessboard and provides utility functions for converting between square indices and ranks/files.

## Responsibilities
- Encapsulate a square index (0-63) where 0 represents a1 and 63 represents h8 (or another consistent mapping).
- Provide methods to get the rank and file of a square.
- Provide constructors from rank and file or from a raw index.
- Provide comparison operators.

## Dependencies
- None (only standard library types).

## Important Classes
- `class Square`: Represents a chessboard square.

## Important Functions
- Constructors: `Square()`, `Square(uint8_t sq)`, `Square(int rank, int file)`.
- Methods: `uint8_t value() const`, `int rank() const`, `int file() const`.
- Operators: `==`, `!=`, `<`.

## Data Structures
- None.

## Why the File Exists
To provide a clear, type-safe representation of a chessboard square and to encapsulate square-related logic, avoiding the use of raw integers and making the code more self-documenting.

## Interaction with Other Files
Used throughout the engine wherever a square is needed: board representation, move generation, move making/unmaking, evaluation, etc.