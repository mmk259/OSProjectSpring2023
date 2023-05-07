#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_SIZE 10

void fifoRule(int myRef[], int myFrame, int size);
void optimalRule(int myRef[], int myFrame, int size);
void lfuRule(int myRef[], int myFrame, int size);

int main()
{
    //
    // Calculating the size of reference string from text file given
    //

    FILE *file;
    char filename[] = "input.txt";
    file = fopen(filename, "r");

    int size = 0; // size of Reference String
    char temp[MAX_SIZE];

    while (fgets(temp, MAX_SIZE, file) != NULL)
    {
        if (strlen(temp) > 0)
        {
            size += 1;
        }
    }

    printf("Size of reference string is: %d\n", size);
    fclose(file);

    //
    // Storing all the elements from text file into an integer array (size of array calculated above)
    //

    file = fopen(filename, "r");

    int myRef[size]; // integer array
    int looper = 0;

    while (fgets(temp, MAX_SIZE, file) != NULL)
    {
        if (strlen(temp) == 0)
        {
            continue;
        }
        else
        {
            myRef[looper] = atoi(temp);
        }
        looper += 1;
    }

    //
    // Getting total frames as input from user
    //

    int myFrame; // total frames from user

    do
    {

        printf("\nPlease enter how many frames to use (range: 3-6): ");
        scanf("%d", &myFrame);

    } while (myFrame < 3 || myFrame > 6);

    int choice = 0; // choose algorithm to use from user

    do
    {

        printf("\n--- Please choose which algorithm to use ---\n");
        printf("1. FIFO Rule\n");
        printf("2. Optimal Rule\n");
        printf("3. LFU Rule\n\n");
        scanf("%d", &choice);

    } while (choice < 1 || choice > 3);

    printf("\n___Applying Page Replacement Algorithm___\n");
    if (choice == 1)
    {
        fifoRule(myRef, myFrame, size);
    }
    else if (choice == 2)
    {
        optimalRule(myRef, myFrame, size);
    }
    else
    {
        lfuRule(myRef, myFrame, size);
    }
    return 0;
}

void fifoRule(int myRef[], int myFrame, int size)
{
    FILE *fout;
    fout = fopen("output_FIFO.txt", "w");
    int i, j, flag, popped;
    int *temp = (int *)malloc(sizeof(int) * myFrame);
    memset(temp, 0, sizeof(int) * myFrame);
    bool isPresent;
    int head = 0;

    for (i = 0; i < size; i++)
    {
        isPresent = false;
        for (j = 0; j < myFrame; j++)
        {
            if (temp[j] == myRef[i])
            {
                isPresent = true;
                break;
            }
        }
        if (!isPresent)
        {
            if (temp[head] != 0)
            {
                temp[head] = myRef[i];
                head = (head + 1) % myFrame;
            }
            else
            {
                temp[head] = myRef[i];
            }
        }

        printf("\n%d   :   ", myRef[i]);
        fprintf(fout, "%d   :   ", myRef[i]);

        if (isPresent)
        {
            printf("NO Page fault");
            fprintf(fout, "NO Page fault");
            fprintf(fout, "\n");
        }
        else
        {
            for (j = 0; j < myFrame; j++)
            {
                printf("| ");
                fprintf(fout, "| ");
                if (temp[j] != 0)
                {
                    popped = temp[j];
                    printf("%d | ", popped);
                    fprintf(fout, "%d | ", popped);
                }
                else
                {
                    printf("- | ");
                    fprintf(fout, "- | ");
                }
            }
            fprintf(fout, "\n");
        }
    }

    printf("\n");
    fprintf(fout, "\n");

    fclose(fout);
    free(temp);
}

