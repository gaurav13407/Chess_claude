#ifndef CHESS_BOARD_POSITION_HPP
#define CHESS_BOARD_POSITION_HPP

#include <array>
#include <cstdint>
#include "../Color.hpp"
#include "../utils/Square.hpp"
#include "CastlingRights.hpp"
#include "../PieceType.hpp"

namespace chess {

class Position {
public:
    Position()
        : piece_bitboards_{}
        , white_occupancy_(0)
        , black_occupancy_(0)
        , all_occupancy_(0)
        , side_to_move_(Color::White)
        , castling_rights_()
        , en_passant_square_()
        , has_en_passant_(false)
        , halfmove_clock_(0)
        , fullmove_number_(1)
        , zobrist_hash_(0)
        , king_square_{}
    {}

    explicit Position(const char* fen) : Position() {
        // TODO: implement FEN parsing
    }

    // Getters (to be implemented later)
    Color side_to_move() const { return side_to_move_; }
    const CastlingRights& castling_rights() const { return castling_rights_; }
    Square en_passant_square() const { return en_passant_square_; }
    int halfmove_clock() const { return halfmove_clock_; }
    int fullmove_number() const { return fullmove_number_; }
    uint64_t zobrist_hash() const { return zobrist_hash_; }

private:
    // Bitboards: [color][piece_type]
    std::array<std::array<uint64_t, 6>, 2> piece_bitboards_{};
    // Occupancy bitboards
    uint64_t white_occupancy_ = 0;
    uint64_t black_occupancy_ = 0;
    uint64_t all_occupancy_ = 0;

    Color side_to_move_ = Color::White;
    CastlingRights castling_rights_;
    Square en_passant_square_ = Square(); // square 0 (a1) represents no en passant? We'll use a separate boolean.
    bool has_en_passant_ = false;

    int halfmove_clock_ = 0;
    int fullmove_number_ = 1;
    uint64_t zobrist_hash_ = 0;

    // King squares for each color (for quick check detection)
    Square king_square_[2]; // [color]
};

} // namespace chess

#endif // CHESS_BOARD_POSITION_HPP