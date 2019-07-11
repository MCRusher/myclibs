#include "newstd.h"

typedef struct Point {
  i8 x, y;
} Point;

//can be implicitly used in call to print_custom, print_in_custom, and the println variants
string Point_ToString(Point const p) {
  //[----,----]\0
  char tmp[12];
  usize count = snprintf(tmp,12,"(%d,%d)",p.x,p.y);
  char* str = tralloc(count);
  return (string){.data = memcpy(str,tmp,count), .count = count};
}

int main(void){
  f32 pi = 3.14f;
  flong tau = 6.28L;
  char i = ToChar(104.5);
  //println_in and similar allows one argument (no way to use C++ style template pack expansion)
  //to be spliced into a c-string any number of times at the symbol "{}"
  println_in("Pi is {}",pi);
  println_in("One is {}",1);
  println_in("Tau is {}",tau);
  println_in("I is '{}'",i);
MANAGE //sets up a managed allocation scope
  string str = string_Create("Hello, World!");//allocates
  println_in("Str is \"{}\"",str);
  println_in("Str[SPACE] is at index {}",string_Find(str," "));
  u64 n = string_ToU32(string_Create("+18446744073709551615"));//freed at UNMANAGE
  println_in("N is {}",n);
  flong f = string_ToFlong(string_Create("3.14159"));//freed at UNMANAGE
  println_in("F is {}",f);
UNMANAGE //frees scoped allocations at the end of scope, makes no value changes
  //string_Delete(&str); //manual way to release str, properly adjusts state to represent empty string.
  //should be done instead if string will be reused for some reason.

  Point p = {.x = -128, .y = -128};
  //calls user-defined function Point_ToString
  //Type must consist of only one identifier (all structs must be typedef)
  println_in_custom("Point at: {}",p,Point);
}
