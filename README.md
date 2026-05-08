# Reversi Game Engine

A terminal-based implementation of the classic Reversi (Othello) board game written in C. The project features full move validation, piece flipping mechanics, and an AI opponent that selects moves using heuristic-based decision making.

## Features

- Fully playable Reversi game in the terminal
- Dynamic board sizes supporting any even dimension up to 26×26
- Coordinate-labeled board rendering using alphabetical indexing
- Legal move validation in all 8 directions
- Automatic piece flipping after valid moves
- AI opponent with heuristic-based move selection
- Corner and edge prioritization strategies
- Move evaluation based on positional value and flip count

## Technologies Used

- C
- Terminal / console-based rendering
- Heuristic algorithms
- Grid-based game logic

## Gameplay

Players take turns placing pieces on the board to capture opponent pieces by surrounding them horizontally, vertically, or diagonally. When a valid move is made, all enclosed opponent pieces are flipped to the current player's color.

The game continues until no legal moves remain, and the winner is determined by the number of pieces controlled at the end of the match.

## AI Logic

The AI evaluates every legal move using a heuristic scoring system based on board position and strategic value.

### Positional Heuristics
- Strongly prioritizes corner positions
- Prefers stable edge positions
- Avoids risky squares adjacent to corners
- Evaluates board safety and control

### Move Evaluation
Each move score is calculated using:
- Positional weighting
- Number of opponent pieces flipped

The AI selects the move with the highest overall score.

## Project Highlights

This project focused on algorithm design, game state management, and heuristic-based AI decision making. Major challenges included implementing efficient move validation across all directions, managing dynamic board states, and designing an AI capable of making strategically strong moves.
