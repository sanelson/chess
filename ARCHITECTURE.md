# Chess Game Architecture Primer

This document provides a simple, robust architecture for building the chess game in C with SDL. The goal is to keep chess rules independent of rendering and user interface code while building one feature at a time.

## Recommended structure

```text
src/
  board.h/.c        # Board state and piece placement
  rules.h/.c        # Chess rules and move generation
  game.h/.c         # Game lifecycle and state transitions
  fen.h/.c          # Optional notation/import/export
  ai.h/.c           # Computer move selection
  render.h/.c       # SDL drawing
  input.h/.c        # SDL mouse and keyboard handling
  config.h          # User-editable configuration
main.c              # SDL setup and the main loop
```

The recommended dependency direction is:

```text
Input → Game → Rules + Board
AI    → Rules + Board
Render → Board/Game state
```

The graphical interface should not perform chess logic. It should only interpret positions and display the result of valid game transitions.

## Core principles

### 1. Keep one authoritative board representation

Represent pieces directly on the board:

```c
typedef enum {
    PIECE_NONE,
    PIECE_PAWN,
    PIECE_KNIGHT,
    PIECE_BISHOP,
    PIECE_ROOK,
    PIECE_QUEEN,
    PIECE_KING
} PieceType;

typedef enum {
    COLOR_NONE,
    COLOR_WHITE,
    COLOR_BLACK
} PieceColor;

typedef struct {
    PieceType type;
    PieceColor color;
} Piece;

Piece board[8][8];
```

Avoid storing piece positions in multiple places. One board array should be the source of truth.

Only add extra state when it is genuinely required, such as:

- castling rights
- en passant target square
- pieces that have moved
- halfmove count
- full move number
- move history

### 2. Separate moves from rules

Define moves as data rather than as separate coordinate arguments:

```c
typedef struct {
    int from_row;
    int from_col;
    int to_row;
    int to_col;
    PieceType promotion;
} Move;
```

The rules engine should provide operations like:

```c
bool rules_generate_moves(const Board *board, PieceColor side,
                          Move *moves, size_t capacity, size_t *count);
bool rules_is_legal(const Board *board, PieceColor side, Move move);
bool rules_is_in_check(const Board *board, PieceColor side);
bool rules_is_checkmate(const Board *board, PieceColor side);
bool rules_is_stalemate(const Board *board, PieceColor side);
```

Generate pseudo-legal moves first, then test them by temporarily applying the move and checking whether the moving king is still in check.

### 3. Make moves transactional

All game moves should pass through one central function:

```c
bool game_apply_move(GameState *game, Move move);
```

This function may:

- validate the move
- update the board
- handle captures
- handle castling
- handle en passant
- handle promotion
- update castling rights
- switch the side to move
- update game status

Neither the renderer nor the AI should change the board directly.

### 4. Keep rules independent of SDL

The rules module should use only C types and memory. It should not depend on windows, textures, events, fonts, or input handling.

This makes it easier to:

- test rules without opening a window
- run AI calculations
- run the game through a terminal
- catch regressions with a test suite

### 5. Use an explicit game state

```c
typedef enum {
    GAME_PLAYING,
    GAME_CHECK,
    GAME_CHECKMATE,
    GAME_STALEMATE,
    GAME_DRAW
} GameStatus;

typedef struct {
    Board board;
    PieceColor turn;
    GameStatus status;
    unsigned move_number;
} GameState;
```

Keep game status derived from board state and move history whenever practical. In-check, checkmate, and stalemate are results of the current position and should not be stored as independent editable fields.

## Initialization

Initialization should build a complete starting position and all of the initial game metadata.

A starting board setup might include:

- place pawns on ranks 2 and 7
- place rooks in the corner squares
- place knights beside each rook
- place bishops beside each knight
- place queens on their matching colored squares
- place kings between the queen and bishop
- set White to move
- enable all castling rights
- clear the en passant target
- set the counters and status to their starting values

Consider creating one initialization function:

```c
void game_init(GameState *game);
```

This should also establish a known, valid state for tests.

## Rules engine

Implement chess rules in small stages:

1. Pawn movement
2. Knight movement
3. Bishop, rook, and queen movement
4. King movement
5. Prevent moves that leave the king in check
6. Check
7. Checkmate
8. Stalemate
9. Castling
10. En passant
11. Promotion
12. Draw conditions

### Move generation

Keep move generation separate from move application:

```text
Generate → Validate → Apply
```

A typical move-validation flow is:

1. Generate pseudo-legal moves for the side to move.
2. Copy or temporarily modify the board.
3. Apply the move to the copy.
4. Reject the move if the moving king is in check.
5. Add the move to the legal move list.

Temporary board copies are useful while the board is small. Optimize only after correctness is established.

### Special rules

Before implementing a chess feature, identify:

1. What state it reads
2. What state it changes
3. What values are invalid
4. How a move can be undone
5. How to test it

#### Castling

Castling depends on:

- king and rook not having moved
- king and rook remaining on their starting squares
- no pieces between the king and rook
- king not currently in check
- king not passing through an attacked square
- king not ending in check

It changes both the king and rook positions and should update castling rights.

#### En passant

En passant depends on:

- a pawn moving two ranks on the immediately previous move
- the capturing pawn being beside it
- the current side being allowed to capture it
- the target square being empty

The en passant target should be cleared after one turn unless the capture is made immediately.

#### Promotion

Promotion should require one of:

- queen
- rook
- bishop
- knight

It changes the pawn type on the destination square. Avoid using the number `PIECE_NONE` or color values as promotion values.

#### Draw conditions

Draws may include:

- stalemate
- insufficient mating material
- fifty-move rule
- threefold repetition
- mutual agreement
- automatic draw rules where applicable

