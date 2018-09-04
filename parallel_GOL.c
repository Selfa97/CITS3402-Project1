#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define ARRAYSIZE 2048

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

//FREE ALLOCATED GAME STATE MEMORY
void freeMem(int **state)
{
    for(int i = 0; i < 2 * ARRAYSIZE; i++)
    {
        free(state[i]);
    }
    free(state);
}

//GET NUMBER OF NEIGHBOURS
int getNeighbours(int **state, int r, int c)
{
    int live = 0;
    //WRAP AROUND OF CELLS
    int north = (r-1 < 0) ? ARRAYSIZE-1 : r-1;
    int south = (r+1)%ARRAYSIZE;
    int west = (c-1 < 0) ? ARRAYSIZE-1 : c-1;
    int east = (c+1)%ARRAYSIZE;

    //CHECK NEIGHBOURS
    live += state[north][c];
    live += state[south][c];
    live += state[r][west];
    live += state[r][east];

    return live;
}

//PRINT STATE TO FILE
int printState(int **state, int step) 
{
    char *dir = "./Parallel_Figs";
    char fname[BUFSIZ];
    sprintf(fname, "%s/par_state_%d.txt", dir, step);
    FILE *f = fopen(fname, "w");

    if(f == NULL)
    {
        printf("Error creating file: %s", fname);
        return(EXIT_FAILURE);
    }

    for(int i = 0; i < ARRAYSIZE; i++)
    {
        for(int j = 0; j < ARRAYSIZE; j++)
        {
            fprintf(f, "%d ", state[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return(EXIT_SUCCESS);
}

int main()
{
    double begin = omp_get_wtime();

    //ALLOCATE MEMORY FOR GAME STATE & TEMP GAME STATE -> OFFSET INDEX
    int **state = (int **)malloc(2 * ARRAYSIZE * sizeof(int *));
    for (int i = 0; i < 2 * ARRAYSIZE; i++)
    {
        state[i] = (int *)malloc(ARRAYSIZE * sizeof(int));
    }

    //READ STATIC GAME STATE FROM FILE
    if(readFile(&state) == EXIT_FAILURE)
    {
        return (EXIT_FAILURE);
    }

    //GAME LOOP
    for(int step = 1; step <= 100; step++)
    {
        //GAME LOGIC -- PARALLEL
        #pragma omp parallel for
        for(int i = 0; i < ARRAYSIZE; i++)
        {
            for(int j = 0; j < ARRAYSIZE; j++)
            {
                int curr = state[i][j];
                int neighbours = getNeighbours(state, i, j);

                //IF ALIVE
                if(curr)
                {
                    //WITH TOO FEW OR TOO MANY NEIGHBOURS -> DEAD
                    if(neighbours < 2 || neighbours == 4)
                    {
                        state[ARRAYSIZE + i][j] = 0;
                    }
                    //OTHERWISE ALIVE
                    else
                    {
                        state[ARRAYSIZE + i][j] = 1;
                    }
                }
                //IF DEAD WITH 3 NEIGHBOURS -> ALIVE
                else if(neighbours == 3)
                {
                    state[ARRAYSIZE + i][j] = 1;
                }
                //OTHERWISE DEAD
                else
                {
                    state[ARRAYSIZE + i][j] = 0;
                }
            }
        }
        
        //TRANSFER TEMP STATE TO CURR STATE -- PARALLEL
        #pragma omp parallel for
        for(int i = 0; i < ARRAYSIZE; i++)
        {
            for(int j = 0; j < ARRAYSIZE; j++)
            {
                state[i][j] = state[ARRAYSIZE + i][j];
            }
        }

        //PRINT STATE RESULT TO FILE AT 24, 25, 49, 50, 74, 75, 99, 100
        if(step % 25 == 24 || step % 25 == 0)
        {
            if(printState(state, step) == EXIT_FAILURE)
            {
                return(EXIT_FAILURE);
            }
        }
    }

    //FREE ALLOCATED MEMORY
    freeMem(state);

    printf("Time executed: %f\n", (omp_get_wtime() - begin));
    return(EXIT_SUCCESS);
}