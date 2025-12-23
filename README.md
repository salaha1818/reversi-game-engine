# reversi-game-engine

A terminal-based implementation of the Reversi board game written in C.  
Includes full move validation in all 8 directions, piece flipping logic, and an AI move selector using a positional-value heuristic.

## Features
- **Custom board size**: supports any even dimension up to 26×26
- **Board rendering**: prints a coordinate-labeled board using "a..z"
- **Legal move detection**: checks validity in **8 directions** (N, S, E, W, and diagonals)
- **Piece flipping**: flips all sandwiched opponent pieces after a move
- **AI move selection (heuristic)**:
  - prioritizes **corners** heavily
  - discourages squares adjacent/diagonal to corners
  - values edges and “safe” edge squares
  - adds extra weight based on **number of pieces flipped**

## How the AI Chooses a Move
For each legal move, the program computes:
- a **positional score** (corner/edge/near-corner weighting)
- plus a **flip score** (total pieces flipped × 10)

The AI selects the move with the highest total score (ties broken by lowest row/column index).