Do not implement every draw rule at once. Add repetition tracking only after move history works.

## Computer player engine

Expose AI behavior through a small interface:

```c
typedef struct {
    unsigned random_seed;
    int skill_level;
} AIOptions;

Move ai_choose_move(const GameState *game,
                    const AIOptions *options);
```

The AI should return a move. It should not modify the game state directly.

### Difficulty progression

Start with one-ply evaluation and gradually add strength:

- **Beginner:** shallow search, random alternatives, frequent weak moves
- **Intermediate:** limited minimax search and basic material evaluation
- **Advanced:** deeper minimax or alpha-beta search

### Personality parameters

Separate AI profile values from chess strength:

```c
typedef struct {
    int skill;
    int impulsiveness;
    int experience;
    int exhaustion;
} AIProfile;
```

Possible meanings:

- **skill:** search depth and evaluation quality
- **impulsiveness:** preference for captures and immediate threats
- **experience:** reduction of known or repeated mistakes
- **exhaustion:** chance of shallow search or a random move
- **wisdom:** preference for strategic concepts learned over time

Use a seeded random number generator so games and tests can be reproduced.

## Graphical interface

The graphical module should have three responsibilities:

- draw the current board state
- convert mouse coordinates into board coordinates
- display input feedback and hints

### Rendering

Keep view data separate from game data:

```text
Game state → Drawing function → SDL textures
Mouse input → Board coordinates → Selection state
```

A piece texture or glyph cache is only a rendering detail. The current board state should remain in `Board` or `GameState`.

### Selection and hints

Possible UI state:

```c
typedef struct {
    bool has_selection;
    int selected_row;
    int selected_col;
} Selection;
```

The UI may:

- highlight the selected square
- highlight legal destinations
- highlight the previous move
- display a check indication
- show invalid-move feedback
- display coordinates
- show promotion choices

Hints should be generated from a copy or through read-only rules/AI APIs. Hints must never modify the live game state.

## Application controller

Keep `main.c` as a coordinator rather than a rules implementation:

```text
handle SDL input
  → update selection
  → ask GameState to validate/apply a move
  → request an AI move when appropriate
  → update the renderer
```

A typical loop can be:

1. Poll SDL events.
2. Convert input into a move or UI action.
3. Ask the game module to process it.
4. If the computer should act, ask the AI for a move.
5. Apply the AI move through the central game function.
6. Render the current state.

Do not put legality checks inside event handling code.

## State and history

### Undo and redo

A simple starting approach is to store complete game-state snapshots:

```c
GameState history[MAX_MOVES];
```

For better memory use, snapshot only before a move and restore it on undo. A single move may also be paired with a reversible board delta.

Start with undo support only after normal move application is reliable. Snapshotting is easier to get correct than incremental reversals.

### Move history

A move record can support:

- undo
- repetition detection
- move notation
- game review
- debugging

```c
typedef struct {
    Move move;
    Piece captured;
    GameState before;
} MoveRecord;
```

The exact structure can evolve. For a first implementation, keep full snapshots or a move list plus enough state to reverse special moves.

## Testing

Test the rules engine independently of SDL.

A test should set up a position, generate or apply moves, and assert an expected state.

Minimum tests:

- starting position has 32 pieces
- pawns move and capture correctly
- knights jump over pieces
- bishops stay on diagonals
- rooks stay on ranks and files
- queens combine rook and bishop movement
- kings cannot move into check
- pinned pieces cannot expose their king
- castling is valid only in legal positions
- en passant is available only for the immediate turn
- promotion creates the requested piece
- checkmate stops the game
- stalemate stops the game
- draw counters update correctly
- repetition detection works where implemented

Use deterministic scenarios instead of random starting positions whenever possible.

## Incremental build plan

### Milestone 1: Display and representation

- Represent the board in C
- Initialize the starting position
- Render pieces and squares
- Add mouse coordinate conversion
- Select and deselect squares

### Milestone 2: Basic movement

- Pawn movement
- Knight movement
- Sliding pieces
- King movement
- Capture handling
- Invalid move feedback

### Milestone 3: Legality

- Generate pseudo-legal moves
- Apply moves to a temporary board
- Reject moves that expose the king
- Detect check
- Detect checkmate
- Detect stalemate

### Milestone 4: Special rules

- Castling
- En passant
- Promotion
- Fifty-move and repetition rules if desired

### Milestone 5: Computer player

- Random legal move player
- Material evaluation
- One-ply search
- Limited minimax
- Alpha-beta pruning
- Difficulty profiles
- Personality parameters

### Milestone 6: Quality improvements

- Move history
- Undo and redo
- FEN loading/saving
- Algebraic notation
- Hints
- Reproducible AI testing
- UI settings

## Configuration

Separate platform-specific resources and user choices:

- font selection
- SDL window dimensions
- starting position
- player types
- AI profile
- board colors
- piece glyph set
- sound settings
- hint settings

Configuration should not affect chess-rule correctness. It should only change how the game is configured or displayed.

## Common mistakes to avoid

- Letting the renderer move pieces
- Letting the AI move pieces directly
- Maintaining piece positions in both an array and a linked list
- Putting castling logic throughout the UI
- Checking checkmate without first testing every legal move
- Considering every move that does not capture a piece legal
- Forgetting to copy or reset en passant state
- Implementing repetition before recording enough history
- Trying to add every rule before testing basic movement
- Optimizing move generation before the rules are correct

## Development checklist

Before completing a feature, ask:

- Can it be tested without SDL?
- Is it applied through the central game API?
- Does it preserve exactly one board representation?
- Does undo/redo remain possible?
- Are invalid moves handled consistently?
- Is the rules engine still independent of the UI?
- Is state reset by `game_init` reproducible?
- Did a test prove the feature works?
