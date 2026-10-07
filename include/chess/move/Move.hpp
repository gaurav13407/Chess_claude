#ifndef CHESS_MOVE_MOVE_HPP
#define CHESS_MOVE_MOVE_HPP

#include <cstdint>
#include "../PieceType.hpp"
#include "../utils/Square.hpp"

namespace chess {

class Move {
public:
    constexpr Move() : data_(0) {
        set_promoted_piece(PieceType::None);
        set_captured_piece(PieceType::None);
    }

    // Constructor from source, destination, promoted piece, captured piece, and flags
    constexpr Move(Square src, Square dst, PieceType promoted, PieceType captured,
                   bool en_passant = false, bool castling = false)
        : data_(0) {
        set_source(src);
        set_destination(dst);
        set_promoted_piece(promoted);
        set_captured_piece(captured);
        set_en_passant(en_passant);
        set_castling(castling);
    }

    // Getters
    constexpr Square source() const { return Square(static_cast<uint8_t>(data_ & 0x3F)); }
    constexpr Square destination() const { return Square(static_cast<uint8_t>((data_ >> 6) & 0x3F)); }
    constexpr PieceType promoted_piece() const {
        return static_cast<PieceType>((data_ >> 12) & 0x0F);
    }
    constexpr PieceType captured_piece() const {
        return static_cast<PieceType>((data_ >> 16) & 0x0F);
    }
    constexpr bool en_passant() const { return (data_ >> 20) & 0x1; }
    constexpr bool castling() const { return (data_ >> 21) & 0x1; }

    // Setters
    constexpr void set_source(Square sq) {
        data_ = (data_ & ~0x3F) | (sq.value() & 0x3F);
    }
    constexpr void set_destination(Square sq) {
        data_ = (data_ & ~(0x3F << 6)) | ((static_cast<uint32_t>(sq.value()) & 0x3F) << 6);
    }
    constexpr void set_promoted_piece(PieceType pt) {
        data_ = (data_ & ~(0x0F << 12)) | (static_cast<uint32_t>(pt) & 0x0F) << 12;
    }
    constexpr void set_captured_piece(PieceType pt) {
        data_ = (data_ & ~(0x0F << 16)) | (static_cast<uint32_t>(pt) & 0x0F) << 16;
    }
    constexpr void set_en_passant(bool ep) {
        data_ = (data_ & ~(0x1 << 20)) | (static_cast<uint32_t>(ep) & 0x1) << 20;
    }
    constexpr void set_castling(bool ca) {
        data_ = (data_ & ~(0x1 << 21)) | (static_cast<uint32_t>(ca) & 0x1) << 21;
    }

    // Equality
    constexpr bool operator==(Move other) const { return data_ == other.data_; }
    constexpr bool operator!=(Move other) const { return data_ != other.data_; }

private:
    uint32_t data_;
};

} // namespace chess

#endif // CHESS_MOVE_MOVE_HPP