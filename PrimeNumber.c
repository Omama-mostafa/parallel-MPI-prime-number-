#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
    int rank, noPro, tag = 0;
    int i, j, flag, sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &noPro);

    if(rank == 0)
    {
        int num1, num2;
        printf("\nEnter a first number : ");
        scanf("%d", &num1);

        printf("\nEnter a second number : ");
        scanf("%d", &num2);

        int size, portion, rem;
        size = num2 - num1;
        int *arr = (int*)malloc(size * sizeof(int));

        int num = num1;
        for(i=0; i< size; i++)
        {
            arr[i] = num;
            num++;
        }
        portion = size/ noPro;
        rem = size % noPro;
        printf("\num1 = %d\n", num1);

        printf("\num2 = %d\n", num2);
        for(i=1; i<noPro; i++)
        {
            MPI_Send(&portion, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            MPI_Send(&num1, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            MPI_Send(&num2, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            MPI_Send(arr+i, portion, MPI_INT, i, tag, MPI_COMM_WORLD);
        }
        int totalSum = 0;
        for(i=1; i<noPro; i++)
        {
            int sum;
            MPI_Recv(&sum, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
            totalSum += sum;
        }
        printf("SUM = %d", totalSum);
    }
    else
    {
        int portion, num1, num2;
        MPI_Recv(&portion, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&num1, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&num2, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        int *subArr = malloc(portion*sizeof(int));
        MPI_Recv(subArr, portion, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

        for(i=0; i<portion; i++)
            printf("---> %d", *(subArr + i));
        for(i=num1; i<portion; i++)
        {
            if(i <= 1)
                continue;
            flag = 0;
            for(j=2; j<=i/2; i++)
            {
                if((i % j) == 0)
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
                sum += *(subArr+i);
        }
        printf("---> %d", sum);
        MPI_Send(&sum, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
