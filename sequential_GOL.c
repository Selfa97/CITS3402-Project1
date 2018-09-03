#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define ARRAYSIZE 512

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
    for(int i = 0; i < ARRAYSIZE; i++)
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
    int north = (r-1 < 0) ? 127 : r-1;
    int south = (r+1)%128;
    int west = (c-1 < 0) ? 127 : c-1;
    int east = (c+1)%128;

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
    char fname[16];
    sprintf(fname, "seq_state_%d.txt", step);
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

    //ALLOCATE MEMORY FOR GAME STATE
    int **state = (int **)malloc(ARRAYSIZE * sizeof(int *));
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        state[i] = (int *)malloc(ARRAYSIZE * sizeof(int));
    }

    //ALLOCATE MEMORY FOR TEMP GAME STATE
    int **tmp = (int **)malloc(ARRAYSIZE * sizeof(int *));
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        tmp[i] = (int *)malloc(ARRAYSIZE * sizeof(int));
    }

    //READ STATIC GAME STATE FROM FILE
    if(readFile(&state) == EXIT_FAILURE)
    {
        return (EXIT_FAILURE);
    }

    //GAME LOOP
    for(int step = 1; step <= 100; step++)
    {
        //GAME LOGIC
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
                        tmp[i][j] = 0;
                    }
                    //OTHERWISE ALIVE
                    else
                    {
                        tmp[i][j] = 1;
                    }
                }
                //IF DEAD WITH 3 NEIGHBOURS -> ALIVE
                else if(neighbours == 3)
                {
                    tmp[i][j] = 1;
                }
                //OTHERWISE DEAD
                else
                {
                    tmp[i][j] = 0;
                }
            }
        }
        state = tmp;

        //PRINT STATE RESULT TO FILE AT 25, 50, 75, 100
        if(step % 25 == 0)
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
}