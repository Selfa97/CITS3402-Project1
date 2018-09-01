#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define ARRAYSIZE 128

int readFile(int ***state)
{
    //INIT STATIC STARTING GAME STATE FROM FILE
    char *fname = "start.txt";
    FILE *f = fopen(fname, "r");
    int **arr = *state;

    if (f == NULL)
    {
        printf("Error: Cannot open file - %s", fname);
        return (EXIT_FAILURE);
    }

    //READ IN GAME STATE FROM FILE PROVIDED
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        for (int j = 0; j < ARRAYSIZE; j++)
        {
            int num;
            if (fscanf(f, "%d", &num) != 1)
            {
                printf("Error reading in %s", fname);
                return (EXIT_FAILURE);
            }
            else
            {
                arr[i][j] = num;
            }
        }
    }
    fclose(f);
    return (EXIT_SUCCESS);
}

int main()
{
    //ALLOCATE MEMORY FOR GAME STATE
    int **state = (int **)malloc(ARRAYSIZE * sizeof(int *));
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        state[i] = (int *)malloc(ARRAYSIZE * sizeof(int));
    }

    //READ STATIC GAME STATE FROM FILE
    if (readFile(&state) == EXIT_FAILURE)
    {
        return (EXIT_FAILURE);
    }

    //TEST INTPUT READ
    printf("ROW 1:\n");
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        printf("%d ", state[0][i]);
    }
    printf("\n");

    printf("ROW 10:\n");
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        printf("%d ", state[9][i]);
    }
    printf("\n");

    printf("ROW 20:\n");
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        printf("%d ", state[19][i]);
    }
    printf("\n");

    printf("ROW 128:\n");
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        printf("%d ", state[127][i]);
    }
    printf("\n");
}