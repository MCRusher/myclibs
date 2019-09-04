#include "newstd.h"
int main(void){
    //uses mydef.h's bool values,
    //so true is actual of type bool
    bool b = true;
    //uses randoming.h's functions to
    //generate number between 0 and 100
    i32 secret = randb_i32(0,100), guess;
    while(b){
        //print flushes the stream, so it is
        //guaranteed to be displayed after the call
        print("Guess the number(0-100): ");
        //reads a line from stdin and returns it as
        //a string, discards the newline character
        string _s = string_ReadLn();
        //returns varaint of _s with adjusted
        //start and count to strip ending whitespace
        //without allocation. Result is unsafe to delete
        string const s = string_TrimConst(_s);
        //Does a runtime checked conversion to I32
        //returns 0 if conversion fails
        guess = ToI32(s);
        //if result is zero and string is not a representation of zero,
        //the line was invalid input and is handled as such
        if(guess==0 && !string_IsZero(s))
            println("That wasn't a valid response, try again");
        else if(guess>secret){
            println("Too high");
        }else if(guess<secret){
            println("Too low");
        }else{
            b = false;
        }
        //deallocates original string
        string_Delete(&_s);
    }
    //currently, print(i32) family has additional overhead.
    //faster to use a stdio function
    printf("Congrats, the number was %"i32_PRI".\n",secret);
    return 0;
}