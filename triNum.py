import math
import sys
import cs50

# Code to find the lowest Triangle number with 500 factors

# Generate a triangle number
# Find each prime underneath sqrt of this number

# Use our list of primes to identify the factors of the Tri
# When the array is length 500 return the number

#These need to be global
TARGET_LENGTH = 500
n = 0
tri = 0
# start with the first prime
primes = [2]

def main():
    # We loop indefinitely because our exit clause is checked when we add values to the divisors set
    # If we don't exit, the code is not functional, so this is totally fine
    tri = 1
    n = 2
    approved = ""

    while True:
        # Generate triangle number
        # Literally all you have to do is add n to previous triangle number
        # The first time when n is 2, the tri is 1 (staring with n as 1 made things a bit more confusing)
        tri += n
        print("------------------------------------")
        print(f"Attempt T-Num: {n} = {tri}")

        # When finding factors,you'll get any factor above sqrt(number) by using any number below
        maxPrime = math.trunc(math.sqrt(tri))

        #math.trunc((tri/2))
        print("Largest prime factor = ", maxPrime)

        # We have to reset the divisor set every time we generate a new Tri
        divisors = {1}

        lastPrimeIndex = len(primes) - 1

        # add further primes to the primes array (only need to check up to sqr root)
        # Use the traditional method:
        # Populate an array with every integer from the largest prime up to tri
        setNum = primes[lastPrimeIndex] + 1

        addPrimes(setNum, maxPrime)
        # We have all the primes

        #print("Looking for Divisors:")

        # Using the primes we can recursively divide and construct our divisors set
        findDiv(tri, maxPrime, divisors)

        print(f"Triangle number {tri} has {len(divisors)} or more factors:")
        print(sorted(divisors))
        print("------------------------------------")

        if approved == "":
            approved = cs50.get_string("Proceed?")

        n += 1

    # As a note: This code will end while running the addDiv function
    # If it doesn't end, something must have gone wrong so we need to debug anyway


# Add new divisors as they come
def findDiv(subject, maxPrime, divisors):

    # Check each prime we have up to our limits (as defined in the addPrimes function)
    for divisor in primes:

        # We only want to operate on values below sqrt(tri) and ones that divide the subject
        if divisor <= maxPrime and subject % divisor == 0:

            product = math.trunc(subject/divisor)
            #print(f"    {subject} / {divisor} = {product}")

            # addDiv checks both that we aren't duplicating and if our master case has been achieved
            addDiv(divisor, divisors)

            # If we find that our product is further divisible, we need to dig deeper
            if product not in primes:
                # So we call this function again using the product as our subject
                findDiv(product, maxPrime, divisors)

        # We stop iterating if we're above maxPrime
        elif divisor > maxPrime:
            break

    # Whenever we run this function, the subject will always be a divisor of the Tri so we'll always add it
    addDiv(subject, divisors)


# Use Sieve of Eratosthenes to find primes between two given values
def addPrimes(lowestPrime, upperLimit):

    upperLimit = lowestPrime if lowestPrime > upperLimit else upperLimit

    #print(f"    Adding primes between values {lowestPrime} and {upperLimit}")
    #print("    Current primes = ", primes)

    x = lowestPrime

    testVals = []

    # Just fill an empty array with True values, one for each value between our min and max
    # Index 0 correlates to the lowestPrime value (so index 0 would translate to index+lowestPrime)
    while x <= upperLimit:
        testVals.append(True)
        x += 1

    #print("    Test Vals = ", testVals)

    # Now we just eliminate values
    # First eliminate every prime'th index
    for index in range(len(testVals)):
        thisVal = index+lowestPrime
        #print(f"    Is {thisVal} prime?")
        for prime in primes:
            # We don't look higher than maxPrime
            if prime > upperLimit:
                break
            else:
                #print(f"    (I:{index}){thisVal} % {prime} = {thisVal%prime}")
                if (thisVal) % prime == 0:
                    testVals[index] = False
                    #print("   ", thisVal, "is not Prime")
                    break


    #print("    Test Vals after checks = ", testVals)

    # Now we add any true values to the global prime array
    for p in range(len(testVals)):
        if testVals[p]:
            # Remember, our array index value is offset by the min value
            primes.append(p+lowestPrime)
            #print(f"    p = {p}, adding p + {lowestPrime} = {p + lowestPrime}")

    #print("    Updated primes: ", primes)


# Add a new element to a set
def addDiv(number, numbers):

    # Don't add duplicates
    if number not in numbers:
        numbers.add(number)

    #print("    Div set length = ", len(numbers))

    # If we have found enough divisors we can just stop the code
    if len(numbers) > TARGET_LENGTH:
        # tri is a global variable for this reason
        print()
        print()
        print("Triangle number with >= 500 divisors found!  <----------")
        print(tri)
        print()
        print()
        sys.exit(0)


if __name__ == "__main__":
    main()