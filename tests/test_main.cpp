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

// Test Position
TEST(PositionTest, DefaultConstructor) {
    using chess::Position;
    using chess::Color;
    using chess::PieceType;
    using chess::Square;

    Position pos;
    EXPECT_EQ(pos.side_to_move(), chess::Color::White);
    // Initially, the board is empty
    Color c;
    EXPECT_EQ(pos.piece_on(Square(0), c), chess::PieceType::None);
    EXPECT_FALSE(pos.occupied(Square(0)));
    EXPECT_FALSE(pos.occupied_by(Square(0), chess::Color::White));
    EXPECT_FALSE(pos.occupied_by(Square(0), chess::Color::Black));
}

TEST(PositionTest, SetAndGetPiece) {
    using chess::Position;
    using chess::Color;
    using chess::PieceType;
    using chess::Square;

    Position pos;
    // Place a white pawn on a1 (square 0)
    pos.set_piece(Square(0), chess::Color::White, chess::PieceType::Pawn);
    Color c;
    PieceType pt = pos.piece_on(Square(0), c);
    EXPECT_EQ(pt, chess::PieceType::Pawn);
    EXPECT_EQ(c, chess::Color::White);
    EXPECT_TRUE(pos.occupied(Square(0)));
    EXPECT_TRUE(pos.occupied_by(Square(0), chess::Color::White));
    EXPECT_FALSE(pos.occupied_by(Square(0), chess::Color::Black));

    // Place a black king on e1 (square 4)
    pos.set_piece(Square(4), chess::Color::Black, chess::PieceType::King);
    pt = pos.piece_on(Square(4), c);
    EXPECT_EQ(pt, chess::PieceType::King);
    EXPECT_EQ(c, chess::Color::Black);
    EXPECT_TRUE(pos.occupied(Square(4)));
    EXPECT_TRUE(pos.occupied_by(Square(4), chess::Color::Black));
    EXPECT_FALSE(pos.occupied_by(Square(4), chess::Color::White));

    // Check that a1 still has the white pawn
    pt = pos.piece_on(Square(0), c);
    EXPECT_EQ(pt, chess::PieceType::Pawn);
    EXPECT_EQ(c, chess::Color::White);
}

TEST(PositionTest, RemovePiece) {
    using chess::Position;
    using chess::Color;
    using chess::PieceType;
    using chess::Square;

    Position pos;
    // Place a white knight on b1 (square 1)
    pos.set_piece(Square(1), chess::Color::White, chess::PieceType::Knight);
    Color c;
    PieceType pt = pos.piece_on(Square(1), c);
    EXPECT_EQ(pt, chess::PieceType::Knight);
    EXPECT_EQ(c, chess::Color::White);

    // Remove the piece
    bool removed = pos.remove_piece(Square(1), c, pt);
    EXPECT_TRUE(removed);
    EXPECT_EQ(pt, chess::PieceType::Knight);
    EXPECT_EQ(c, chess::Color::White);
    EXPECT_EQ(pos.piece_on(Square(1), c), chess::PieceType::None);
    EXPECT_FALSE(pos.occupied(Square(1)));

    // Try to remove from an empty square
    Color c2;
    PieceType pt2;
    bool removed2 = pos.remove_piece(Square(1), c2, pt2);
    EXPECT_FALSE(removed2);
}

