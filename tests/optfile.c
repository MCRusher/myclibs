//Hello, World!
#include "../code/myclibs/newstd.h"

typedef option(FILE*) option_file;

option_file openFile(char const * const s){
    FILE* tmp = fopen(s,"r");
    option_file of;
    if(!tmp)
        option_SetNone(of);
    else
        option_Set(of,tmp);
    return of;
}

int main(void){
    option_file of = openFile("optfile.c");
    if(option_IsNone(of)){
        println("Could not open file.",stderr);
        return -1;
    }
    char line[200];
    //option_Unbox will never deref null, as
    //calling unbox on a none option causes
    //a runtime error and exits.
    fgets(line,200,option_Unbox(of));
    char* nlptr = strchr(line,'\n');
    if(nlptr) *nlptr = '\0';
    println_in("line is \"{}\"",line);
}