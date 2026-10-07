# Position.hpp

## Purpose
Represents a chess position, including the board state, side to move, castling rights, en passant square, halfmove clock, fullmove number, and Zobrist hash.

## Responsibilities
- Store all information necessary to represent a chess position.
- Provide constructors for initializing a position (default constructor initializes to an empty position; a constructor from FEN string is declared but not yet implemented).
- Provide accessors for the various components of the position.
- (Future) Provide methods to make and unmake moves, generate FEN strings, etc.

## Dependencies
- `Color.hpp`
- `PieceType.hpp`
- `Square.hpp`
- `CastlingRights.hpp`

## Important Classes
- `class Position`: Represents a chess position.

## Important Functions (so far)
- Constructors: 
  - `Position()`: Initializes an empty position (all bitboards zero, side to move white, no castling rights, no en passant, halfmove clock 0, fullmove number 1, Zobrist hash 0).
  - `Position(const char* fen)`: Declared; will initialize the position from a FEN string.
- Accessors (getters):
  - `side_to_move()`: Returns the color whose turn it is.
  - `castling_rights()`: Returns the castling rights.
  - `en_passant_square()`: Returns the en passant square (if any).
  - `halfmove_clock()`: Returns the halfmove clock.
  - `fullmove_number()`: Returns the fullmove number.
  - `zobrist_hash()`: Returns the Zobrist hash of the position.

## Data Structures
- Bitboards: `std::array<std::array<uint64_t, 6>, 2> piece_bitboards_` 
  - First dimension: color (0=white, 1=black)
  - Second dimension: piece type (0=pawn, 1=knight, 2=bishop, 3=rook, 4=queen, 5=king)
  - Each bitboard is a 64-bit integer where a bit is set if a piece of that color and type occupies the corresponding square.
- Occupancy bitboards (derived but cached for efficiency):
  - `uint64_t white_occupancy_`: Bitboard of all white pieces.
  - `uint64_t black_occupancy_`: Bitboard of all black pieces.
  - `uint64_t all_occupancy_`: Bitboard of all pieces (white | black).
- `Color side_to_move_`: The side to move.
- `CastlingRights castling_rights_`: The castling rights available.
- `Square en_passant_square_`: The square where an en passant capture is possible (if any).
- `bool has_en_passant_`: Flag indicating whether there is an en passant target.
- `int halfmove_clock_`: Number of halfmoves since the last capture or pawn advance.
- `int fullmove_number_`: Number of the full move (starts at 1 and increments after black's move).
- `uint64_t zobrist_hash_`: Zobrist hash of the position (for transposition tables).
- `Square king_square_[2]`: Squares of the white and black kings (for quick check detection).

## Why the File Exists
To provide a comprehensive representation of a chess position that serves as the central state object for the chess engine, used by move generation, search, evaluation, and the UCI interface.

## Interaction with Other Files
- Used by move generation to generate legal moves from a given position.
- Used by the search algorithm to evaluate positions and explore the game tree.
- Used by the evaluation function to assess the strength of a position.
- Used by the make/unmake move functions to transition between positions.
- Used by the UCI module to receive and send positions in FEN format.