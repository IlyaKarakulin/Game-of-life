# Game of Life

## Overview

This is a C++ implementation of Conway's **Game of Life**. The game simulates cellular automata, where each cell is either alive or dead, and its state evolves based on its neighbors. The game can be run interactively or automatically for a specified number of iterations.

## Features

- **Interactive Mode**: Step through the game one iteration at a time.
- **Automatic Mode**: Run the game for a set number of iterations.
- **File Support**: Load and save game states to `.live` files.
- **Help**: View command usage with `help`.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/IlyaKarakulin/Game-of-life.git
   cd game-of-life
   ```

2. Building a project:
   ```bash
   cmake -B ./build -S ./
   cmake --build ./build
   ```

3. Launch examples
   ```bash 
   ./build/game
   ```

   ```bash
   ./build/game ./game1.live
   tick
   tick 5
   dump ./out.live
   exit
   ```

   ```bash
   ./build/game ./game1.live --iterations=3 --output=out.live
   ```

   ```bash
   ./build/game ./game1.live -i 3 -o out.live
   ```

   ```bash
   ./build/game ./game1.live -o out.live --iterations=3
   ```

4. Testing \
   It is tested using Google tests, they are downloaded automatically during assembly
   ```bash 
   ./build/tests/live_test
   ```
