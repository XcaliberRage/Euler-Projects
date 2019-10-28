// Identify the largest palindrome that is made from the product of two 3 digit numbers


#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#define DIGITS 3
#define ODD true
#define EVEN false

typedef struct
{
    int number;
    int numberSize;
    int number_of_Pairs;
    int digitPairs[DIGITS+1];
    bool oddDigits;
} palindrome;

int lowestFactor, highestFactor, largestProductDigits,
    lowestProductDigits, highestProductDigits, highestProduct, lowestProduct;
palindrome *largestPalindrome;

int *factorA = NULL, *factorB = NULL;

int findDigitCount(int number),
    findPalindrome(int startNumber, palindrome *target),
    makePalindrome(palindrome *target),
    pallyPairs(palindrome *target),
    nextValue(int *array, int location, int originalSize);

void setOdd(palindrome *target), reduceNumberSet(palindrome *target),
    nextPalindrome(palindrome *target, int location);

bool isValid(palindrome *target, int *factorA_master, int *factorB_master),
    isPalindrome(int *array, int size);


int main(void)
{
    // First I need to find the value range of multiplying two 3 digit numbers
    lowestFactor = pow(10,(DIGITS - 1));
    highestFactor = pow(10, DIGITS) - 1;
    highestProduct = highestFactor*highestFactor;
    lowestProduct = lowestFactor*lowestFactor;

    // I also want to know how big the largest possible palindrome would be
    lowestProductDigits = findDigitCount(lowestProduct);
    highestProductDigits = findDigitCount(highestProduct);

    largestPalindrome = malloc(sizeof(palindrome));
    if (largestPalindrome == NULL)
    {
        printf("Could not allocate memory.\n");
        free(largestPalindrome);
        return 1;
    }

    largestPalindrome->numberSize = highestProductDigits;
    largestPalindrome->number_of_Pairs = pallyPairs(largestPalindrome);

    // intialise the palindrome, put 9s in all the used slots
    for (int i = 0; i < DIGITS+1; i++)
        largestPalindrome->digitPairs[i] = (i < largestPalindrome->number_of_Pairs) ? 9 : INT_MAX;

    setOdd(largestPalindrome);
    largestPalindrome->number = 0;

    largestPalindrome->number = findPalindrome(highestProduct, largestPalindrome);

    // Now we have our palindrome, we check if we can get an appropriate factor or do we need to find the next smaller palindrome
    factorA = malloc(sizeof(int));
    factorB = malloc(sizeof(int));

    *factorA = highestFactor;
    *factorB = lowestFactor;

    while (!isValid(largestPalindrome, factorA, factorB))
    {
        nextPalindrome(largestPalindrome, (largestPalindrome->number_of_Pairs - 1));
        largestPalindrome->number = makePalindrome(largestPalindrome);
    }

    printf("%i x %i = %i\n", *factorA,*factorB, largestPalindrome->number);

    free(largestPalindrome);
    free(factorA);
    free(factorB);
}

// Find how many digits are in a given number
int findDigitCount(int number)
{
    int count = 1;

    while (number > 9)
    {
        number = floor((number/10));
        count++;
    }

    return count;
}

// Take a number and find the first palindrome from there
int findPalindrome(int startNumber, palindrome *target)
{
    // Count the start number down until the first and last digit match
    // Start by making an array populated by the digits
    int arraySize = findDigitCount(startNumber);
    int numbersArray[arraySize];
    for (int i = 0; i < arraySize; i++)
    {
        numbersArray[i] = startNumber;
        // Grab each digit sequentially
        while (numbersArray[i] > ((pow(10,(i + 1))) - 1))
            numbersArray[i] = floor((numbersArray[i] / 10));

        numbersArray[i] %= 10;
    }

    // Check if this number is palindromic, if not, count down by one until it is
    while (!isPalindrome(numbersArray, arraySize))
        arraySize = nextValue(numbersArray, arraySize - 1, arraySize);

    // Copy in the pairs to the struct
    for (int j = 0; j < target->number_of_Pairs; j++)
    {
        // Null any extra array slots
        if (j >= ceil((double)arraySize/2))
        {
            target->digitPairs[j] = INT_MAX;
        }
        else
        {
            target->digitPairs[j] = numbersArray[j];
        }
    }

    while (target->numberSize > arraySize)
        reduceNumberSet(target);

    setOdd(target);
    return makePalindrome(target);
}

