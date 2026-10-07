#ifndef CHESS_BOARD_POSITION_HPP
#define CHESS_BOARD_POSITION_HPP

#include <array>
#include <cstdint>
#include <string>
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
        // Parse the FEN string
        std::string fen_str(fen);
        size_t pos = 0;
        auto next_space = [&]() {
            size_t next = fen_str.find(' ', pos);
            std::string token = (next == std::string::npos) ? fen_str.substr(pos) : fen_str.substr(pos, next - pos);
            pos = (next == std::string::npos) ? fen_str.size() : next + 1;
            return token;
        };

        // 1. Piece placement
        std::string placement = next_space();
        int rank = 7; // start from rank 8 (index 7) down to 0
        int file = 0;
        for (char c : placement) {
            if (c == '/') {
                rank--;
                file = 0;
            } else if (isdigit(c)) {
                int offset = c - '0';
                file += offset;
            } else {
                Color color;
                PieceType type;
                switch (c) {
                    case 'P': color = Color::White; type = PieceType::Pawn; break;
                    case 'N': color = Color::White; type = PieceType::Knight; break;
                    case 'B': color = Color::White; type = PieceType::Bishop; break;
                    case 'R': color = Color::White; type = PieceType::Rook; break;
                    case 'Q': color = Color::White; type = PieceType::Queen; break;
                    case 'K': color = Color::White; type = PieceType::King; break;
                    case 'p': color = Color::Black; type = PieceType::Pawn; break;
                    case 'n': color = Color::Black; type = PieceType::Knight; break;
                    case 'b': color = Color::Black; type = PieceType::Bishop; break;
                    case 'r': color = Color::Black; type = PieceType::Rook; break;
                    case 'q': color = Color::Black; type = PieceType::Queen; break;
                    case 'k': color = Color::Black; type = PieceType::King; break;
                    default: continue; // should not happen
                }
                set_piece(Square(static_cast<uint8_t>(rank * 8 + file)), color, type);
                file++;
            }
        }

        // 2. Side to move
        std::string side_str = next_space();
        if (side_str == "w") {
            side_to_move_ = Color::White;
        } else if (side_str == "b") {
            side_to_move_ = Color::Black;
        }

        // 3. Castling rights
        std::string castling_str = next_space();
        if (castling_str != "-") {
            for (char c : castling_str) {
                switch (c) {
                    case 'K': castling_rights_.set_white_king_side(true); break;
                    case 'Q': castling_rights_.set_white_queen_side(true); break;
                    case 'k': castling_rights_.set_black_king_side(true); break;
                    case 'q': castling_rights_.set_black_queen_side(true); break;
                }
            }
        }

        // 4. En passant square
        std::string ep_str = next_space();
        if (ep_str != "-") {
            // Assume the string is two characters: file and rank
            if (ep_str.size() == 2) {
                char file_char = ep_str[0];
                char rank_char = ep_str[1];
                int ep_file = file_char - 'a';
                int ep_rank = rank_char - '1';
                if (ep_file >= 0 && ep_file < 8 && ep_rank >= 0 && ep_rank < 8) {
                    en_passant_square_ = Square(static_cast<uint8_t>(ep_rank * 8 + ep_file));
                    has_en_passant_ = true;
                }
            }
        }

        // 5. Halfmove clock
        std::string hm_clock_str = next_space();
        halfmove_clock_ = std::stoi(hm_clock_str);

        // 6. Fullmove number
        std::string fullmove_str = next_space();
        fullmove_number_ = std::stoi(fullmove_str);
    }

    // Returns the FEN string representation of the position
    std::string fen() const {
        std::string result;

        // 1. Piece placement
        for (int rank = 7; rank >= 0; --rank) {
            int empty_count = 0;
            for (int file = 0; file < 8; ++file) {
                Square sq(static_cast<uint8_t>(rank * 8 + file));
                Color c;
                PieceType p = piece_on(sq, c);
                if (p == PieceType::None) {
                    empty_count++;
                } else {
                    if (empty_count > 0) {
                        result += static_cast<char>('0' + empty_count);
                        empty_count = 0;
                    }
                    char piece_char;
                    switch (p) {
                        case PieceType::Pawn:   piece_char = (c == Color::White) ? 'P' : 'p'; break;
                        case PieceType::Knight: piece_char = (c == Color::White) ? 'N' : 'n'; break;
                        case PieceType::Bishop: piece_char = (c == Color::White) ? 'B' : 'b'; break;
                        case PieceType::Rook:   piece_char = (c == Color::White) ? 'R' : 'r'; break;
                        case PieceType::Queen:  piece_char = (c == Color::White) ? 'Q' : 'q'; break;
                        case PieceType::King:   piece_char = (c == Color::White) ? 'K' : 'k'; break;
                        default: piece_char = '?'; // should not happen
                    }
                    result += piece_char;
                }
            }
            if (empty_count > 0) {
                result += static_cast<char>('0' + empty_count);
            }
            if (rank != 0) {
                result += '/';
            }
        }

        result += ' ';
        // 2. Side to move
        result += (side_to_move_ == Color::White) ? 'w' : 'b';
        result += ' ';

        // 3. Castling rights
        if (castling_rights_.white_king_side()) result += 'K';
        if (castling_rights_.white_queen_side()) result += 'Q';
        if (castling_rights_.black_king_side()) result += 'k';
        if (castling_rights_.black_queen_side()) result += 'q';
        if (!castling_rights_.white_king_side() && !castling_rights_.white_queen_side() &&
            !castling_rights_.black_king_side() && !castling_rights_.black_queen_side()) {
            result += '-';
        }
        result += ' ';

        // 4. En passant square
        if (has_en_passant_) {
            int file = en_passant_square_.file();
            int rank = en_passant_square_.rank();
            result += static_cast<char>('a' + file);
            result += static_cast<char>('1' + rank);
        } else {
            result += '-';
        }
        result += ' ';

        // 5. Halfmove clock
        result += std::to_string(halfmove_clock_);
        result += ' ';

        // 6. Fullmove number
        result += std::to_string(fullmove_number_);

        return result;
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
    bool has_en_passant() const { return has_en_passant_; } // Added getter for testing
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