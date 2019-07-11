#include "newstd.h"

typedef struct Point {
  i8 x, y;
} Point;

//can be implicitly used in call to print_custom, print_in_custom, and the println variants
string Point_ToString(Point const p) {
  //(----,----)\0
  char tmp[12];
  usize count = snprintf(tmp,12,"(%d,%d)",p.x,p.y);
  char* str = tralloc(count);
  return (string){.data = memcpy(str,tmp,count), .count = count};
}

typedef struct Rect {
  Point p1, p2;
} Rect;

string Rect_ToString(Rect const r) {
  //((----,----),(----,----))
  char tmp[9+4+4+4+4+1];
  usize count = snprintf(tmp,9+4+4+4+4+1,"Rectangle( (%d,%d) to (%d,%d) )",r.p1.x,r.p1.y,r.p2.x,r.p2.y);
  char* str = tralloc(count);
  return (string){.data = memcpy(str,tmp,count), .count = count};
}

u16 Rect_Area(Rect const r){
  i8 x_big, x_small, y_big, y_small;
  if(r.p1.x<r.p2.x){x_big=r.p2.x;x_small=r.p1.x;}else{x_big=r.p1.x;x_small=r.p2.x;}
  if(r.p1.y<r.p2.y){y_big=r.p2.y;y_small=r.p1.y;}else{y_big=r.p1.y;y_small=r.p2.y;}
  return (u16)(x_big - x_small) * (u16)(y_big - y_small);
}

Rect Rect_Square(Point const p, u16 const side_len) {
  if((i32)side_len+(i32)p.x > i8_MAX || (i32)side_len+(i32)p.y > i8_MAX){
	  fputs("p2 out of range.\n",stderr);
	  exit(-1);
  }
  return (Rect){.p1=p, .p2=(Point){.x=(i8)p.x+(i8)side_len, .y=(i8)p.y+(i8)side_len}};
}

int main(void){
  Point const p = {-128,-128};
  println_in_custom("P is {}",p,Point);
  Rect const r = Rect_Square(p,u8_MAX);
  u16 r_area = Rect_Area(r);
  println_in_custom("R is {}",r,Rect);
  printf("Area of r is %u\n",r_area);
}
