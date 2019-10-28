// Find the 10,001st prime

// Find each prime from 2
// To do so, take a number, if it has modulo 0 against any previously found prime number, it is prime
// Stop at the 10,001st prime found.

// What's a good data structure? Probably a llist.
// It might be big but finding the last entered element is always fast and that's all we really care about
// I think I might have the llist have two entrances though so I can work my way from the smallest primes (more likely to out composits)

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define TARGET_POSITION 10001

// Let's make our llist nodes, two way!
typedef struct node
{
    int value, position;
    struct node *next;
    struct node *prev;
}
node;

// Here I globally define a root node and cap node.
// Root gives a way in from last-in
// Cap gives us first-in entry.
struct node *root;
struct node *cap;

void initialiseNode(struct node *target), llistDel(struct node *target),
    unload();
bool isPrime(int value, struct node *llist);

int main(void)
{
    // Intialise our memory for the llist, both ways
    int nodeSize = sizeof(node);

    root = malloc(nodeSize);
    if (root == NULL)
    {
        free(root);
        printf("Error: Memory allocation failed at _root_.\n");
        return 1;
    }
    initialiseNode(root);

    cap = malloc(nodeSize);
    if (cap == NULL)
    {
        free(cap);
        free(root);
        printf("Error: Memory allocation failed at _cap_.\n");
        return 1;
    }
    initialiseNode(cap);

    root->next = cap;
    cap->prev = root;

    int currentNumber = 1;
    int primeCount = 0;

    // Construct our llist of primes, when we've made the 10001st, stop
    do
    {
        currentNumber++;
        // First identify if the current number is prime
        if (isPrime(currentNumber, cap->prev))
        {
            // If it is, let's make a new element of the list
            primeCount++;

            struct node *new_node;
            new_node = malloc(nodeSize);
            if (new_node == NULL)
            {
                unload();
                printf("Error: Memory allocation failed at prime %i (%i).\n", primeCount, currentNumber);
                return 1;
            }

            // Intialise
            new_node->value = currentNumber;
            new_node->position = primeCount;

            // Push the node into the first position
            new_node->prev = root;
            new_node->next = root->next;
            root->next = new_node;
            new_node->next->prev = new_node;

            printf("The %i(st/nd/rd/th) primes is %i.\n", primeCount, currentNumber);
        }
    }
    while(primeCount != TARGET_POSITION);

    unload();
}

// Return true if non of our existing primes can divide it
bool isPrime(int value, struct node *llist)
{
    // I just know that 2 will be the first member of this list
    if (value == 2)
        return true;

    //Otherwise, we need to start running our llist
    while(llist->prev != NULL)
    {
        // As soon as one of our primes divides the number, return false
        if (value % llist->value == 0)
            return false;

        // Check next in the list
        llist = llist->prev;
    }

    // If you're at the end of the llist, go ahead and retrun true
    return true;
}

void initialiseNode(struct node *target)
{
    target->value = 0;
    target->position = 0;
}

void unload()
{
    if (root->next != NULL)
        llistDel(root->next);

    free(root);
}

void llistDel(struct node *target)
{
    if (target->next != NULL)
        llistDel (target->next);

    free(target);
}