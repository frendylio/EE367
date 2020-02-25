#include <stdlib.h>
#include <stdio.h>

#define MAXLENGTH 100  /* Max length of a string */

int one_away(char str1[], char str2[]);
int str_length(char str[], int max_length);

char str[] = "pale";
char strA[] = "ple";
char strB[] = "pales";
char strC[] = "bale";
char strD[] = "bake";


void main() 
{
printf("'%s' and '%s' have result %d\n", str, strA, one_away(str, strA));
printf("'%s' and '%s' have result %d\n", str, strB, one_away(str, strB));
printf("'%s' and '%s' have result %d\n", str, strC, one_away(str, strC));
printf("'%s' and '%s' have result %d\n", str, strD, one_away(str, strD));
}


int one_away(char str1[], char str2[])
{
    // Loop variables
    int i = 0;
    int j = 0;
    int onedifferent = 0;

    while(str1[i] || str2[j]){
        // if same character, move togehter
        if (str1[i] == str2[j]){
            i++;
            j++;
        }

        // if different
        else{

            // increase onedifferent
            onedifferent++;

            // if onedifferent == 2 return 0
            if (onedifferent == 2){
                return 0;
            }  

            // Check next one of str1 if is same as str2
            if(str1[i + 1] == str2[j]){
                i++;
            }

            // Check next one of str2 if is same as str1
            else if(str1[i] == str2[j + 1]){
                j++;
            }

            //Cheeck if both next one are same
            else if (str1[i + 1] == str2[j + 1]){
                i++;
                j++;
            }
                           
        }
    }
return onedifferent;
}



int str_length(char str[], int max_length)
{
int length;
for (length = 0; (str[length] != '\0') && (length < max_length); length++); 
return length;
}

