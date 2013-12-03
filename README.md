# MPI Examples


## Intro

I needed to learn myself some MPI, and I thought I could share my learnings in
the form of examples. They start being pretty basic, and they become
increasingly complex. They are well commented and explained.


## Building

In order to build all the examples, just use `make`. If you want just one of
them, use `make XX-super_potato`. Each example code is called
`XX-super_potato.c`, and the compiled executable file once you build it will be
called `XX-super_potato`.

If you want to delete all the executable files, just run `make clean`.

_Remember_: You need to have a MPI implementation, such as Open MPI, installed.


## Running

`mpirun -np 4 XX-super_potato`

This will run the `XX-super_potato` example, using 4 processes. If you have a
list of hosts you can connect using SSH, you can add the option `--hostnames
<list>`, where `<list>` is a file with the hostnames you want to connect to,
one on each line.


## License

Copyright (c) 2013 Pablo Caro. All Rights Reserved.

Pablo Caro <<me@pcaro.es>> - <https://pcaro.es/>

See LICENSE file.
