# Chess Game Development Backlog

A staged backlog for building the chess game in C while keeping the implementation manageable and testable.

The backlog is intentionally ordered from foundation to polish. Complete each item before moving on. The goal is to improve C skills through small, working increments rather than attempting to build everything at once.

## How to use this backlog

For each item:

1. Move it into the current milestone.
2. Define the C types and API before writing the implementation.
3. Write or update a small test or reproducible scenario.
4. Implement only the behavior required by that item.
5. Check that existing features still work.
6. Commit a working increment.

Do not move to a later item merely because it sounds easier or more interesting. Later features become much more reliable when the board and move-validation foundation is solid.

## Backlog status labels

- **P0:** Required foundation
- **P1:** Core game behavior
- **P2:** Gameplay quality and AI
- **P3:** Polish

```text
Status: Not started | In progress | Blocked | Complete
```

## Epic 1: Project and board foundation

### 1.1 Create the board module

**Priority:** P0  
**Status:** Not started

Create `board.h` and `board.c` with:

- piece types
- piece colors
- a board representation
- a piece lookup function
- a board initialization function

Acceptance criteria:

- [ ] `Board` or equivalent represents all 64 squares.
- [ ] Starting-piece setup is defined in code.
- [ ] There is no SDL dependency in the board module.
- [ ] A clean build succeeds.
- [ ] A small test can initialize the board and verify the piece at a square.

Recommended first API:

```c
void board_init(Board *board);
Piece board_get_piece(const Board *board, int row, int col);
void board_set_piece(Board *board, int row, int col, Piece piece);
void board_clear_square(Board *board, int row, int col);
```

### 1.2 Add board coordinates and move data

**Priority:** P0  
**Status:** Not started

Create consistent coordinate conventions and a `Move` structure.

Acceptance criteria:

- [ ] One coordinate convention is documented.
- [ ] Row and column ranges are checked.
- [ ] `Move` includes source, destination, and optional promotion.
- [ ] Invalid coordinates are handled without memory corruption.

### 1.3 Initialize the starting game state

**Priority:** P0  
**Status:** Not started

Create `GameState` initialization in `game.h` and `game.c`.

Acceptance criteria:

- [ ] A new game contains exactly 32 pieces.
- [ ] 16 pieces belong to White and 16 to Black.
- [ ] White is the initial side to move.
- [ ] En passant state begins empty.
- [ ] Castling rights begin enabled for all eligible pieces.
- [ ] Calling initialization repeatedly produces a known starting state.

---

## Epic 2: Rules engine foundation

### 2.1 Implement piece-square and bounds helpers

**Priority:** P0  
**Status:** Not started

Create defensive helpers for:

- checking square bounds
- checking whether a square is occupied
- checking whether two pieces are opponents
- finding a king
- copying a board for testing and validation

Acceptance criteria:

- [ ] No board indexing occurs outside the `[0, 7]` range.
- [ ] Helper behavior is covered by tests.
- [ ] The rules module has no SDL dependencies.

### 2.2 Implement pawn moves

**Priority:** P0  
**Status:** Not started

Acceptance criteria:

- [ ] Pawns move forward one square.
- [ ] Pawns move forward two squares from their starting rank only.
- [ ] Pawns cannot move forward into an occupied square.
- [ ] Pawns capture diagonally only.
- [ ] Pawns cannot capture an empty square normally.
- [ ] A pawn cannot move backward.

Test scenarios:

- initial White pawn movement
- initial Black pawn movement
- blocked pawn
- normal capture
- pawn blocked by an occupied target square

### 2.3 Implement knight moves

**Priority:** P0  
**Status:** In progress / Next

Acceptance criteria:

- [ ] All eight knight offsets are generated.
- [ ] Knights jump over occupied squares.
- [ ] Knights cannot capture their own pieces.
- [ ] The move list excludes off-board squares.

### 2.4 Implement bishop movement

**Priority:** P0  
**Status:** Not started

Acceptance criteria:

- [ ] Bishop moves stop at the board edge.
- [ ] Bishop moves stop at friendly pieces.
- [ ] Bishop captures enemy pieces by stopping on that square.
- [ ] The bishop cannot continue past a friendly or enemy piece.

### 2.5 Implement rook movement

**Priority:** P0  
**Status:** Not started

Acceptance criteria:

- [ ] Rook moves along ranks and files.
- [ ] Friendly pieces block movement.
- [ ] Enemy pieces stop movement after being included as a capture.
- [ ] The rook cannot continue beyond the first occupied square.

