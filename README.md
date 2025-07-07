# Checkers

**Checkers AI in C++**

This project implements a simplified Checkers (Draughts) game with AI using Minimax and Alpha-Beta pruning algorithms. The AI plays as Black, while the human (or another program) can play as White by extending the input logic.

---

## Features

* **Game Representation**: 8x8 board with standard Checkers piece encoding:

  * `-1`: unusable square (dark squares only)
  * `0` : empty square
  * `1` : White man (pawn)
  * `2` : Black man (pawn)
  * `3` : White king
  * `4` : Black king

* **Move Generation**

  * Generates all legal moves, including forced captures and multi-jumps.
  * Supports promotion to king when a man reaches the far side.

* **Evaluation Function (`static_value`)**

  * Material count (pawns vs kings).
  * Positional bonuses depending on midgame/endgame phases.
  * Mobility (number of legal moves) and clustering penalties in endgame.

* **Search Algorithms**

  * **Minimax** (with optional tracking of expanded nodes).
  * **Alpha-Beta Pruning** (default at depth = `9`).

* **Metrics**

  * Counters for Minimax and Alpha-Beta node evaluations (`minimax_counter`, `alphabeta_counter`).

---

## Getting Started

### Prerequisites

* A C++ compiler supporting C++11 or later (e.g., `g++`, `clang++`).

### Building

```bash
# Compile the program
g++ -std=c++11 Checkers.cpp -o checkers_ai
```

### Running

```bash
# Launch the AI vs Human (White) Checkers game�33[0m
./checkers_ai
```

1. The board will be printed to the console in ASCII format, with `W`/`B` for men and `KW`/`KB` for kings. Empty dark squares are shown as `-` and unused light squares as `*`.
2. When it’s White’s turn (starting player), implement input logic for human moves by reading coordinates (e.g., `x1 y1 x2 y2`) and applying `commit_move`.
3. On Black’s turn, the AI computes the best move using Alpha-Beta and displays feedback:

   * `Best Move!`, `Good Move!`, `Mistake!`, `Blunder!`, or `Brilliant Move!` based on evaluation changes.
4. The game ends when one side has no movable pieces or no legal moves (win/loss), or a draw if desired.

---

## Code Structure

* `game`, `motion`, `tree`, and `Move` structs define the board state, moves, and search tree nodes.
* **Move Generation & Validation**: `get_valid_moves`, `total_get_valid_moves`, `allcaptures`.
* **Game Logic**: `commit_move`, `further_capture`, `print_board`, `init_board`.
* **Evaluation**: `static_value` covers opening/mid/endgame heuristics.
* **Search**:

  * `minimax(tree*, bool, depth)`
  * `alphabeta(tree*, bool, depth, alpha, beta)`
  * Overloaded wrappers for simpler calls.

---

## Configuration

* **`depth_search`**: Set the search depth (default `9`). Higher depth increases playing strength at the cost of performance.
* **`infi`**, **piece values**, and **heuristic weights** can be tuned in `static_value()`.

---

## Limitations & Future Work

* **User Input**: Current code does not include human input parsing—needs extension for interactive play.
* **Draw Detection**: No draw by repetition or 50-move rule.
* **Performance**: Single-threaded; potential for multi-threading or transposition table (Zobrist hashing).
* **GUI**: Integration with a graphical interface or network play.

---

## License

This project is provided under the MIT License. Feel free to use, modify, and distribute.
