#include <stdio.h>
#include <string.h>

#define MAX_INPUT   10 // Number of input
#define MAX_LENGTH  12 // Max size of input

void quicksort(char A[][MAX_LENGTH+1], int len); // Quicksort I made for a 2D array of strings

int main(void)
{
    char states[MAX_INPUT][MAX_LENGTH + 1]; // +1 length for null terminator
    char temp[MAX_LENGTH + 1];

    int count = 0;
    while(scanf("%s", states[count]) != EOF) // read in
        ++count;

    quicksort(states, count);

    for(int i = 0; i < count; i++)
        printf("%s\n", states[i]);

    return 0;
}

void quicksort(char A[][MAX_LENGTH+1], int len)
{
    if (len < 2)
        return;

    char pivot[MAX_LENGTH + 1]; // pivot is comparator
    strcpy(pivot, A[len / 2]);

    int i = 0;
    int j = len - 1;
    char temp[MAX_LENGTH + 1];

    while(1) 
    {
        // find first to the left of pivot that is larger than pivot
        while (strcmp(A[i], pivot) < 0)
            ++i;
            
        // find first to the right of pivot that is smaller than pivot
        while (strcmp(A[j], pivot) > 0)
            --j;

        // Swap if i (larger than pivot) is left of j (smaller than pivot)
        if(i < j)
        {
            strcpy(temp, A[i]);
            strcpy(A[i], A[j]);
            strcpy(A[j], temp);
        }
        else break;

        ++i;
        --j;
    }

    quicksort(A, i); // left half
    quicksort(A + i, len - i); // right half
}