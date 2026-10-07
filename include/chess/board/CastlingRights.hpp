#ifndef CHESS_BOARD_CASTLING_RIGHTS_HPP
#define CHESS_BOARD_CASTLING_RIGHTS_HPP

#include <cstdint>

namespace chess {

class CastlingRights {
public:
    constexpr CastlingRights() : rhs_(0) {}
    constexpr explicit CastlingRights(uint8_t rhs) : rhs_(rhs) {}

    // Constants for each right
    static constexpr uint8_t WhiteKingSide   = 0x1;
    static constexpr uint8_t WhiteQueenSide  = 0x2;
    static constexpr uint8_t BlackKingSide   = 0x4;
    static constexpr uint8_t BlackQueenSide  = 0x8;

    // Getters
    constexpr bool white_king_side()   const { return (rhs_ & WhiteKingSide)   != 0; }
    constexpr bool white_queen_side()  const { return (rhs_ & WhiteQueenSide)  != 0; }
    constexpr bool black_king_side()   const { return (rhs_ & BlackKingSide)   != 0; }
    constexpr bool black_queen_side()  const { return (rhs_ & BlackQueenSide)  != 0; }

    // Setters
    constexpr void set_white_king_side(bool b)   { rhs_ = b ? (rhs_ | WhiteKingSide)   : (rhs_ & ~WhiteKingSide); }
    constexpr void set_white_queen_side(bool b)  { rhs_ = b ? (rhs_ | WhiteQueenSide)  : (rhs_ & ~WhiteQueenSide); }
    constexpr void set_black_king_side(bool b)   { rhs_ = b ? (rhs_ | BlackKingSide)   : (rhs_ & ~BlackKingSide); }
    constexpr void set_black_queen_side(bool b)  { rhs_ = b ? (rhs_ | BlackQueenSide)  : (rhs_ & ~BlackQueenSide); }

    // Clear all
    constexpr void clear() { rhs_ = 0; }

    // Equality
    constexpr bool operator==(CastlingRights other) const { return rhs_ == other.rhs_; }
    constexpr bool operator!=(CastlingRights other) const { return rhs_ != other.rhs_; }

private:
    uint8_t rhs_;
};

} // namespace chess

#endif // CHESS_BOARD_CASTLING_RIGHTS_HPP