# myclibs

Collection of C libraries (uses gcc extensions and tested in mingw-w64) that together create a new C ecosystem.

Features include
 - "Seedless" RNG for both windows(vista+ with -lbcrypt) and unix(uhh I tested it on onlinegdb and repl.it) : random.h
 - Threading support for windows, unix, and others via <threads.h> if available (rough & not well tested) : threading.h
 - millisecond sleep for windows and unix : sleep_for.h
 - very rough and simple optional type : option.h
 - size based types , (i8,i32,f64,flong,etc.) in addition to standard c types with
   macro properties in the form of "i8_MAX", "f32_MAX", "flong_NMAX", etc. : mystd.h
 - a length-based string type with relevant functions (not well tested) : stringing.h
 - Allocation system with runtime tracking that allows having managed scopes, and freeing of (and warning of on debug builds)
   memory leaks at program end.
 - runtime checked and processed conversions to and from most types with a generic interface
   for base types : tochar.h, to8.h, to16.h, to32.h, to64.h, tof32.h, tof64.h, toflong.h, tostring.h, stringto.h
    1) tostring.h may allocate new data and should be handled accordingly,
       although string_ToString does not.
    2) tostring.h can be used with tralloc via "#define TRALLOC_REPLACE" before include for
       additional safety and usage in managed scopes.
 - Generic printing via print, print_in, print_custom, print_in_custom, and println variants
   through generic ToString conversions.
    1) has compatibility for TRALLOC_REPLACE'd tostring.h (in the same method of overloading tostring.h)
       which ensures no memory leaks after ToString conversion is performed
 - A header which combines all of the above and splices stringto.h conversions into all the other generic conversions : newstd.h
   
   Was done for fun and to experiment with _Generic and manual runtime memory management.
