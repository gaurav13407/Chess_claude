#ifndef CHESS_COLOR_HPP
#define CHESS_COLOR_HPP

namespace chess {

enum class Color : uint8_t {
    White = 0,
    Black = 1
};

constexpr Color opponent(Color c) {
    return static_cast<Color>(static_cast<uint8_t>(c) ^ 1);
}

} // namespace chess

#endif // CHESS_COLOR_HPP