void optimalRule(int myRef[], int myFrame, int size)
{
    FILE *fout;
    fout = fopen("output_Optimal.txt", "w");

    int myVector[size];
    memset(myVector, 0, sizeof(myVector));

    for (int i = 0; i < size; i++)
    {
        int temp[size];
        int tempIndex = 0;
        int flag = 1;
        for (int j = 0; j < size; j++)
        {
            if (myVector[j] == myRef[i])
            {
                flag = 0;
            }
            if (myVector[j] != 0)
            {
                temp[tempIndex] = myVector[j];
                tempIndex++;
            }
        }
        if (flag == 1)
        {
            if (myVector[0] == 0 || tempIndex < myFrame)
            {
                myVector[tempIndex] = myRef[i];
            }
            else
            {
                int index = 0;
                int myFlag = 0;
                for (int j = i + 1; j < size; j++)
                {
                    if (index < myFrame)
                    {
                        if (myVector[index] == myRef[j])
                        {
                            index++;
                        }
                    }
                    if (index >= myFrame)
                    {
                        myFlag = 1;
                    }
                }
                if (myFlag == 0)
                {
                    myVector[index] = myRef[i];
                }
                else
                {
                    int tempj = i + 1;
                    index = 0;
                    for (int j = i + 1; j < size; j++)
                    {
                        if (index < myFrame)
                        {
                            if (myVector[index] == myRef[j])
                            {
                                index++;
                                if (tempj < j)
                                {
                                    tempj = j;
                                }
                                j = i + 1;
                            }
                        }
                    }
                    for (int index = 0; index < myFrame; index++)
                    {
                        if (myVector[index] == myRef[tempj])
                        {
                            myVector[index] = myRef[i];
                        }
                    }
                }
            }
        }
        printf("\n%d   :   ", myRef[i]);
        fprintf(fout, "%d   :   ", myRef[i]);

        int tempSize = tempIndex;
        for (int j = tempSize - 1; j >= 0; j--)
        {
            printf("| %d | ", temp[j]);
            fprintf(fout, "| %d | ", temp[j]);
        }
        for (int j = 0; j < myFrame - tempSize; j++)
        {
            printf("| - | ");
            fprintf(fout, "| - | ");
        }

        if (flag == 0)
        {
            printf("NO Page fault");
            fprintf(fout, "NO Page fault");
            fprintf(fout, "\n");
        }
        else
        {
            fprintf(fout, "\n");
        }
    }
    printf("\n");
    fprintf(fout, "\n");

    fclose(fout);
}

void lfuRule(int myRef[], int myFrame, int size)
{
    FILE *fout;
    fout = fopen("output_LFU.txt", "w");

    int myArray[myFrame];
    int countFrequency[myFrame];
    int timeFrequency[myFrame];

    for (int j = 0; j < myFrame; j++)
    {
        myArray[j] = -1;
        countFrequency[j] = 0;
        timeFrequency[j] = 0;
    }

    for (int i = 0; i < size; i++)
    {
        int flag = 1;
        for (int j = 0; j < myFrame; j++)
        {
            if (myRef[i] == myArray[j])
            {
                flag = 0;
                countFrequency[j]++;
                break;
            }
        }

        if (flag == 1)
        {
            int min = 0;
            for (int k = 1; k < myFrame; k++)
            {
                if (countFrequency[k] < countFrequency[min])
                {
                    min = k;
                }
                else if (countFrequency[k] == countFrequency[min] && timeFrequency[k] > timeFrequency[min])
                {
                    min = k;
                }
            }
            myArray[min] = myRef[i];
            countFrequency[min] = 1;
            timeFrequency[min] = 0;
        }

        printf("\n%d   :   ", myRef[i]);
        fprintf(fout, "%d   :   ", myRef[i]);

        if (flag == 0)
        {
            printf("NO Page fault");
            fprintf(fout, "NO Page fault");
            fprintf(fout, "\n");
        }
        else
        {
            for (int j = 0; j < myFrame; j++)
            {
                printf("| ");
                fprintf(fout, "| ");

                if (myArray[j] != -1)
                {
                    timeFrequency[j]++;
                    printf("%d | ", myArray[j]);
                    fprintf(fout, "%d | ", myArray[j]);
                }
                else
                {
                    printf("- | ");
                    fprintf(fout, "- | ");
                }
            }
            fprintf(fout, "\n");
        }
    }
    printf("\n");
    fprintf(fout, "\n");

    fclose(fout);
}