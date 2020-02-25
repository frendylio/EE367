/*
 * Your task is to modify function rob_rec() to solve the Robber Problem
 * by implementing dynamic programming with memoization.  Your
 * algorithm should run in O(n) time where n is the length of the array.
 *
 * The main function calls function rob() four times with different
 * inputs, in particular the array money[ ] will be different each
 * time.  Recall, that money[k] is the amount that can be stolen
 * at house k.
 *
 * The function should return the maximum amount that can be stolen
 * This will be displayed as a Score.  The function should also 
 * indicate in the house[] array, which houses should be robbed, 
 * e.g., house[k] = 1 means house k should be robbed, and house[k] = 0
 * means it should not be robbed.
 *
 * Note that 
 *    - rob() calls function rob_rec().
 *    - array house[] is initialized to 0.  
 *    - main also calls check_houses() which checks if house[] is valid
 * 
 * You can modify rob(), rob_rec(), and introduce new functions that rob()
 * can call. Do not modify main().
 */



// Frendy Lio
// Final
// Feb 13, 2020

#include <stdlib.h>
#include <stdio.h>

#define HOUSE_SIZE 20

int rob(int* money, int* house, int numHouses);

/*  Utility functions */
void clear_houses(int * house, int length); /* Clears array house[] */
void check_houses(int * house, int * money, int length);  /* Checks if houses are valid */
void random_fill(int * money, int length); /* Randomly fill array */
void display_array(int* a, int length); 

void main()
{
int money1[4] = {1, 2, 3, 1};
int money2[5] = {2, 7, 9, 3, 1};
int money3[5] = {10, 7, 3, 9, 1};
int money4[HOUSE_SIZE];
random_fill(money4, HOUSE_SIZE); /* Fill money3[ ] */

int house[HOUSE_SIZE];

clear_houses(house,4);
printf("\nScore = %d\n", rob(money1,house,4));
check_houses(house, money1, 4);

clear_houses(house,5);
printf("\nScore = %d\n", rob(money2,house,5));
check_houses(house, money2, 5);

clear_houses(house,5);
printf("\nScore = %d\n", rob(money3,house,5));
check_houses(house, money3, 5);

clear_houses(house,HOUSE_SIZE);
printf("\nScore = %d\n", rob(money4,house,HOUSE_SIZE));
check_houses(house, money4, HOUSE_SIZE);
}


/*========================================================================

rob_rec does the dynamic programming. and basically implement the recursion

memo is basically our money.

========================================================================*/

///////////////////////////////
// Return max of 2 numbers
///////////////////////////////
int rob_n(int x,int y){
    if (x>y){
        return 1;
    }

    else{
        return 0;
    }
}

int rob_rec(int* money, int* house, int* memo, int numHouses, int size)
{

    
    int score;
    int house_n;
    int house_nPlus1;

    if (numHouses < 0){
        return 0;
    }
    // Check if we have the value in our memory
    if (memo[numHouses] >= 0){
        return memo[numHouses];
    }

    else{
        // Rob first house
        house_n = money[numHouses] + rob_rec(money, house, memo, numHouses-2, size);
        // Rob second house instead
        house_nPlus1 = rob_rec(money, house, memo, numHouses-1, size);

        if (rob_n(house_n, house_nPlus1)){
            score = house_n;
        }
        else{
            score = house_nPlus1;
        }

        // Case 1: House a b c d
        //          Currently in house b
        //          When House a = b means we rob a
        if (memo[numHouses - 1] == score) {
            house[numHouses - 1] = 1;
        }

        // Case 2: House a b c d
        //          Currently in house c
        //          If a has not rob and b too, it means that c or b should be rob
        if (house[numHouses - 2] == 0 && house[numHouses -1] == 0){
            // Should we rob house b?
            if(money[numHouses - 1] > money[numHouses]){
                house[numHouses - 1] = 1;
            }
        }

        // Case 3: House a b c d e
        //          Currently in e
        //          if a b c are not rob but d was rob, rob b
        if (house[numHouses - 3] == 0 && house[numHouses - 2] == 0 && house[numHouses - 1] == 0 ){
            house[numHouses - 2] = 1;
        }

        // Case 4: House ... a b c
        //          if I am in c (the last house)
        //          check if b was rob if not then c MUST be rob
         if( numHouses == size && house[numHouses - 1] == 0){
             house[numHouses] = 1;
         }

        memo[numHouses] = score;

        return score;
    }
}


