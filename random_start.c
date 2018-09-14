#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    char f1name[BUFSIZ];

    if(argc != 2) {
        printf("Invalid number of args\n");
        return(EXIT_FAILURE);
    }

    int const ARRAYSIZE = atoi(argv[1]);

    if(ARRAYSIZE == 128 || ARRAYSIZE == 256 || ARRAYSIZE == 512 || ARRAYSIZE == 1024 || ARRAYSIZE == 2048)
    {
        sprintf(f1name, "start_state.txt");

        FILE *f1 = fopen(f1name, "w");

        if(f1 == NULL)
        {
            printf("Error: File could not be opened!\n");
            return(EXIT_FAILURE);
        }

        srand((unsigned)time(NULL));

        for(int i = 0; i < ARRAYSIZE; i++)
        {
            for(int j = 0; j < ARRAYSIZE; j++)
            {
                int num = rand() % 2;
                fprintf(f1, "%d ", num);
            }
            fprintf(f1, "\n");
        }
        fclose(f1);

        return(EXIT_SUCCESS);
    }
    else
    {
        printf("Expected array size of: 128, 256, 512, 1024, 2048\n");
        return(EXIT_FAILURE);
    }
}