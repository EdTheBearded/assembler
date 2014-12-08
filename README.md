## Simple MIPS assembler ##
**Made using FLEX/BISON and C.**

### Remarks ###
Implemented instructions on *types.txt*, also grouped by their "subtypes" under R/I/J-Types. Error handling was treated following [Christian Hagen's article](http://www.ibm.com/developerworks/library/l-flexbison/).

**Compilation:** <pre>$ make</pre>
As simple as that.

**Usage:** <pre>$ ./asm *infile* [-b|-h] [-o *outfile*]</pre>
**-b** activate debugging info. (still needs work)  
**-h** print help message  
**-o** specify output file name to *outfile*, if not specified, file will be named *a* by default.


### What happened ###
* **v1.0**  
Small MIPS assembler, following the syntax showed in *types.txt*. This program translates assembly code directly to hex, as simple as that. Small error detection (syntax, semantics and such), with friendly error messages. Some test cases can be found on *test/* folder (found some code somewhere on the internet).

* **v0.1**  
Now with error handling and helpful error messages.
Still no binary though.

* **v0.0**  
Parsing text and checking syntax. Still pretty simple and errors are not handled.
Still isn't generating binary code.
Makefile included.<br>