#ifndef CHESS_PIECE_TYPE_HPP
#define CHESS_PIECE_TYPE_HPP

namespace chess {

enum class PieceType : uint8_t {
    Pawn = 0,
    Knight = 1,
    Bishop = 2,
    Rook = 3,
    Queen = 4,
    King = 5,
    None = 6  // For representing empty squares
};

} // namespace chess

#endif // CHESS_PIECE_TYPE_HPP