# swooce

A program that runs an arbitrary linear filter on an image.

# Dependencies
- OpenCV
- CMake

# Usage
Compile the program like any other CMake project. If you do not know how to,
please leave right now.

```shell
./swooce [kernel] [input] [output]
```

where kernel is a whitespace separated list of numbers in the form

```
W H values... [scale]
```

`W` and `H` have to be non-negative integers, while the `values...` and `[scale]` can be
any real number.  
If scale is absent, the kernel will automatically be scaled so that it sums to 1.

## example:
```
5 5 1 0 0 0 1 0 1 0 1 0 0 0 1 0 0 0 1 0 1 0 1 0 0 0 1
```
or more readable:
```
5 5
1 0 0 0 1
0 1 0 1 0
0 0 1 0 0
0 1 0 1 0
1 0 0 0 1
```

# TODO
- An arg for printing the kernel after normalizing/scaling
- An actual argument parser, so kernel scale can be nicely specified
- Maybe per channel kernel support, or at least the ability to skip channels
- expose filter2D border behaviour

# Trivia
Swooce is a made up word first used in [The Misadventures of Skooks](https://knowyourmeme.com/memes/the-misadventures-of-skooks),
[specifically this scene from episode 2](https://youtu.be/lpRPl8TlVBU?t=112).

This program was originally made to do a X blur similar to
[this kernel in my compton config](https://github.com/udf/dotfiles-stow/blob/c1081df98ec8321b69d9316d1fc7be433df605fb/home/.config/compton.conf#L3-L13) but
for my lock screen, and with a much higher size.