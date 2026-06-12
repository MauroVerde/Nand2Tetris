// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

(MAINLOOP)
@8192    //total screen size
D=A

@LOOP
M=D
@SCREEN
D=A-1
@DynScreen
A=D

(LOOP) 
@DynScreen
D=D+1
A=D
M=0
@LOOP
M=M-1
@LOOP
M;JGT


@8192
D=A
@LOOP2
M=D

@KBD
D=M


@MAINLOOP
D;JEQ

@SCREEN
D=A-1
@DynScreen2
A=D

(LOOP2)
@DynScreen2
D=D+1
A=D
M=-1

@LOOP2
M=M-1

@LOOP2
M;JGT

(LOOP3)
@KBD
D=M
@LOOP3
D;JGT

@MAINLOOP
0;JMP
