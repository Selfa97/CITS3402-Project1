#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ARRAYSIZE 512

int main()
{
    FILE *f = fopen("start.txt", "w");

    if(f == NULL)
    {
        printf("Error: File could not be opened!\n");
    }

    srand((unsigned)time(NULL));

    for(int i = 0; i < ARRAYSIZE; i++)
    {
        for(int j = 0; j < ARRAYSIZE; j++)
        {
            fprintf(f, "%d ", rand() % 2);
        }
        fprintf(f, "\n");
    }
    fclose(f);

    return(EXIT_SUCCESS);
}