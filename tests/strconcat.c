#include "newstd.h"

//Returns an allocated string with a set prefix
string PrefixedString(char const * const s){
    //takes 2 string consts and returns a
    //string containing both strings
    //concatenated in the same order they were passed
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
    //frees allocated string
    string_Delete(&s);
}
