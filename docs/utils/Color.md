# Color.hpp

## Purpose
Defines the `Color` enum class and related utilities for representing the two sides in chess (White and Black).

## Responsibilities
- Provide a type-safe enumeration for chess colors.
- Provide a function to get the opponent of a given color.

## Dependencies
- None (only standard library types).

## Important Classes and Enums
- `enum class Color`: Represents the color of a chess piece or side to move. Values: `White`, `Black`.

## Important Functions
- `constexpr Color opponent(Color c)`: Returns the opposite color.

## Data Structures
- None.

## Why the File Exists
To provide a clear, type-safe representation of chess colors, avoiding the use of raw booleans or integers, and to encapsulate color-related logic.

## Interaction with Other Files
This header is included by virtually every other header in the engine, as color is a fundamental concept used in board representation, move generation, search, evaluation, etc.