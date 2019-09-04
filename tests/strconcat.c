#include "newstd.h"

string PrefixedString(char const * const s){
    return string_Combine(
        //creates string without allocation,
        //result cannot safely be modified
        string_CreateConst("!: "),
        string_CreateConst(s)
    );
}

int main(void){
    string s = PrefixedString("Hey");
    println(s);
    string_Delete(&s);
}