/*========================================================================

rob creates and initializes the memo array.

# Find the maximun value between:
#
# Case 1: n and n+2
# Case 2: n+1

# If Case 1 => Case 2 then rob n, else rob n+1 and then skip next one.

========================================================================*/

int rob(int* money, int* house, int numHouses){

    ///////////////////////////
    // Case 1: No houses return 0
    ///////////////////////////
    if (numHouses == 0){
        return 0;
    }

    ///////////////////////////
    // Case 2: 1 House 
    ///////////////////////////    
    else if(numHouses == 1){
        house[0] = 1;
        return money[0];
    }

    ///////////////////////////
    // Case 3: More than 1 House 
    /////////////////////////// 
    else{
        //variables and prepopulate witg -1
        int memo[10000] = { [0 ... 9999] = -1 };
        return rob_rec(money, house, memo, numHouses - 1, numHouses -1);
    }      
}

////////////////////////
// No recursion code
///////////////////////
// int rob(int* money, int* house, int numHouses)
// {
//  /* Create and initialize memo */
// int memo[HOUSE_SIZE+1]; 

// // Varialbes to storage money
// int money_n = 0;
// int money_nPlus1 = 0;
// int money_nPlus2 = 0;
// int money_nPlus3 = 0;
// int money_nPlus4 = 0;

// int house_1 = 0;
// int house_2 = 0;
// int house_3 = 0;

// int score = 0;
// int skip = 0;
// // loop variables
// int i = 0;
// int k = 0;
// int break_loop = 0;

// /////////////////////////////////////////
// // First populate memo for n, n+2, n+3
// /////////////////////////////////////////
// while (i < numHouses){
//     memo[i] = money[i];
//     i = i + 1;
//     break_loop = break_loop + 1;

//     // if break_loop = 4, all my varialbes are populated
//     if (break_loop == 4){
//         break;
//     }
// }

// // Add the end i = 4

// // Now we check if we rob or not
// while (i < numHouses + 4){
//     // Check if we rob house n or not
//     money_n = memo[i-4];
//     money_nPlus1 = memo[i - 3];
//     money_nPlus2 = memo[i - 2];
//     money_nPlus3 = memo[i - 1];
//     money_nPlus4 = memo[i - 0];

//     house_1 = money_n + money_nPlus2;
//     house_2 = money_nPlus1;
//     house_3 = money_n + money_nPlus3;

//     // Rob House n
//     // a b c d e
//     // Check if a + c > b
//     if (rob_n(house_1, house_2) && skip == 0){
//         // Check if a + c > a + d
//         if (rob_n(house_3, house_1)){
//             // check if e > d
//             if (money_nPlus4 > money_nPlus3){
//                 skip = 2;
//             }
//             else{
//                 skip = 1;
//             }
//         }
//         else{
//             skip = 1;
//         }
//       //  house[i-4] = 1;
//         score = score + money_n;
//         house[i-4] = 1;
        
//     }
    
//     // Rob House n+1
//     else{
//         skip = skip - 1;
//     }

//     memo[i] = money[i];
//     i = i + 1;
// }  

// return score;
// }


/* Utility functions */

void display_array(int* a, int length) 
{
for (int k=0; k<length; k++) {
    printf("%3d",a[k]);
}
printf("\n");
}

void random_fill(int * money, int length)
{
int state = 11;
for (int k=0; k<length; k++) {
    state = (53*state + 71)%97;
    money[k] = state;
}
}

void check_houses(int * house, int * money, int length) 
{
for (int k=0; k+1 < length; k++) {
    if (house[k] == 1 && house[k+1] == 1) {
	printf("Houses %d and %d are being robbed\n", k, k+1);
    }
    else if (house[k] !=0 && house[k] != 1) {
	printf("House %d is invalid\n", house[k]);
    }
}
int check_score=0;

for (int k=0; k<length; k++) {
    if (house[k] == 1) check_score += money[k];
}

printf("Check to verify solution:\n");
printf("   Score = %d\n", check_score);
printf("   Money = ");
display_array(money,length);
printf("   House = ");
display_array(house,length);
return;
}

void clear_houses(int * house, int length)
{
for (int i=0; i<length; i++) {
    house[i] = 0;
}
}