### 2.6 Implement queen movement

**Priority:** P0  
**Status:** Not started

Acceptance criteria:

- [ ] Queen movement combines rook and bishop behavior.
- [ ] Friendly pieces block movement.
- [ ] Captures stop movement in the expected direction.

### 2.7 Implement king movement

**Priority:** P0  
**Status:** Not started

Acceptance criteria:

- [ ] Kings move one square in all directions.
- [ ] Kings cannot capture friendly pieces.
- [ ] Kings cannot move into an attacked square.
- [ ] Castling is not included until its dedicated milestone.

### 2.8 Separate pseudo-legal and legal move generation

**Priority:** P0  
**Status:** Not started

Implement:

- pseudo-legal piece movement
- temporary board application
- king safety validation
- legal move generation

Acceptance criteria:

- [ ] Pinned pieces are excluded when their move exposes the king.
- [ ] A side in check can only return moves that remove the check.
- [ ] A move cannot leave or place the moving king in check.
- [ ] `rules_generate_legal_moves` returns a valid array/count.

### 2.9 Centralize move application

**Priority:** P0  
**Status:** Not started

Create one move application API used by the UI and AI.

Acceptance criteria:

- [ ] Invalid moves do not mutate the game state.
- [ ] Valid moves update the board.
- [ ] Captures remove the captured piece.
- [ ] The side-to-move changes only after a successful move.
- [ ] Move application is separate from rendering and input handling.

---

## Epic 3: Complete game rules

### 3.1 Detect check

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] Attack detection does not depend on move generation that rejects king exposure incorrectly.
- [ ] Kings can attack adjacent squares.
- [ ] Pinned enemy pieces are not considered legal attackers.
- [ ] Check is detected after every move.

### 3.2 Detect checkmate

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] Checkmate requires both check and no legal moves.
- [ ] The game status becomes checkmate.
- [ ] The side to move cannot continue playing.

### 3.3 Detect stalemate

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] Stalemate requires no check and no legal moves.
- [ ] The game status becomes stalemate.
- [ ] The side to move cannot continue playing.

### 3.4 Implement castling

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] Castling requires unmoved king and rook pieces.
- [ ] Squares between king and rook are empty.
- [ ] The king is not in check.
- [ ] The king does not pass through check.
- [ ] The king cannot end in check.
- [ ] The move updates both piece positions.
- [ ] Castling rights are cleared when appropriate.

### 3.5 Implement en passant

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] En passant is offered only after a double pawn advance.
- [ ] The target exists for only one turn.
- [ ] The correct pawn is captured.
- [ ] The target is cleared after the turn.
- [ ] The move cannot expose the capturing pawn's king.

### 3.6 Implement promotion

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] Promotion is offered on the final rank.
- [ ] The player can choose queen, rook, bishop, or knight.
- [ ] The pawn is replaced by the selected piece.
- [ ] Promotion moves participate in king-safety validation.

### 3.7 Implement draw conditions

**Priority:** P1  
**Status:** Not started

Start with the easiest independent conditions:

- [ ] fifty-move rule
- [ ] insufficient mating material
- [ ] threefold repetition

Acceptance criteria:

- [ ] Draw counters are updated correctly.
- [ ] Repetition uses immutable position history or a defined position key.
- [ ] Draw status is separate from checkmate.

---

## Epic 4: Graphical interface

### 4.1 Create a rendering module

**Priority:** P1  
**Status:** Not started

Move all board and piece drawing into `render.c`.

Acceptance criteria:

- [ ] Rendering reads game state only.
- [ ] Rendering does not mutate board state.
- [ ] Square sizes are defined consistently.
- [ ] Board origin and piece positioning are documented.

### 4.2 Convert mouse input to board squares

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] SDL mouse coordinates map to valid board rows and columns.
- [ ] Out-of-board input is handled safely.
- [ ] One center point can be used as a stable test value.

### 4.3 Add piece selection

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] Selecting a piece stores the square.
- [ ] Selecting a legal destination creates a move.
- [ ] Selecting another own piece changes the selection.
- [ ] An invalid destination clears or preserves selection according to a documented rule.

### 4.4 Add board highlighting

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] Selected squares are highlighted.
- [ ] Legal destination squares are highlighted.
- [ ] Last-move squares are distinguishable.
- [ ] Check state is visually indicated.

### 4.5 Add input feedback and promotion UI

**Priority:** P2  
**Status:** Not started

Acceptance criteria:

