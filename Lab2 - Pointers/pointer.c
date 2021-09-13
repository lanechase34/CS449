/*
 * pointer.c - Source file with your solutions to the Lab.
 *             This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>. The included file, "common.c" contains a function declaration
 * that should prevent a compiler warning. In general, it's not good practice
 * to ignore compiler warnings, but in this case it's OK.
 */

#ifndef COMMON_H
#include "common.h"
#endif

/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

#if 0
You will provide your solution to this homework by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
     not allowed to use big constants such as 0xffffffff.
     However, you are allowed to combine constants to values greater
     than 255 or less than 0. e.g. 250 + 250 = 500, so long as
     the operator you are using to combine the constants is listed as
     "legal" at the top of the method you are writing.
  2. Function arguments and local variables (no global variables).
  3. Any operators listed as "allowed" at the top of the method you are writing.
  4. casting.

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc. (unless
     otherwise stated).
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions (unless otherwise stated).
  5. Use any operators not "allowed" at the top of the method you are writing.

  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has *undefined* behavior when shifting by a negative amount or an amount
     greater than or equal to the number of bits in the value being shifted.
     e.g. For x >> n, shifts by n < 0 or n >= *size of x* are undefined
     e.g. if x is a 32-bit int, shifts by >= 32 bits are undefined
     Undefined means you do not know what result you will get from the operation.
#endif

/*
 * STEP 2: Modify the following functions according the coding rules.
 */

/*
 * Return the size of an integer in bytes.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, eFDIStc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, <<, >>, ==, !=, ^, /, %
 *   Unary integer operators: ~, -
 */
int intSize() {
  int intArray[10];//int array allocates 10 * sizeofInt() space in memory
  int * intPtr1 = intArray; //set the first pointer to the start of the array
  int * intPtr2 = intArray + 1; //set second pointer to +1 in the array memory which is moving the sizeofInt
  // Write code to compute size of an integer.
  return abs(abs(intPtr2) - abs(intPtr1)); //take ptr2 - ptr1 to calculate the difference in 1 spot of memory in the array which is size of int
  //my memory address was allocated as a negative number? so I added the abs part
}

/*
 * Return the size of a double in bytes.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, <<, >>, ==, !=, ^, /, %
 *   Unary integer operators: ~, -
 */
int doubleSize() {
  double doubArray[10];
  double * doubPtr1 = doubArray; //first pointer set to the start of the array
  double * doubPtr2 = doubArray + 1; //second pointer set to 1 step above the start which is the size of double
  // Write code to compute size of a double.

  return abs(abs(doubPtr2) - abs(doubPtr1)); //take the difference which equals size of double
  //absolute part b/c memory was assigned negative on my computer and would give -8 
}

/*
 * Return the size of a pointer in bytes.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, <<, >>, ==, !=, ^, /, %
 *   Unary integer operators: ~, -
 */
int pointerSize() {
  double * ptrArray[10];
  double ** ptrPtr1 = ptrArray; //first pointer set to the start of the array
  double ** ptrPtr2 = ptrArray + 1;//second pointer set to 1 step above the start which is the size of ptr
  // Write code to compute size of a pointer.

  return abs(abs(ptrPtr2) - abs(ptrPtr1));//take the difference which equals size of ptr
  //absolute part b/c memory was assigned negative on my computer and would give -8 
}

/*
 * Given pointers to two distinct variables write a function to swap the values
 * of said variables.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, <<, >>, ==, !=, ^, /, %
 *   Unary integer operators: ~, -
 */
void swapInts(int *ptr1, int *ptr2) {
  int temp = *ptr1; //temp holds the value that ptr1 is pointing to
  *ptr1 = *ptr2; //ptr1 now holds the value ptr2 was pointing to
  *ptr2 = temp; //ptr2 now holds the value of temp which was the value ptr1 was pointing to
}

/*
 * Modify intArray[5] to be the value 449 using only intArray and pointer arithmetic.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *, <<, >>, ==, ^
 *   Unary integer operators: !, ~
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, ~= etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, !=, /, %
 *   Unary integer operators: -
 */
int changeValue() {
  int intArray[10];
  int * intPtr1 = intArray + 5; //add 5 to get to the spot in memory where intArray[5] is stored
  // Remember not to use constants greater than 255.
  // Remember to use * to dereference. You cannot use '[<index>]' syntax.
  int value = 0xFF + 0xC2; // 0xFF = 255 + 0xC2 = 194 = 449
  *intPtr1 = value; //change the value that intPtr1 (intArray[5]) holds to the value of 'value' which is 449
  return intArray[5];
}

/*
 * Return 1 if the addresses stored in ptr1 and ptr2 are within the
 * *same* 64-byte aligned  block of memory. Check the spec for examples if you are
 * confused about what this means. Return zero otherwise.
 * Operators / and % and loops are NOT allowed.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *, <<, >>, ==, ^
 *   Unary integer operators: !, ~
 *   Shorthand operators based on the above: ex. <<=, *=, ++, --, etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, !=, /, %
 *   Unary integer operators: -
 */
