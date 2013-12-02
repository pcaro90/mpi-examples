# MPI Examples


## Intro

Pretty easy to explain. I needed to learn some MPI, and I thought I could share
my learnings.


## Building

To build all the examples, just use `make`. If you want just one of them, use
`make 01-hello_world`.


## Running

`mpirun -np 4 01-hello_world`

This will run the 01-hello_world example, using 4 processes. If you have a list
of hosts you can connect using SSH, you can add the option `--hostnames
<list>`, where `<list>` is a list with the hostnames you want to connect to.

License
-------

Copyright (c) 2013 Pablo Caro. All Rights Reserved.

Pablo Caro <<me@pcaro.es>> - <http://pcaro.es/>

See LICENSE file.
