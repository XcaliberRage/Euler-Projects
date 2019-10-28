// Take numbers 1 to 100, sum the square of each number
// Square the sum of each number
// Output the difference between the two

#include <stdio.h>

#define RANGE 100

int main(void)
{
    int sumOfSquares = 0, squareOfSum = 0,
        numbersSummed = 0, difference;

    // Get each number, add it to the sum and then square the number;
    for (int i = 0; i < RANGE; i++)
    {
        int number = i + 1;
        // Add it to the sum
        numbersSummed += number;
        // Add the square to the sum
        sumOfSquares += (number * number);
    }

    squareOfSum = numbersSummed * numbersSummed;

    difference = squareOfSum - sumOfSquares;

    printf("%i - %i = %i.\n", squareOfSum, sumOfSquares, difference);
}