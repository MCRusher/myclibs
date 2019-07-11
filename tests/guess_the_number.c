#include "newstd.h"

int main(void){
    u8 ans = randb_u8(0,100);
    unsigned int guess;
    puts("Guess the number(0-100):");
    int tmp;
    while(1){
		if((tmp=scanf("%u",&guess))== 0 || tmp == EOF){
			puts("Retry, guess was not a number.");
		}else if(guess>100){
			puts("Retry, guess was over 100.");
		}else if(guess>ans){
			puts("Retry, guess was too high.");
		}else if(guess<ans){
			puts("Retry, guess was too low.");
		}else if(guess==ans){
			printf("Correct, answer was %u!\n",ans);
			break;
		}
		//discards unused input on the same line.
		//without this, multiple guesses can be made at once
		//and incorrect input results in an infinite loop.
		while((tmp=getchar())!='\n' && tmp != EOF);
    }
    return 0;
}
