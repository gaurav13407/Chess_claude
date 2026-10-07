# Move.hpp

## Purpose
Defines the `Move` class for representing a chess move in a compact, efficient format.

## Responsibilities
- Store all necessary information to make and unmake a move on a chessboard.
- Support encoding of: source square, destination square, promotion piece, captured piece, en passant flag, and castling flag.
- Provide getter and setter methods for each component.
- Provide equality operators.

## Dependencies
- `Square.hpp`
- `PieceType.hpp`

## Important Classes
- `class Move`: Represents a chess move using a 32-bit integer bit field.

## Important Functions
- Constructors: default constructor (initializes to a null move), and constructor from source, destination, promoted piece, captured piece, and flags.
- Getters: `source()`, `destination()`, `promoted_piece()`, `captured_piece()`, `en_passant()`, `castling()`.
- Setters: `set_source()`, `set_destination()`, `set_promoted_piece()`, `set_captured_piece()`, `set_en_passant()`, `set_castling()`.
- Operators: `==`, `!=`.

## Data Structures
- The move is stored as a 32-bit unsigned integer with the following bit layout:
  - Bits 0-5: source square (0-63)
  - Bits 6-11: destination square (0-63)
  - Bits 12-15: promoted piece type (0=None, 1=Pawn, 2=Knight, 3=Bishop, 4=Rook, 5=Queen, 6=King) [Note: we use the PieceType enum values, but note that 0 is Pawn and 6 is King; we use 0 for no promotion? Actually we store the PieceType enum value directly, and we use PieceType::None (6) to indicate no promotion. Similarly for captured piece.]
  - Bits 16-19: captured piece type (same encoding as promoted piece)
  - Bit 20: en passant flag (1 if the move is an en passant capture)
  - Bit 21: castling flag (1 if the move is a castling move)

## Why the File Exists
To provide an efficient, compact representation of a chess move that can be easily stored, copied, and used in move generation and search algorithms, while still being self-documenting through its interface.

## Interaction with Other Files
Used by move generation to create new moves, by the board to make and unmake moves, and by the search to store and analyze moves.