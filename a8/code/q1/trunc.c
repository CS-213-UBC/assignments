#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "list.h"

void stringToNumber(element_t* rv, element_t av){
  int **r = (int**) rv;
  char *a = av;
  char *endptr;
  if (*r == NULL){
    *r = malloc(sizeof(int));
  }
  **r = strtol (a, &endptr, 0);
  if (*endptr){
    **r = -1; 
  }
}

int isPositive (element_t av) {
  int *a = av;
  return (*a > 0);
}

int noNull (element_t av) {
  char *a = av;
  if (a){
      return 1;
  } else{
      return 0;
  }
  return 0;
}

void addNull(element_t* rv, element_t av, element_t bv){
    char** r = (char**) rv;
    char* a = av;
    int* b = bv;
    
    if(*b < 0)
        *r = a;
    else
        *r = NULL;
}

void truncateList(element_t* rv, element_t av, element_t bv){
    char** r = (char**) rv;
    char* a = av;
    int* b = bv;

    a[*b] = 0; //truncate
    *r = a;
}

void concatString(element_t* rv, element_t av, element_t bv) {
    char **r = (char**) rv; 
    char *b = bv;
    char *a = av;
    if (strlen(*r) > 0){
        strcat(*r, " ");
        strcat(*r, b);
    }
        
}

void getMax (element_t* rv, element_t av, element_t bv) {
  int *a = av, *b = bv, **r = (int**) rv;
  if (*r == NULL)
    *r = malloc(sizeof(int));
  if (*b > **r){
      **r = *b;
  }
}


void print (element_t ev) {
  int* e = ev;
  printf ("%d\n", *e);
}

void printchar (element_t ev) {
    char* e = ev;
    printf ("%s\n", e? e : "NULL");
}

int** starify(int* inArray, int n) {
  int** outArray = malloc (n * sizeof(int*));
  for (int i=0; i<n; i++)
    outArray[i] = &inArray[i];
  return outArray;
}

int main(int argc, char *argv[]) {
    struct list* arglist = list_create();

    for (int i = 1; i < argc; i++){
        list_append(arglist, (element_t) argv[i]);
    }
    //printf ("argslist:\n");
    //list_foreach (printchar, arglist);

    struct list* listOfNumbers = list_create();
    list_map1 (stringToNumber, listOfNumbers, arglist);

    //printf ("listofnumbers:\n");
    //list_foreach (print, listOfNumbers);

    struct list* listWithNull = list_create();
    list_map2 (addNull, listWithNull, arglist, listOfNumbers);

    ////printf ("listwithnull:\n");
    //list_foreach (printchar, listWithNull);

    //Filter the number list to produce a new list with all negative values removed. The list
    //may thus be shorter than the original list.
    struct list* listNoNegative = list_create();
    list_filter (isPositive, listNoNegative, listOfNumbers);
    //printf ("positive:\n");
    //list_foreach (print, listNoNegative);

    //Filter the string list to produce a new list with all NULLs removed.
    struct list* listNoNull = list_create();
    list_filter (noNull, listNoNull, listWithNull);
    //printf ("no null:\n");
    //list_foreach (printchar, listNoNull);

    struct list* listTruncated = list_create();
    list_map2 (truncateList, listTruncated, listNoNull, listNoNegative);

    ///printf ("list truncated:\n");
    //list_foreach (printchar, listTruncated);

    // creating just one string
    char* s = malloc(sizeof(char));
    *s = 0;
    list_foldl (concatString, (element_t*) &s, listTruncated);
    printf ("string is %s\n", s);
    free (s);

    int s2 = 0, *sp = &s2;
    list_foldl (getMax, (element_t*) &sp, listOfNumbers);
    printf ("max val: %d\n", s2);

    list_destroy (arglist);
    list_destroy (listOfNumbers);
    list_destroy (listWithNull);
    list_destroy (listNoNegative);
    list_destroy (listNoNull);
    list_destroy (listWithNull);


}
