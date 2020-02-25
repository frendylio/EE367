/* Frendy Lio 
 *
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * The function's input is the char string 'str[]' and its length
 * It will find the longest substring that has just one character.
 * We will refer to such a substring as a "single-string" since 
 * it has only a single character.
 * For example, the longest substring in "aababbbbaa" is "bbbb".
 * It returns the index of the first and last characters in the 
 * substring.  In the example, first = 4 and last = 7.
 */ 
int strlength(char str[], int length, int *first, int *last);

/*
 * This function will return the length of the char string
 */
int total_length(char str[]);

char str1[] = "aababbbbaabba";
char str2[] = "acbcbbabcccbbca";

void main(int argc, char *argv[])
{
int first, last;

int length = strlength(str1, total_length(str1), &first, &last);
printf("Longest substring of '%s' has length=%d, starts at %d, ends at %d\n", str1, length,first, last);

length = strlength(str2, total_length(str2), &first, &last);
printf("Longest substring of '%s' has length=%d, starts at %d, ends at %d\n", str2, length,first, last);
}

int strlength(char str[], int length, int *first, int *last)
{
int ssFirst = 0;      /* Beginning of the current single-string */
int ssLast = 0;;
int bestFirst = 0;    /* Best single-string so far */
int bestLast = 0;
int k;

// Keep track of characters
char uno = str[0]; // initialize as first charcter
char dos = '\0'; // at the stat is empty so empty string

/////////////
// Loop all string
////////////
for (k=0; k<length; k++) {

    ///////////
    // Debug
    //////////
    #ifdef DEBUG
        printf("----str[k] %c ---- \n", str[k]);
        printf("----str[ssFirst] %c ---- \n", str[ssFirst]);
        printf("----ssLast %d ---- \n", ssLast);
        printf("----ssFirst %d ---- \n", ssFirst);
        printf("----bestLast %d ---- \n", bestLast);
        printf("----bestFirst %d ---- \n", bestFirst);
        printf("----k %d ---- \n\n\n", k);
    #endif

    ////////////
    // Code
    ///////////
    if (
            str[k] != uno 
        &&  str[k] != dos
        &&  dos != '\0'
        ){ /* End of current substring */

        ssLast = k-1;

        // Switch
        if (
                (ssLast - ssFirst) > (bestLast - bestFirst)
            ) {
            bestFirst = ssFirst;
            bestLast = ssLast;
        }

        // Tracker
        ssFirst = k-1;
        uno = str[k-1];
        dos = str[k];
    }

    // first time is empty so put it stuff
    else if(
            dos == '\0' 
        &&  str[k] != uno
    ) dos = str[k];
}
// Switch at the end
    ssLast = k-1;

    if ((ssLast - ssFirst) > (bestLast - bestFirst)) {
        bestFirst = ssFirst;
        bestLast = ssLast;
    }

    *first = bestFirst;
    *last = bestLast;
return bestLast-bestFirst + 1;
}

int total_length(char str[])
{
int k;
for (k=0; str[k]!='\0'; k++);
return k;
}



