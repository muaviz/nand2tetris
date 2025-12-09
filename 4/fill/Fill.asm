// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

// Pseudocode:
// kbd = KBD
// LOOP:
//  screen = SCREEN
//  if RAM[kbd] != 0 goto BLACK
//  goto WHITE
// BLACK:
//  if screen = kbd goto LOOP
//  RAM[screen] = -1
//  screen = screen + 1
//  goto BLACK
// WHITE:
//  if screen = kbd goto LOOP
//  RAM[screen] = 0
//  screen = screen + 1
//  goto WHITE

@KBD
D=A
@kbd
M=D

(LOOP)
@SCREEN
D=A
@screen
M=D

@kbd
A=M
D=M
@WHITE
D;JEQ
@BLACK
0;JMP

(BLACK)
@kbd
D=M
@screen
D=D-M
@LOOP
D;JEQ
@screen
A=M
M=-1
@screen
M=M+1
@BLACK
0;JMP

(WHITE)
@kbd
D=M
@screen
D=D-M
@LOOP
D;JEQ
@screen
A=M
M=0
@screen
M=M+1
@WHITE
0;JMP
