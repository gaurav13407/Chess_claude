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
        // TODO: implement FEN parsing (M3)
    }

    // Clears the board (sets all pieces to empty)
    void clear() {
        piece_bitboards_.fill({});
        white_occupancy_ = 0;
        black_occupancy_ = 0;
        all_occupancy_ = 0;
        // Note: side_to_move, castling rights, en passant, halfmove clock, fullmove number, zobrist hash, and king squares are not cleared by this method.
        // Those are part of the game state and should be reset separately if needed.
        // For now, we only clear the piece placement.
    }

    // Places a piece on the given square.
    // Assumes the square is empty.
    void set_piece(Square sq, Color color, PieceType type) {
        int c = static_cast<int>(color);
        int t = static_cast<int>(type);
        piece_bitboards_[c][t] |= (1ULL << sq.value());
        // Update occupancies
        if (color == Color::White) {
            white_occupancy_ |= (1ULL << sq.value());
        } else {
            black_occupancy_ |= (1ULL << sq.value());
        }
        all_occupancy_ |= (1ULL << sq.value());
        // Update king squares if the piece is a king
        if (type == PieceType::King) {
            king_square_[c] = sq;
        }
        // TODO: Update Zobrist hash (will be done incrementally in makeMove/unmakeMove)
    }

    // Removes a piece from the given square.
    // Returns the piece that was removed (color and type) via the output parameters.
    // Returns true if there was a piece, false if the square was empty.
    bool remove_piece(Square sq, Color& color, PieceType& type) {
        // First, check what piece is on the square
        PieceType piece_type = piece_on(sq, color);
        if (piece_type == PieceType::None) {
            return false;
        }
        type = piece_type;
        // Now remove it
        int c = static_cast<int>(color);
        int t = static_cast<int>(piece_type);
        piece_bitboards_[c][t] &= ~(1ULL << sq.value());
        // Update occupancies
        if (color == Color::White) {
            white_occupancy_ &= ~(1ULL << sq.value());
        } else {
            black_occupancy_ &= ~(1ULL << sq.value());
        }
        all_occupancy_ &= ~(1ULL << sq.value());
        // Update king squares if the piece was a king
        if (piece_type == PieceType::King) {
            king_square_[c] = Square(); // temporary: set to invalid square (we'll improve later)
        }
        return true;
    }

    // Returns the piece type and color on the given square.
    // If the square is empty, returns PieceType::None and the color is set to Color::White (arbitrary).
    PieceType piece_on(Square sq, Color& color) const {
        // Check each color and each piece type
        for (int c = 0; c < 2; ++c) {
            for (int t = 0; t < 6; ++t) {
                if (piece_bitboards_[c][t] & (1ULL << sq.value())) {
                    color = static_cast<Color>(c);
                    return static_cast<PieceType>(t);
                }
            }
        }
        color = Color::White; // arbitrary
        return PieceType::None;
    }

    // Returns the piece type on the given square (ignoring color)
    PieceType piece_on(Square sq) const {
        Color dummy;
        return piece_on(sq, dummy);
    }

    // Returns true if the square is occupied by a piece of the given color
    bool occupied_by(Square sq, Color color) const {
        int c = static_cast<int>(color);
        for (int t = 0; t < 6; ++t) {
            if (piece_bitboards_[c][t] & (1ULL << sq.value())) {
                return true;
            }
        }
        return false;
    }

    // Returns true if the square is occupied (by any piece)
    bool occupied(Square sq) const {
        return (all_occupancy_ & (1ULL << sq.value())) != 0;
    }

    // Getters (same as before)
    Color side_to_move() const { return side_to_move_; }
    const CastlingRights& castling_rights() const { return castling_rights_; }
    Square en_passant_square() const { return en_passant_square_; }
    int halfmove_clock() const { return halfmove_clock_; }
    int fullmove_number() const { return fullmove_number_; }
    uint64_t zobrist_hash() const { return zobrist_hash_; }

    // TODO: Add methods to set side_to_move, castling rights, en passant, halfmove clock, fullmove number, zobrist hash as needed.

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

    // King squares for each color (for quick check detection) - TODO: replace with a method that computes from bitboards
    Square king_square_[2]; // [color]
};

} // namespace chess

#endif // CHESS_BOARD_POSITION_HPP