/*
Assigment: 1 
Name: Hao-En Lu 
G number: G01442266 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// define common parameters
#define STRING_BUF 30
#define STRING_CNT 10
#define MAX_STRING_LEN 25
#define MIN_STRING_LEN 2

// define functions
void inputStrings(char str[STRING_CNT][STRING_BUF]);
bool checkInputStringError(char str[STRING_BUF]);
bool checkStringLen(char str[STRING_BUF]);
bool checkLegalCharacters(char str[STRING_BUF]);
bool checkDuplicate(char str[STRING_CNT][STRING_BUF],int count);


void inputStrings(char str[STRING_CNT][STRING_BUF]){
    /* Get the input string from the console*/

    int count  = 0;
    printf("Enter 10 charater strings:\n\n");

    while (count < STRING_CNT){
        printf("Enter string %d: ",count + 1);
        fgets(str[count],STRING_BUF,stdin); // user inputs string using fgets function to include whitespace
        
        if (checkInputStringError(str[count]) == true){ // return true if string has no error
            if (checkDuplicate(str,count) == true){ // return true if string is not duplicate
                count++;
            }
        }
    }
}


bool checkInputStringError(char str[STRING_BUF]){
    /* Check if the string have any error*/

    if (checkStringLen(str) == false){ // check string length
        return false; // not pass return false
    }
    if (checkLegalCharacters(str) == false){ // check illegal character
        return false; // not pass return false
    }

    return true; // return true if no error
}

bool checkStringLen(char str[STRING_BUF]){
    /* Check if the length of string is in the restricted range of number*/

    if ((strlen(str) - 1) == 0){ // check if string length is zero
        printf("Error: string has zero length - please re-enter\n"); // print error and return false
        return false;
    }

    if ((strlen(str)-1) > MAX_STRING_LEN || (strlen(str)-1) < MIN_STRING_LEN) // check if string length exceed 25 or below 2
    {
        printf("Error: string length needs to be between 2 to 25 - please re-enter\n"); // print error message and return false
        return false;
    }
    return true; // return true if no error
}

bool checkLegalCharacters(char str[STRING_BUF]){
    /* Check if the string have illegal characters*/

    char illegal_char[] = "!@#$%^()";
    for (int i=0;i<strlen(illegal_char);i++){
        char* res = strchr(str,illegal_char[i]);  // check if the string contain illegal character using strchr 
        if (res != NULL){   
            printf("Error: %c is an illegal character - please re-enter\n",illegal_char[i]);  // print error message and return false
            return false;
        }
    }
    return true; // return true if no error
}

bool checkDuplicate(char str[STRING_CNT][STRING_BUF],int count){
    /* Check if the string already exist in the array*/

    for (int i=0;i<count;i++){
        if ((strcmp(str[i],str[count])) == 0){
            printf("Error: duplicate string - please re-enter\n"); // print error and return false
            return false;
        }
    }
    return true; // return true if no error
}

void swap(char arr[STRING_CNT][STRING_BUF],int i, int j){ 
    /* Swap the two string in the array */

    char temp[STRING_BUF];
    strcpy(temp,arr[i]);
    strcpy(arr[i],arr[j]);
    strcpy(arr[j],temp);
}

void ascendingSort(char sortOrder,char strings[STRING_CNT][STRING_BUF]){
    /* Sort the array in corresponding order*/

    if (sortOrder == 'A'){  // sort the array with ascending order(low to high)
        printf("Ascending Order:\n\n");
        for (int i=0;i<STRING_CNT - 1;i++){
            for (int j=i+1;j<STRING_CNT;j++){
                if ((strcmp(strings[i],strings[j])) > 0){
                    swap(strings,i,j);
                }
            }
        }
    }
    else if (sortOrder == 'D'){
        printf("Descending Order:\n\n"); // sort the array with decending order(high to low)
        for (int i=0;i<STRING_CNT - 1;i++){
            for (int j=i+1;j<STRING_CNT;j++){
                if ((strcmp(strings[i],strings[j])) < 0){
                    swap(strings,i,j);
                }
            }
        }
    }

    for (int i=0;i<STRING_CNT;i++){ // print out the sorted array
        printf("%s",strings[i]);
    }
}

int main(){

    char strings[STRING_CNT][STRING_BUF]; // Initialize array for user inputs

    inputStrings(strings);

    char sortOrder;
    printf("Print character strings in Ascending(A) or Descending(D) order:");
    scanf(" %c",&sortOrder);// user input the sort order

    while (sortOrder != 'A' && sortOrder != 'D'){ // check if the character is A or D
        printf("Error: please enter either A(Ascending Order) or D(Descending Order)\n"); // print out error
        printf("Print character strings in Ascending(A) or Descending(D) order:");
        scanf(" %c",&sortOrder); // if not A or D- input character again
    }

    ascendingSort(sortOrder,strings);
   
    /* print out highest and lowest ASCII value string */
    if (sortOrder == 'A'){
        printf("\n");
        printf("String with lowest ASCII value:%s",strings[0]);
        printf("String with highest ASCII value:%s",strings[STRING_CNT-1]);
    }

    else if (sortOrder == 'D'){
        printf("\n");
        printf("String with lowest ASCII value:%s",strings[STRING_CNT-1]);
        printf("String with highest ASCII value:%s",strings[0]);
    }
    
    return 0;
}
