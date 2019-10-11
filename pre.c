#include <stdio.h>

#define MAX_INPUT   10 // At most ten states given
#define MAX_LENGTH  12 // Longest state name "Rhode Island" contains 12 characters

int main(void)
{
    char states[MAX_INPUT][MAX_LENGTH + 1]; // Length plus one for null terminator
    int pop, count = 0;

    while(scanf("%s", states[count]) != EOF && scanf("%d", &pop) != EOF)
    {
        if(pop > 10) // Only increment if population requirement met, else overwrite current entry on next iteration
            ++count;
    }

    for(int i = 0; i < count; i++)
        printf("%s\n", states[i]);

    return 0;
}