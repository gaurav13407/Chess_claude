#ifndef CHESS_UTILS_SQUARE_HPP
#define CHESS_UTILS_SQUARE_HPP

#include <cstdint>

namespace chess {

class Square {
public:
    constexpr Square() : sq_(0) {}
    constexpr explicit Square(uint8_t sq) : sq_(sq) {}
    constexpr Square(int rank, int file) : sq_(static_cast<uint8_t>(rank * 8 + file)) {}

    constexpr uint8_t value() const { return sq_; }
    constexpr int rank() const { return static_cast<int>(sq_) / 8; }
    constexpr int file() const { return static_cast<int>(sq_) % 8; }

    constexpr bool operator==(Square other) const { return sq_ == other.sq_; }
    constexpr bool operator!=(Square other) const { return sq_ != other.sq_; }
    constexpr bool operator<(Square other) const { return sq_ < other.sq_; }

private:
    uint8_t sq_;
};

} // namespace chess

#endif // CHESS_UTILS_SQUARE_HPP