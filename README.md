# Wordle Solver in C

This project implements a Wordle solver written in the C programming language.
It was developed as part of the ALGO3 course (L2 ISIL C).

The solver uses feedback from previous guesses (gray, yellow, green) to eliminate
invalid words and progressively find the correct solution.

---

## Features
- Solves the Wordle game using logical filtering
- Interactive user mode (manual feedback input)
- Uses a dictionary of valid 5-letter words
- Efficient elimination of impossible words

---

## Project Structure
- wordlesolvergameinc.c : Main source file containing the solver logic
- words.txt             : Dictionary of valid 5-letter words
- README.md             : Project documentation

---

## Compilation
Use GCC to compile the program:

```bash
gcc wordlesolvergameinc.c -o wordle
