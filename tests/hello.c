#include "newstd.h"

int main(void){
  puts("What is your name?");
  string name = string_ReadLn();
  println_in("Hello, {}",name);
  string_Delete(&name);
}