TEST(PositionTest, Occupancy) {
    using chess::Position;
    using chess::Color;
    using chess::PieceType;
    using chess::Square;

    Position pos;
    // Initially, all squares should be unoccupied
    for (uint8_t sq = 0; sq < 64; ++sq) {
        EXPECT_FALSE(pos.occupied(Square(sq)));
        EXPECT_FALSE(pos.occupied_by(Square(sq), chess::Color::White));
        EXPECT_FALSE(pos.occupied_by(Square(sq), chess::Color::Black));
    }

    // Place a white pawn on a1 (square 0)
    pos.set_piece(Square(0), chess::Color::White, chess::PieceType::Pawn);
    EXPECT_TRUE(pos.occupied(Square(0)));
    EXPECT_TRUE(pos.occupied_by(Square(0), chess::Color::White));
    EXPECT_FALSE(pos.occupied_by(Square(0), chess::Color::Black));
    EXPECT_EQ(pos.piece_on(Square(0)), chess::PieceType::Pawn);
    Color c;
    PieceType pt;
    EXPECT_EQ(pos.piece_on(Square(0), c), chess::PieceType::Pawn);
    EXPECT_EQ(c, chess::Color::White);

    // Place a black pawn on a2 (square 8)
    pos.set_piece(Square(8), chess::Color::Black, chess::PieceType::Pawn);
    EXPECT_TRUE(pos.occupied(Square(8)));
    EXPECT_TRUE(pos.occupied_by(Square(8), chess::Color::Black));
    EXPECT_FALSE(pos.occupied_by(Square(8), chess::Color::White));
    EXPECT_EQ(pos.piece_on(Square(8)), chess::PieceType::Pawn);
    EXPECT_EQ(pos.piece_on(Square(8), c), chess::PieceType::Pawn);
    EXPECT_EQ(c, chess::Color::Black);

    // Check that a1 is still occupied by the white pawn
    EXPECT_TRUE(pos.occupied(Square(0)));
    EXPECT_TRUE(pos.occupied_by(Square(0), chess::Color::White));
    EXPECT_FALSE(pos.occupied_by(Square(0), chess::Color::Black));
    EXPECT_EQ(pos.piece_on(Square(0)), chess::PieceType::Pawn);

    // Remove the white pawn from a1
    pos.remove_piece(Square(0), c, pt);
    EXPECT_FALSE(pos.occupied(Square(0)));
    EXPECT_FALSE(pos.occupied_by(Square(0), chess::Color::White));
    EXPECT_FALSE(pos.occupied_by(Square(0), chess::Color::Black));
    EXPECT_EQ(pos.piece_on(Square(0)), chess::PieceType::None);

    // Remove the black pawn from a2
    pos.remove_piece(Square(8), c, pt);
    EXPECT_FALSE(pos.occupied(Square(8)));
    EXPECT_FALSE(pos.occupied_by(Square(8), chess::Color::White));
    EXPECT_FALSE(pos.occupied_by(Square(8), chess::Color::Black));
    EXPECT_EQ(pos.piece_on(Square(8)), chess::PieceType::None);
}

TEST(PositionTest, Fen) {
    using chess::Position;
    using chess::Color;
    using chess::PieceType;
    using chess::Square;

    // Test the starting position
    const char* start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Position pos(start_fen);

    // Check side to move
    EXPECT_EQ(pos.side_to_move(), chess::Color::White);

    // Check castling rights
    EXPECT_TRUE(pos.castling_rights().white_king_side());
    EXPECT_TRUE(pos.castling_rights().white_queen_side());
    EXPECT_TRUE(pos.castling_rights().black_king_side());
    EXPECT_TRUE(pos.castling_rights().black_queen_side());

    // Check en passant
    EXPECT_FALSE(pos.has_en_passant()); // Now we have a getter

    // Check halfmove clock and fullmove number
    EXPECT_EQ(pos.halfmove_clock(), 0);
    EXPECT_EQ(pos.fullmove_number(), 1);

    // Check a few piece placements
    // White king on e1 (square 4)
    Color c;
    PieceType pt = pos.piece_on(Square(4), c);
    EXPECT_EQ(pt, chess::PieceType::King);
    EXPECT_EQ(c, chess::Color::White);

    // White queen on d1 (square 3)
    pt = pos.piece_on(Square(3), c);
    EXPECT_EQ(pt, chess::PieceType::Queen);
    EXPECT_EQ(c, chess::Color::White);

    // Black king on e8 (square 60)
    pt = pos.piece_on(Square(60), c);
    EXPECT_EQ(pt, chess::PieceType::King);
    EXPECT_EQ(c, chess::Color::Black);

    // Black queen on d8 (square 59)
    pt = pos.piece_on(Square(59), c);
    EXPECT_EQ(pt, chess::PieceType::Queen);
    EXPECT_EQ(c, chess::Color::Black);

    // White pawn on a2 (square 8)
    pt = pos.piece_on(Square(8), c);
    EXPECT_EQ(pt, chess::PieceType::Pawn);
    EXPECT_EQ(c, chess::Color::White);

    // Black pawn on a7 (square 48)
    pt = pos.piece_on(Square(48), c);
    EXPECT_EQ(pt, chess::PieceType::Pawn);
    EXPECT_EQ(c, chess::Color::Black);

    // Check that the FEN string we generate matches the input (up to the spaces we normalized)
    std::string generated = pos.fen();
    // The generated FEN should have the same board placement, side to move, castling rights, en passant, halfmove, fullmove.
    // We can compare the strings directly, but note that our en passant square is set to '-' and we have no en passant.
    // Also, we store castling rights as four booleans, and we output them in the order KQkq.
    // The input FEN has "w KQkq - 0 1", so we expect the same.
    // However, note that our board placement uses empty numbers, which should be the same.
    // We'll do a direct string comparison.
    EXPECT_EQ(generated, start_fen);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}