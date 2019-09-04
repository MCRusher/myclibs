#include "newstd.h"

int main(void)
MANAGE
  puts("What is your name?");
  string name = string_ReadLn();
  println_in("Hello, {}",name);
  puts("How many years old are you?");
  u8 age = ToU8(string_ReadLn());
  puts("What is your sex?");
  string sex = string_ReadLn();
  puts("\nYour profile:");
  println_in("- Name: \"{}\"",name);
  printf("- Age: %d\n",age);
  println_in("- Sex: {}",sex);
UNMANAGE
