# PieceType.hpp

## Purpose
Defines the `PieceType` enum class for representing the different types of chess pieces.

## Responsibilities
- Provide a type-safe enumeration for chess piece types.
- Include a value for `None` to represent empty squares.

## Dependencies
- None (only standard library types).

## Important Classes and Enums
- `enum class PieceType`: Represents the type of a chess piece. Values: `Pawn`, `Knight`, `Bishop`, `Rook`, `Queen`, `King`, `None`.

## Important Functions
- None.

## Data Structures
- None.

## Why the File Exists
To provide a clear, type-safe representation of chess piece types, avoiding the use of raw integers, and to encapsulate piece-type-related logic.

## Interaction with Other Files
This header is used in board representation, move generation, evaluation, and anywhere pieces are processed.