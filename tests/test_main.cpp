#include <gtest/gtest.h>
#include <chess/Color.hpp>
#include <chess/PieceType.hpp>
#include <chess/utils/Square.hpp>
#include <chess/move/Move.hpp>
#include <chess/board/CastlingRights.hpp>
#include <chess/board/Position.hpp>

// Test Color
TEST(ColorTest, Basics) {
    using chess::Color;
    EXPECT_EQ(static_cast<int>(chess::Color::White), 0);
    EXPECT_EQ(static_cast<int>(chess::Color::Black), 1);
    EXPECT_EQ(static_cast<int>(chess::opponent(chess::Color::White)), 1);
    EXPECT_EQ(static_cast<int>(chess::opponent(chess::Color::Black)), 0);
}

// Test PieceType
TEST(PieceTypeTest, Basics) {
    using chess::PieceType;
    EXPECT_EQ(static_cast<int>(chess::PieceType::Pawn), 0);
    EXPECT_EQ(static_cast<int>(chess::PieceType::Knight), 1);
    EXPECT_EQ(static_cast<int>(chess::PieceType::Bishop), 2);
    EXPECT_EQ(static_cast<int>(chess::PieceType::Rook), 3);
    EXPECT_EQ(static_cast<int>(chess::PieceType::Queen), 4);
    EXPECT_EQ(static_cast<int>(chess::PieceType::King), 5);
    EXPECT_EQ(static_cast<int>(chess::PieceType::None), 6);
}

// Test Square
TEST(SquareTest, Basics) {
    using chess::Square;
    Square sq; // default constructor
    EXPECT_EQ(sq.value(), 0);
    EXPECT_EQ(sq.rank(), 0);
    EXPECT_EQ(sq.file(), 0);

    Square sq2(0, 0); // rank 0, file 0 -> a1
    EXPECT_EQ(sq2.value(), 0);
    EXPECT_EQ(sq2.rank(), 0);
    EXPECT_EQ(sq2.file(), 0);

    Square sq3(7, 7); // rank 7, file 7 -> h8
    EXPECT_EQ(sq3.value(), 63);
    EXPECT_EQ(sq3.rank(), 7);
    EXPECT_EQ(sq3.file(), 7);

    Square sq4(63); // from value
    EXPECT_EQ(sq4.value(), 63);
    EXPECT_EQ(sq4.rank(), 7);
    EXPECT_EQ(sq4.file(), 7);

    EXPECT_EQ(Square(0, 0), Square(0));
    EXPECT_NE(Square(0, 0), Square(1, 0));
}

// Test Move
TEST(MoveTest, Basics) {
    using chess::Move;
    using chess::Square;
    using chess::PieceType;

    Move m; // default
    EXPECT_EQ(m.source().value(), 0);
    EXPECT_EQ(m.destination().value(), 0);
    EXPECT_EQ(m.promoted_piece(), chess::PieceType::None);
    EXPECT_EQ(m.captured_piece(), chess::PieceType::None);
    EXPECT_FALSE(m.en_passant());
    EXPECT_FALSE(m.castling());

    Move m2(Square(0), Square(1), chess::PieceType::Queen, chess::PieceType::Pawn, false, false);
    EXPECT_EQ(m2.source().value(), 0);
    EXPECT_EQ(m2.destination().value(), 1);
    EXPECT_EQ(m2.promoted_piece(), chess::PieceType::Queen);
    EXPECT_EQ(m2.captured_piece(), chess::PieceType::Pawn);
    EXPECT_FALSE(m2.en_passant());
    EXPECT_FALSE(m2.castling());

    // Test setters
    m2.set_en_passant(true);
    EXPECT_TRUE(m2.en_passant());
    m2.set_castling(true);
    EXPECT_TRUE(m2.castling());
}

// Test CastlingRights
TEST(CastlingRightsTest, Basics) {
    using chess::CastlingRights;
    CastlingRights cr; // default
    EXPECT_FALSE(cr.white_king_side());
    EXPECT_FALSE(cr.white_queen_side());
    EXPECT_FALSE(cr.black_king_side());
    EXPECT_FALSE(cr.black_queen_side());

    cr.set_white_king_side(true);
    EXPECT_TRUE(cr.white_king_side());
    cr.set_white_queen_side(true);
    EXPECT_TRUE(cr.white_queen_side());
    cr.set_black_king_side(true);
    EXPECT_TRUE(cr.black_king_side());
    cr.set_black_queen_side(true);
    EXPECT_TRUE(cr.black_queen_side());

    cr.set_white_king_side(false);
    EXPECT_FALSE(cr.white_king_side());

    CastlingRights cr2(static_cast<uint8_t>(0xF)); // all rights
    EXPECT_TRUE(cr2.white_king_side());
    EXPECT_TRUE(cr2.white_queen_side());
    EXPECT_TRUE(cr2.black_king_side());
    EXPECT_TRUE(cr2.black_queen_side());
}

// Test Position (default constructor)
TEST(PositionTest, DefaultConstructor) {
    using chess::Position;
    Position pos;
    EXPECT_EQ(pos.side_to_move(), chess::Color::White);
    EXPECT_FALSE(pos.castling_rights().white_king_side()); // initially false? Actually default castling rights should be all false? We'll see.
    // We'll just check that it doesn't crash.
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}