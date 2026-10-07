# CastlingRights.hpp

## Purpose
Represents the castling rights for both sides in a chess position.

## Responsibilities
- Track whether each side can castle king-side and/or queen-side.
- Provide methods to get and set each castling right.
- Provide initialization to no castling rights.

## Dependencies
- None (only standard library types).

## Important Classes
- `class CastlingRights`: Represents the castling rights using a 4-bit bitmask.

## Important Functions
- Constructors: default constructor (no rights), and constructor from a bitmask.
- Getters: `white_king_side()`, `white_queen_side()`, `black_king_side()`, `black_queen_side()`.
- Setters: `set_white_king_side(bool)`, `set_white_queen_side(bool)`, `set_black_king_side(bool)`, `set_black_queen_side(bool)`.
- Method: `clear()` to reset all rights.
- Operators: `==`, `!=`.

## Data Structures
- The castling rights are stored as an 8-bit integer (uint8_t) with the following bitmask:
  - Bit 0 (0x1): white king-side castling rights
  - Bit 1 (0x2): white queen-side castling rights
  - Bit 2 (0x4): black king-side castling rights
  - Bit 3 (0x8): black queen-side castling rights

## Why the File Exists
To provide a clean, efficient representation of castling rights, which are essential for move generation (to generate castling moves) and for making/unmaking moves (to update rights after a move).

## Interaction with Other Files
Used by the Position class to store the current castling rights, by move generation to check if castling is allowed, and by the make/unmake move functions to update rights after a move.