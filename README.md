# myclibs

Collection of C libraries (uses gcc extensions and tested in mingw-w64) that together create a new C ecosystem.

Features include
 - A simple preconfigured setup of the library : newstd.h
    1) Binds allocating library functions to tralloc.
    2) Rebinds stringto.h functions to their main counterparts (ToI8, ToF32, ToChar, etc).
 - "Seedless" RNG for both windows(vista+ with -lbcrypt) and unix(uhh I tested it on onlinegdb and repl.it) : randoming.h
    1) worse portable implementation using rand is available by defining `RANDOMING_USE_RAND` before including it.
    2) should be threadsafe, and rand implementation uses mutexes to ensure this.
 - Threading support for windows via winapi, unix via pthreads, and others via <threads.h> if available (better tested) : threading.h
 - millisecond sleep for windows and unix : sleep_for.h
 - Clearing console (not recommended, uses system, but it does work) support for windows and unix : clear_screen.h
 - simple optional type : option.h
    1) Best way to use it is to typedef it since `option(int) != option(int)` due to quirks of anonymous structs, 
    but with `typedef option(int) option_int`, `option_int == option_int`.
 - size based types and special types (`i8`,`i32`,`f64`,`flong`,etc.) in addition to standard c types with
   macro properties in the form of `i8_MAX`, `f32_MAX`, `flong_NMAX`, etc. : mystd.h
 - a length-based `string` type with relevant functions (better tested now) : stringing.h
 - a bounded length-based string type that uses no heap allocation : ministr.h
 - Allocation system with runtime tracking that allows having managed scopes, and freeing of (and warning of on debug builds)
   memory leaks at program end : tralloc.h
   1) Thread safe by default, can be disabled by defining `TRALLOC_NOTHREADSAFE` before including
 - runtime checked and processed conversions to and from most types with a generic interface
   for base types : tochar.h, to8.h, to16.h, to32.h, to64.h, tof32.h, tof64.h, toflong.h, tostring.h, stringto.h
    1) I give no guarantees on the float conversions, I did my best but it's pretty confusing &
       I used an algorithm from user [mch's answer on Stack Overflow](https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison) that I only partially understand to compare floats with other floats.
    2) tostring.h may allocate new data and should be handled accordingly,
       although string_ToString does not.
    3) tostring.h can be used with `tralloc` via `#define TRALLOC_REPLACE` before include for
       additional safety and usage in managed scopes.
 - Generic printing via `print`, `print_in`, `print_custom`, `print_in_custom`, and `println` variants
   through generic `ToString` conversions.
    1) does not require `TRALLOC_REPLACE`'d tostring.h to be memory safe.
    2) bool is printed as "true" or "false".
    2) char*, string, and bool are basic cases that perform no allocation or copying in conversion.
 - Exception system for handling non-critical failure : exceptions.h
    1) Thread safe by default, can be disabled by defining `EXCEPTIONS_NOTHREADSAFE` before including
    2) uses a string for information and an integer code for matching in a Catch().
    3) unhandled exceptions will raise at either the end of the program or the first call to exit()

Was done for fun and to experiment with `_Generic` and manual runtime memory management.
   
Also I have no clue what I'm doing.
