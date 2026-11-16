/* Configuration */

struct piece {
    int type;
    int color;
    char *start_position;
};

/*
 * Unicode chess board reference (a8 at top-left):
 *
 *       a      b      c      d      e      f      g      h
 *  8  ♜◼️  ♞◼️  ♝◼️  ♛◼️  ♚◼️  ♝◼️  ♞◼️  ♜
 *  7  ♟◼️  ♟◼️  ♟◼️  ♟◼️  ♟◼️  ♟◼️  ♟◼️  ♟
 *  6  ◼️  ◻️  ◼️  ◻️  ◼️  ◻️  ◼️  ◻️
 *  5  ◻️  ◼️  ◻️  ◼️  ◻️  ◼️  ◻️  ◼️
 *  4  ◼️  ◻️  ◼️  ◻️  ◼️  ◻️  ◼️  ◻️
 *  3  ◻️  ◼️  ◻️  ◼️  ◻️  ◼️  ◻️  ◼️
 *  2  ♙◼️  ♙◼️  ♙◼️  ♙◼️  ♙◼️  ♙◼️  ♙◼️  ♙
 *  1  ♖◼️  ♘◼️  ♗◼️  ♕◼️  ♔◼️  ♗◼️  ♘◼️  ♖
 *
 *  White pieces: ♔ ♕ ♖ ♗ ♘ ♙
 *  Black pieces: ♚ ♛ ♜ ♝ ♞ ♟
 *
 *  The alternating square pattern is shown with ◻️/◼️ helpers; actual board
 *  cells are typically empty until a piece is placed on them.
 */

// Board
