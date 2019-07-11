#include "newstd.h"

typedef struct Person {
  char* name;
  u8 age;
  bool male;
} Person;

//constructor rand_start is called
int main(void){
  Person p;
  rand_abstract(&p);
  p.name = "Joy Price";
  //becuase _Bool is a thinly veiled uchar, filling the memory at a bool results in
  //values almost always above 0.
  //In this example sex will almost always be male.
  printf("name: %s\nage: %u\nsex: %s\n\n",p.name,p.age,p.male ? "male" : "female");

  Person p2;
  p2.name = "Jay Pippin";
  //age will not exceed 120yrs
  p2.age = randb_u8(0,120);
  //male will always be 1 or 0
  p2.male = rand_bool();
  //In this example the values are properly formatted and sex is likely to be
  //male or female with about equal chance.
  printf("name: %s\nage: %u\nsex: %s\n",p2.name,p2.age,p2.male ? "male" : "female");
}
//destructor rand_end is called
