// Find all primes below 1million
// Sum them

#include <stdio.h>
#include <stdbool.h>

#define T 2000000

bool primes[T];

int main(void)
{
    primes[0] = false;
    primes[1] = false;
    // Set every odd value from 3 to true (0 and 1 will never be prime)
    // Also set 2 to true, everything else should be false
    for (int i = 2; i < T; i++)
    {
        primes[i] = (i % 2 == 0 && i != 2) ? false:true;
    }

    // Now go through and start eliminating primes
    int cursor = 3;
    while (cursor < T)
    {
        //If this value is prime, eliminate all multiples
        if (primes[cursor])
        {
            for (int k = cursor * 2; k < T; k += cursor)
                primes[k] = false;
        }
        cursor++;
    }

    unsigned long sum = 0;
    for (int j = 0; j < T; j++)
    {
        if (primes[j])
            sum += j;
    }

    printf("Sum of all primes under %i = %lu\n", T, sum);
}