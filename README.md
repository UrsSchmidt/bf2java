# bf2java
`bf2java` is a [Befunge-93](https://esolangs.org/wiki/Befunge "Befunge-93") to Java ([Jasmin](http://jasmin.sourceforge.net/ "Jasmin")) compiler.
It doesn't insert the original source code into the binary.
`g` and `p` instructions are therefore not supported.
Instructions like `$` will fail if the stack contains too few elements, because of this, some examples were adjusted to explicitly push a `0` before execution.