- [ ] Invalid moves provide visible feedback.
- [ ] Promotion offers four clear choices.
- [ ] Keyboard and mouse behavior are documented.

---

## Epic 5: Game history and notation

### 5.1 Add move history

**Priority:** P2  
**Status:** Not started

Acceptance criteria:

- [ ] Every successful move is recorded.
- [ ] The history contains enough state to undo special moves.
- [ ] Invalid moves are not recorded.

### 5.2 Add undo and redo

**Priority:** P2  
**Status:** Not started

Acceptance criteria:

- [ ] Undo restores the previous board and metadata.
- [ ] Redo restores an undone move.
- [ ] Undo is unavailable when history is empty.
- [ ] Starting a new move after undo clears the redo stack.

### 5.3 Add algebraic or portable notation

**Priority:** P2  
**Status:** Not started

Choose one incremental format:

- coordinates for easy debugging
- SAN for human-readable notation
- FEN for position loading/saving

Acceptance criteria:

- [ ] One complete notation format is documented.
- [ ] Parsing and writing round-trip through valid positions.
- [ ] Invalid notation fails safely.

---

## Epic 6: Computer player

### 6.1 Add a random legal-move player

**Priority:** P2  
**Status:** Not started

Acceptance criteria:

- [ ] The AI uses only legal moves.
- [ ] Random selection is reproducible with a seed.
- [ ] The AI never mutates the live board directly.
- [ ] The AI returns a `Move` or an explicit no-move result.

### 6.2 Add material evaluation

**Priority:** P2  
**Status:** Not started

Acceptance criteria:

- [ ] Pawn, knight, bishop, rook, queen, and king values are defined.
- [ ] Evaluation is from the AI player's perspective or explicitly normalized.
- [ ] Evaluation is deterministic.
- [ ] Tactics have no hidden SDL dependency.

### 6.3 Add one-ply search

**Priority:** P2  
**Status:** Not started

Acceptance criteria:

- [ ] The AI evaluates every legal move.
- [ ] The best legal move is returned for a fixed seed/evaluation.
- [ ] Captures are not required to win automatically without a clear test.

### 6.4 Add minimax

**Priority:** P2  
**Status:** Not started

Acceptance criteria:

- [ ] Search depth is configurable.
- [ ] Both sides are modeled correctly.
- [ ] Terminal states are evaluated.
- [ ] Alpha-beta pruning is added only after plain minimax works.

### 6.5 Add difficulty profiles

**Priority:** P2  
**Status:** Not started

Suggested levels:

- [ ] Beginner: shallow search and mistakes
- [ ] Intermediate: moderate search
- [ ] Advanced: deeper search and evaluation

Acceptance criteria:

- [ ] Profiles are data, not separate event handlers.
- [ ] Difficulty settings can be selected in configuration.

### 6.6 Add personality parameters

**Priority:** P3  
**Status:** Not started

Consider separate values for:

- impulsiveness
- experience
- exhaustion
- risk tolerance
- randomness

Acceptance criteria:

- [ ] The parameters affect documented choices.
- [ ] The same seeded profile produces reproducible behavior.
- [ ] Personality does not bypass legal move generation.

---

## Epic 7: Testing and quality

### 7.1 Build a rules test runner

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] Tests can run without SDL.
- [ ] Tests assert board state and legal moves.
- [ ] Test positions are easy to read and construct.
- [ ] The test command is documented in `README.md` or `Makefile`.

### 7.2 Add regression tests for every special rule

**Priority:** P1  
**Status:** Not started

Acceptance criteria:

- [ ] Castling legality has positive and negative cases.
- [ ] En passant has positive and expired-target cases.
- [ ] Promotion tests cover all piece choices.
- [ ] Checkmate and stalemate test distinct positions.
- [ ] A pinned piece cannot expose its king.

### 7.3 Add AI tests

**Priority:** P2  
**Status:** Not started

Acceptance criteria:

- [ ] The AI only chooses legal moves.
- [ ] A forced checkmate move is found within the supported search depth.
- [ ] A hanging-piece capture is detected where the test depth allows it.
- [ ] Seeded behavior is reproducible.

### 7.4 Add fuzz or invariant tests

**Priority:** P3  
**Status:** Not started

Acceptance criteria:

- [ ] Move application preserves board bounds.
- [ ] Piece totals never become negative.
- [ ] The king count remains valid for normal game states.
- [ ] Invalid moves do not mutate state.

---

## Epic 8: Configuration and polish

### 8.1 Improve font selection

**Priority:** P2  
**Status:** In progress

Acceptance criteria:

