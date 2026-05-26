# C-Lite Compiler Project 🚀

## Overview

C-Lite is a mini compiler design project built using Python, Lex/Flex, and Yacc/Bison. The project provides a graphical interface for writing, compiling, and testing C programs while demonstrating different phases of compiler construction such as lexical analysis, parsing, and symbol table handling.

The project is designed for learning and demonstrating core compiler concepts in a practical way with an easy-to-use desktop application.

---

# Features ✨

* 🖥️ Interactive GUI built with Python Tkinter
* ✍️ Built-in C code editor
* ⚡ Execute compiler phases directly from the interface
* 🔍 Lexical Analysis using Flex/Lex
* 🌳 Syntax Analysis using Yacc/Bison
* 📑 Symbol Table generation and management
* 📂 Multiple compiler phase modules (P1, P2, P3, P4)
* 🧵 Multithreading support for smooth execution
* 📜 Real-time output console for execution results
* 🎨 User-friendly and clean interface

---

# Technologies Used 🛠️

| Technology     | Purpose                   |
| -------------- | ------------------------- |
| Python         | GUI & automation          |
| Tkinter        | Desktop interface         |
| Flex/Lex       | Lexical analysis          |
| Yacc/Bison     | Syntax parsing            |
| GCC            | Compilation               |
| C Language     | Test programs             |
| Multithreading | Smooth execution handling |

---

# Project Structure 📁

C-Lite/
│
├── fixed_compiler.py      # Main GUI application
├── README.md              # Project documentation
├── test.c                 # Sample C test file
├── test1.c
├── test2.c
├── test3.c
├── test4.c
│
├── P1/                    # Lexical analysis phase
├── P2/                    # Parser + Symbol table
├── P3/                    # Advanced compiler phase
├── P4/                    # Additional compiler implementation
│
└── structure.txt          # Project structure details

---

# How It Works ⚙️

1. User writes or loads C code into the editor.
2. The selected compiler phase (P1/P2/P3/P4) is executed.
3. Flex performs lexical analysis.
4. Bison/Yacc performs syntax parsing.
5. GCC compiles generated files.
6. Output is displayed in the integrated console.

---

# Installation & Setup 💻

## Prerequisites

Make sure the following are installed:

* Python 3.x
* GCC Compiler
* Flex/Lex
* Bison/Yacc

---

## Clone Repository

git clone <your-repository-link>
cd C-Lite

---

## Run the Project

python fixed_compiler.py

---

# Sample Workflow ▶️

1. Open the application.
2. Write or load a C program.
3. Select a compiler phase.
4. Execute the program.
5. View tokens, parser results, and outputs.

---

# Learning Objectives 📚

This project demonstrates:

* Compiler Design Fundamentals
* Lexical Analysis
* Syntax Parsing
* Symbol Table Handling
* GUI-based Compiler Simulation
* Integration of Python with Compiler Tools

---

# Future Improvements 🚀

* Add semantic analysis
* Add syntax tree visualization
* Improve error handling
* Add dark mode UI
* Cross-platform support
* Code optimization phase

---

# Author 👩‍💻

Apeksha Tiwari
B.Tech CSE Student | Compiler Design & Software Development Enthusiast

---

---

# License 📄

This project is developed for educational and learning purposes.