int withinSameBlock(int *ptr1, int *ptr2) {
  // Your code here 0x40 is 64 in hex, 0x3F is how long each block of memory is
  // first shift our values to the right 6 bits because those are the specifics that would change in a specific block of memory./
  // 0x3CE = 001111(001110) 0x404 = 010000(000100) - since the bits dont match minus the least 6 significant bits, these are not in the same block of memory
  // 0x3CE = 001111(001110) 0x3EF = 001111(101111) - since the bits match minus the least 6 significant bits, these are in the same block of memory


  return ((int)ptr1 >> 6) == ((int)ptr2 >> 6); //compares the 26 most significant bits(32 bit) and if they are equal the pointers are in the same chunk of memory
}

/*
 * Return 1 if ptr points to an element within the specified intArray, 0 otherwise.
 * Pointing anywhere in the array is fair game, ptr does not have to
 * point to the beginning of an element. Check the spec for examples if you are 
 * confused about what this method is determining.
 * size is the size of intArray in number of ints. Can assume size != 0.
 * Operators / and % and loops are NOT allowed.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *, <<, >>, ==, ^
 *   Unary integer operators: !, ~
 *   Shorthand operators based on the above: ex. <<=, *=, ++, --, etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, !=, /, %
 *   Unary integer operators: -
 */
int withinArray(int * intArray, int size, int * ptr) {
  
  int start = (int) intArray;
  int ptradd = (int) ptr;

  int length = ptradd - start; //calculate the distance between the ptrAddress and the start of the arrayAddress
  length >>= size; //length = length >> size, testing to see if ptr is farther away than the size of the array, (ptrAdd - arrAdd <= Arr.length)
  //if it is bigger, the value when shifted to the right by size would be greater than 0 

  return length == 0;
}

/*
 * In C characters are are terminated by the null character ('\0')
 * given a pointer to the start of the string return the length of this string.
 * (The null character is not counted as part of the string length.)
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *, ==, !=, <, >
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. <<=, *=, ++, --, etc.
 *   Control constructs: for, while
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <<, >>, ^, /, %
 *   Unary integer operators: ~, -
 */
int stringLength(char * s) {
  //calculate the difference between the start address and the address which stores \0
  char *start = s; //set start to s
  char *end = s; //end also to s
  while(*end != '\0'){ //keep incrementing end by 1 char until it reaches the end character
    end++;
  }
  //subtract the address of the end minus the address of the start to calculate length
  //char is only 1 byte so this works
  return end - start;
}

/*
 * Change the value pointed to by ptr byte-by-byte so that when returned as an integer
 * the value is 449449.
 *
 * Hint: Remember that an int is 4 bytes.
 *
 * Hint: Remember how little endian works for data storage, how is it different between an multiple bytes(int) and a single byte?
 *
 * Hint: It will be easiest to start convert 449449 into binary form and starting seeing how the endian works from there.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, etc.
 *   Unary integer operators: !
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, <<, >>, ==, !=, ^, /, %
 *   Unary integer operators: ~, -
 */
int endianExperiment(int* ptr) {
  // 449449 in binary 00000000000001101101101110101001
  // first 8 bits = 10101001 = 0xA9
  //2nd = 11011011 = 0xDB
  //3rd = 00000110 = 0x06
  //4th = 00000000 = 0xFF 

  char *bytePtr = (char *) ptr; //char is 1 byte, int has 4 bytes
  *bytePtr = 0xA9; //first byte 
  *(bytePtr + 1) = 0xDB; //second byte
  *(bytePtr + 2) = 0x06; //third byte
  *(bytePtr + 3) = 0x00; //fourth byte
  return *ptr;
}

/*
 * Selection sort is a sorting algorithim that works by partitioning the array into
 * a sorted section and unsorted section. Then it repeatedly selects the minimum element
 * from the unsorted section and moves it to the end of the sorted section.
 *
 * So the pseudo-code might look something like this:
 * arr - an array
 * n - the length of arr
 *
 * for i = 0 to n - 1
 *   minIndex = i
 *   for  j = i + 1 to n
 *       if arr[minIndex] > arr[j]
 *           minIndex = j
 *       end if
 *   end for
 *   Swap(arr[i], arr[minIndex])
 * end for
 *
 * Implement selection sort below, it might be helpful to use the swapInts function you
 * defined earlier.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *, ==, !=, <, >
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, etc.
 *   Control constructs: for, while, if
 *   Function calls: swapInt()
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <<, >>, ^, /
 *   Unary integer operators: ~, -
 */
void selectionSort(int arr[], int arrLength) {
  int i, j, min_index;
  // Your code here
  //int arr[], int arrLength
  int *arrPoint = arr;
  for(i = 0; i < arrLength - 1; i++){ //traverse entire array
    min_index = i;
    for(j = i + 1; j < arrLength; j++){ //traverse remaining elements to find the smallest value then swap that value to the front 
      if(*(arrPoint + min_index) > *(arrPoint + j)){ //arr[min_index] > arr[j]
        min_index = j; 
      }
    }
    swapInts((arrPoint + i), (arrPoint + min_index)); //swapInts(int *ptr1, int *ptr2) pass addresses of what to swap, &arr[i], &arr[min_index]
  }
}


