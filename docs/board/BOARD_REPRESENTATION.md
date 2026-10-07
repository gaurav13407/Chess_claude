# Board Representation

## Purpose
This document describes the board representation used in the chess engine, focusing on the use of bitboards for efficient storage and manipulation of the chess position.

## Why Bitboards?
Bitboards are a common technique in chess engines where each piece type and color is represented by a 64-bit integer (bitboard). Each bit corresponds to a square on the chessboard. This representation allows for efficient:
- Piece lookup
- Occupancy checks
- Attack generation (using bitwise operations)
- Move generation
- Make and unmake operations
- Position hashing (Zobrist hashing uses bitboards)

## Square Encoding
We use the following square mapping:
- Square 0: a1 (least significant bit)
- Square 1: b1
- ...
- Square 7: h1
- Square 8: a2
- ...
- Square 63: h8 (most significant bit)

This mapping is little-endian by rank (rank 1 first) and within a rank, file a to h.

## Bitboard Layout
For each color (White and Black) and each piece type (Pawn, Knight, Bishop, Rook, Queen, King), we maintain a separate bitboard.

Thus, we have an array of bitboards: `bitboards[color][piece_type]` where:
- `color`: 0 for White, 1 for Black
- `piece_type`: 0 for Pawn, 1 for Knight, 2 for Bishop, 3 for Rook, 4 for Queen, 5 for King

Each bitboard is a 64-bit integer where a bit is set to 1 if a piece of that color and type occupies the corresponding square.

## Occupancy Bitboards
We also maintain three occupancy bitboards for quick checks:
- `white_occupancy`: union of all white piece bitboards
- `black_occupancy`: union of all black piece bitboards
- `all_occupancy`: union of white and black occupancy (or `white_occupancy | black_occupancy`)

These are updated incrementally when pieces are placed or removed.

## Piece Representation
Pieces are represented by:
- `enum class Color`: { White, Black }
- `enum class PieceType`: { Pawn, Knight, Bishop, Rook, Queen, King, None } (None for empty squares)

A piece on the board is uniquely identified by its color and type.

## Key Operations
### Setting a Piece
To place a piece on a square:
1. Compute the bit index: `1ULL << square.value()`
2. Set the corresponding bit in the appropriate bitboard: `bitboards[color][type] |= bitmask`
3. Update the occupancy bitboards:
   - If white: `white_occupancy |= bitmask`
   - If black: `black_occupancy |= bitmask`
   - `all_occupancy |= bitmask`

### Removing a Piece
To remove a piece from a square (assuming we know what piece is there):
1. Compute the bit mask: `1ULL << square.value()`
2. Clear the corresponding bit in the appropriate bitboard: `bitboards[color][type] &= ~bitmask`
3. Update the occupancy bitboards similarly with `&= ~bitmask`

### Checking a Square
To check if a square is occupied by a piece of a given color and type:
- `bitboards[color][type] & bitmask` != 0

To check if a square is occupied by any piece:
- `all_occupancy & bitmask` != 0

To get the piece on a square (if any), we iterate over all color and piece type combinations and check the bitboards.

## Performance Considerations
Bitwise operations are extremely fast on modern CPUs. The bitboard representation allows us to:
- Generate moves for sliding pieces (Bishop, Rook, Queen) using magic bitboards or other techniques (to be implemented later).
- Check for attacks and overlaps with simple bitwise AND, OR, NOT operations.
- Compute pawn attacks, knight attacks, king attacks via precomputed tables or bitwise shifts.

## Interaction with Other Components
- **Move Generation**: Uses the bitboards to generate pseudo-legal moves by looking at piece positions and attacking squares.
- **Make/Unmake Move**: Updates the bitboards and occupancies when a move is made or undone.
- **Evaluation**: Uses the bitboards to evaluate material, piece-square tables, pawn structure, etc., by iterating over set bits.
- **Zobrist Hashing**: Each piece on each square contributes to the hash; updating the hash is done by XORing the random values associated with the piece and square.

## Future Improvements
- Consider using SIMD instructions for parallel bitwise operations.
- Explore advanced bitboard techniques like magic bitboards for sliding piece attack generation.
- Consider separating the bitboard representation from the Position class for easier experimentation.

## Files Involved
- `include/chess/board/Position.hpp`: Contains the bitboard arrays and methods to manipulate them.
- `include/chess/utils/Square.hpp`: Defines the Square class and conversion functions.
- `include/chess/Color.hpp` and `include/chess/PieceType.hpp`: Define the enums for color and piece type.
- `docs/board/BOARD_REPRESENTATION.md`: This document.
