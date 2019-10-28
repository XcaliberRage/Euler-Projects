// Find the largest prime factor of 600851475143

// N % p
//    Modulo = 0 -> Save p -> Result % p
//    Modulo != 0 -> Try next Prime

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// The target number is a constant
#define N 600851475143

void getPrimes(bool *primes);
long largestFactorial(bool *primes, long number, long maxN);

unsigned int rootN;


int main(void)
{

    // We root N because we can infer the primes above this number using the primes below
    rootN = floor(sqrt((double)N)) + 1;

    // Here is our list of arrays, the index is it's own value. True if it's prime.
    bool primes[rootN+1];

    // let's take our list and set every odd value to true
    for (int i = 0; i < rootN; i ++)
    {
        // If it's 0, 1 or even except 2
        if (i < 2 || ((i > 2) && (i % 2 == 0)))
        {
            primes[i] = false;
        }
        else
        {
            primes[i] = true;
        }
    }

    // Now let's cut the list down until I only have primes (not including 2)
    getPrimes(primes);

    // Yay, now I have all the primes, I can start testing Factorisation
    long answer = largestFactorial(primes, N, rootN);

    printf("The largest factorial of %ld is: %ld\n", N, answer);
}

// Take each prime and identify if it factorialises into N
// This function is always going to spit out the largest factorial
// First we divide by the lowest prime in the list (this is why I populated my Llist in reverse order)
long largestFactorial(bool *primes, long number, long maxN)
{
    // We run the list to the end (or we get our answer)
    for (int p = 2; p < maxN; p++)
    {
        // We do nothing if the current element isn't prime
        if (primes[p])
        {
            // Is N/Pn a whole number? (Pn is whichever prime we're looking at in this list, first up is 2)
            if (number % p == 0)
            {
                // If yes we know that N/Pn is a factor
                // Also, the first time this is true means that N/Pn either is, or contains the largest factor we want
                // As such, all we need to do is get the largest factorial from THIS new number
                return largestFactorial(primes, number/p, number/p);
            }
        }
    }
    // If we get here, that means this number itself is prime because it failed to divide by any other prime number
    return number;
}

// I've already eliminated every even prime (except 2)
// Let's now run through and false all the other elements divisible by a prime number
void getPrimes(bool *primes)
{
    // We start at 3 because 2 was done in the initialisation
    // We then increment along the array
    for (int p = 3; p < rootN; p++)
    {
        // We check if it is a prime number (i.e. true)
        if (primes[p])
        {
            // if it is, we set every p'th field to false
            for (int i = p*2; i < rootN; i += p)
                primes[i] = false;
        }
    }
}