- [ ] Platform defaults are documented.
- [ ] An explicit override is supported.
- [ ] Missing-font errors include actionable guidance.

### 8.2 Add configuration options

**Priority:** P3  
**Status:** Not started

Possible options:

- player vs. player or player vs. computer
- AI difficulty
- board colors
- square coordinates
- hints
- font override
- sound

### 8.3 Add polished game controls

**Priority:** P3  
**Status:** Not started

Possible controls:

- new game
- undo
- redo
- quit
- save/load position
- show legal moves
- restart

### 8.4 Improve error handling

**Priority:** P2  
**Status:** Not started

Acceptance criteria:

- [ ] Initialization failures print a clear reason.
- [ ] Invalid moves do not crash.
- [ ] Allocation failures are handled where applicable.
- [ ] SDL and TTF setup failures are reported separately.

## Recommended first sprint

For a strong first implementation cycle:

1. Create `board.h` and `board.c`.
2. Add `game_init` and starting-position tests.
3. Add bounds and square helpers.
4. Implement pawns.
5. Implement knights.
6. Implement bishops, rooks, and queens.
7. Implement kings.
8. Separate pseudo-legal and king-safe legal moves.
9. Move SDL drawing into a renderer module.
10. Allow selecting a piece and applying a move through `game_apply_move`.

## Recommended next-sprint checkpoint

Before starting the AI, verify:

- [ ] the board has a single source of truth
- [ ] every move passes through one game API
- [ ] pseudo-legal and legal moves are clearly separate
- [ ] check is detected in tests
- [ ] checkmate and stalemate are distinct
- [ ] the rules module builds and tests without SDL

## Definition of done

A backlog item is complete only when:

- [ ] the code builds cleanly
- [ ] behavior is tested with a reproducible scenario
- [ ] no unrelated features are included
- [ ] the implementation matches the module boundaries
- [ ] comments explain non-obvious chess or C decisions
- [ ] the README or architecture primer is updated if public behavior changed

## GitHub backlog maintenance for future agents

The implementation backlog is documented here and mirrored in GitHub Issues. Before creating or updating GitHub items, inspect the current state so work is not duplicated.

### Inspect repository state

```sh
gh repo view --json nameWithOwner,hasIssuesEnabled
gh label list
gh issue list --state all --limit 100
gh api repos/{owner}/{repo}/milestones
```

Replace `{owner}/{repo}` with the value returned by `gh repo view --json nameWithOwner` or use `gh api repos/$(gh repo view --json nameWithOwner --jq .nameWithOwner)/milestones`.

### Create a label or milestone only when missing

```sh
gh label create P0-foundation --color 555555 --force
gh label create P1-core --color d93f0b --force
gh label create P2-quality --color 0052cc --force
gh label create P3-polish --color 6f42c1 --force
gh label create board --color 1d76db --force
gh label create rules --color b60205 --force
gh label create rendering --color 5319e7 --force
gh label create testing --color 0e8a16 --force
gh label create epic --color fbca04 --force

gh api --method POST repos/{owner}/{repo}/milestones -f title="M1 - Board foundation"
```

Before creating an issue, verify that no issue with the same title or purpose already exists:

```sh
gh issue list --search '"exact issue title"' --state all
```

### Create an issue from this backlog

Convert one backlog item into one issue. Use the issue title format `module: concise behavior`, and include summary, acceptance criteria, tests, and module impact.

```sh
gh issue create \
  --title "rules: implement castling" \
  --body-file /tmp/issue.md \
  --label "P1-core,rules,testing" \
  --milestone "M4 - Special rules"
```

The repository currently has starter issues #1–#10, plus milestones `M1 - Board foundation`, `M2 - Basic pieces`, and `M3 - Legal move foundation`. Future agents should use the backlog items below to create additional focused issues rather than recreating the starter items.

### Keep issue status synchronized

```sh
gh issue edit 4 --add-label "P0-foundation"
gh issue edit 4 --add-assignee "@me"
gh issue close 4 --comment "Implemented and verified by tests."
```

A future agent may close an issue only after the acceptance criteria are met and the code builds successfully.

### Recommended synchronization rule

- **BACKLOG.md:** detailed local sequence and acceptance criteria
- **GitHub Issues:** small, independently implementable work units
- **Pull requests:** implementation and verification evidence
- **GitHub milestone:** the implementation stage containing the issue

Do not maintain duplicate detailed descriptions in multiple places. The backlog should remain the source for planned work, while GitHub Issues track assignment and current status.
