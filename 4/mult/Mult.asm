// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

// Pseudocode:
// first = RAM[0]
// second = RAM[1]
// i = 1
// res = 0
// RAM[2] = 0
// LOOP:
//  if i > second goto END
//  res = res + first
//  i = i + 1
//  goto LOOP
// END:
//  RAM[2] = res

@R0
D=M
@first
M=D

@R1
D=M
@second
M=D

@i
M=1

@res
M=0

@R2
M=0

(LOOP)
  @i
  D=M
  @second
  D=D-M
  @END
  D;JGT

  @first
  D=M
  @res
  M=D+M

  @i
  M=M+1

  @LOOP
  0;JMP

(END)
  @res
  D=M
  @R2
  M=D

  @END
  0;JMP
