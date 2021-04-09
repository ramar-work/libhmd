# libhmd 

libhmd is a Markdown module for Hypno 
( <a href="https://github.com/zaiah-dj/hypno">https://github.com/zaiah-dj/hypno</a> )


## How does it work?

libhmd relies on Discount 
(<a href="https://github.com/Orc/discount">https://github.com/Orc/discount</a>), a 
C library written to interpret Markdown files.  
The easiest way to get it going is to grab it from your package manager (which may
be a bit different for each of the different distros and Mac OSX).  You can also
build it yourself.


## Building 

The default Makefile builds a shared object.
Run `make` with no arguments at the top level to build it.

- Compile the library (should just need `make` at the command line and you're good to go).

- Copy it to whereever you have your other modules.


## Files & Directories