// Return true if the given array is palindromic
bool isPalindrome(int *array, int size)
{
    // Look at the first half of the array and check it against the second half
    for (int i = 0; i < ceil((double)size / 2); i++)
    {
        // If the pair don't match, stop checking and report it doesn't match
        if (array[i] != array[size - 1 - i])
            return false;
    }

    return true;
}

// Tick the given value (in array form) down by one
int nextValue(int *array, int location, int originalSize)
{
    // Check if ticking down will change any other values
    if (array[location] > 0 || ((location == 0) && (array[location] > 1)))
    {
        array[location]--;
    }
    else
    {
        // Check if this is the first digit
        if ((location) == 0)
        {
            // If it is we need to reduce the size, this will prevent us looking past the numbers we care about
            originalSize--;
        }

        // Now we set the digit to 9
        array[location] = 9;

        // And tell the next digit to modify
        originalSize = nextValue(array, location - 1, originalSize);
    }

    return originalSize;
}

// Run this function every time the numberSize changes
void setOdd(palindrome *target)
{
    target->oddDigits = (target->numberSize % 2 == 0) ? EVEN : ODD;
}

// Using our refigured contents, remake the new palindrome
int makePalindrome(palindrome *target)
{
    int output = 0;

    // Cap and fill the palindrome
    for (int i = 0; target->digitPairs[i] != INT_MAX; i++)
    {
        output += target->digitPairs[i]*(pow(10,(target->numberSize - (1 + i))));

        // Check if this is the center digit or not
        if (!((target->digitPairs[(i + 1)] == INT_MAX) && target->oddDigits))
        {
            // If it isn't, we need to add the second (smaller) value
            output += target->digitPairs[i]*(pow(10,(i)));
        }
    }

    return output;
}

// Find the factors of this palindrome and return true if they are both 3 digits.
bool isValid(palindrome *target, int *factorA_master, int *factorB_master)
{
    //Start with the largest possible factor
    int factor1 = highestFactor;

    // If it doesn't divide evenly, try the next number down until we drop below 100
    while (target->number % factor1 != 0 && (findDigitCount(factor1) == DIGITS))
        factor1--;


    int factor2 = target->number / factor1;

    // Now we check that both our factors are 3 digit numbers
    if ((findDigitCount(factor1) == DIGITS) && (findDigitCount(factor2) == DIGITS))
    {
        *factorA_master = factor1;
        *factorB_master = factor2;
        return true;
    }

    // If we get here this palindrome isn't valid
    return false;
}

// Try the next lowest palindrome
void nextPalindrome(palindrome *target, int location)
{
    // Identify if the first number can be reduced by 1 (or isn't a cap number about to flick to 0)
    if ((target->digitPairs[location] > 0) || (location == 0 && target->digitPairs[location] > 1))
    {
        target->digitPairs[location]--;
    }
    else
    {
        // If not, check it wasn't a cap number
        if (location == 0)
        {
            // If it was, reduce the entire number set by 1 instead
            reduceNumberSet(target);
            return;
        }

        // Otherwise rotate it back to 9 and reduce the next number by 1
        target->digitPairs[location] = 9;
        nextPalindrome(target, (location - 1));
    }
}

// Reset the number of pairs in the palindrome
int pallyPairs(palindrome *target)
{
    return (int)ceil(target->numberSize/2);
}


void reduceNumberSet(palindrome *target)
{
    target->numberSize--;
    target->number_of_Pairs = pallyPairs(largestPalindrome);
    setOdd(target);
    target->digitPairs[target->number_of_Pairs] = INT_MAX;

    for (int i = 0; i < target->number_of_Pairs; i++)
        target->digitPairs[i] = 9;
}