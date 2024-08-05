# bf2java
`bf2java` is a [Befunge-93](https://esolangs.org/wiki/Befunge "Befunge-93") to Java ([Jasmin](http://jasmin.sourceforge.net/ "Jasmin")) compiler.
It doesn't insert the original source code into the binary. For this reason, `g` and `p` instructions are not supported.
Instructions like `$` will fail if the stack contains too few elements. Because of this, some programs will have to be modified to push a `0` as their first instruction.

## Makefile

Use `make` or `make bf2java` to build the project.
Use `sudo make install` to install `bf2java`.
Use `sudo make uninstall` to uninstall `bf2java`.
Use `make clean` to clean the working directory.
