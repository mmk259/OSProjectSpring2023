#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 10

void fifoRule (int myRef[], int myFrame, int size);
void optimalRule(int myRef[], int myFrame, int size);
void lfuRule(int myRef[], int myFrame, int size);

int main()
{
    // Opening the input file for counting the number of lines in the input file
    FILE *file;
    char filename[] = "Input.txt";
    file = fopen(filename, "r");

    int size = 0; 
    char temp[MAX_SIZE];

    while (fgets(temp, MAX_SIZE, file) != NULL)
    {
        if (strlen(temp) > 0)
        {
            size += 1;
        }
    }

    printf("Size of Reference String is: %d\n", size);
    fclose(file);
    
    // Opening the input file for reading the reference string from the input file
    file = fopen(filename, "r");

    int myRef[size]; 
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
    
    // Getting the number of frames to use from the user
    int myFrame; 

    do
    {
        
        printf("\nPlease enter how many frames to use (from range: 3-6): ");
        scanf("%d", &myFrame);

    } 
    while (myFrame < 3 || myFrame > 6);

    // Getting the algorithm choice from the user and calling the selected algorithm
    int choice = 0; 

    do
    {
        printf("\n Choose any algorithm to implement \n");
        printf("1. FIFO Rule\n");
        printf("2. Optimal Rule\n");
        printf("3. LFU Rule\n\n");
        scanf("%d", &choice);

    } 
    
    while (choice < 1 || choice > 3);

    if (choice == 1)
    {
    	printf("\nApplying FIFO Page Replacement Algorithm\n");
        fifoRule(myRef, myFrame, size);
    }
    else if (choice == 2)
    {
    	printf("\nApplying Optimal Page Replacement Algorithm\n");
        optimalRule(myRef, myFrame, size);
    }
    else
    {
    	printf("\nApplying LFU Page Replacement Algorithm\n");
        lfuRule(myRef, myFrame, size);
    }
    
    return 0;
}

void fifoRule(int myRef[], int myFrame, int size)
{
    FILE *fout;
    // Open output file for writing
    
    fout = fopen("FIFO.txt", "w");       
    int i, j, flag, popped;
    
    // Allocate memory for the frame
    int *temp = (int *)malloc(sizeof(int) * myFrame);
    
    // Initialize the frame with zeros
    memset(temp, 0, sizeof(int) * myFrame);
    bool isPresent;
    
    // Index to keep track of the oldest page
    int head = 0;

    for (i = 0; i < size; i++)
    {
        // Initialize flag to indicate if page is present in frame
        isPresent = false;
        for (j = 0; j < myFrame; j++)
        {
            // Check if page is already present in the frame
            if (temp[j] == myRef[i])
            {
                // Set flag to true if page is present
                isPresent = true;
                // Exit the loop since page is found
                break;
            }
        }
        // If page is not present in frame, perform page replacement
        if (!isPresent)
        {
            // If the oldest page is not empty, replace it with the current page
            if (temp[head] != 0)
            {
                temp[head] = myRef[i];
                // Update the head index in a circular manner
                head = (head + 1) % myFrame;
            }
            // If the oldest page is empty, simply replace it with the current page
            else
            {
                temp[head] = myRef[i];
            }
        }
        
        // Print the current page reference
        printf ("\n%d  :   ", myRef[i]);
        // Write the current page reference to the output file
        fprintf(fout, "%d  :   ", myRef[i]);

        // If the page is already present in the frame, no page fault occurred
        if (isPresent)
        {
            printf("NO Page fault");
            fprintf(fout, "NO Page fault");
            fprintf(fout, "\n");
        }
        // If the page is not present in the frame, a page fault occurred
        else
        {
            // Print the page value
            for (j = 0; j < myFrame; j++)
            {
                printf("| ");
                fprintf(fout, "| ");
                if (temp[j] != 0)
                {
                    popped = temp[j];
                    // Print the page value
                    printf("%d | ", popped);
                    // Write the page value to the output file
                    fprintf(fout, "%d | ", popped);
                }
                else
                {
                    // Print a dash "-" to indicate an empty frame
                    printf("- | ");
                    // Write a dash "-" to the output file to indicate an empty frame
                    fprintf(fout, "- | ");
                }
            }
            
            // Write a newline character to the output file after printing the frame status
            fprintf(fout, "\n");
        }
    }

    printf("\n");
    
    fprintf(fout, "\n");

    // Close the output file
    fclose(fout);
    // Free the dynamically allocated memory for the frame
    free(temp);
}

//this functions take three parameters:
1. an array myRef[] representing the sequence of memory references
2. an myFrame representing the number of available page frames
3. and an size representing the size of the myRef[] array.


void optimalRule(int myRef[], int myFrame, int size)
{
    FILE *fout;
    fout = fopen("output_Optimal.txt", "w");  // opens a file which will used to store the output

    int myVector[size];  // inititalized the array with equal size to represent the current state of the page frames
    memset(myVector, 0, sizeof(myVector)); // memset function is used to initialize all elements of array to 0.

    for (int i = 0; i < size; i++)
    {
        int temp[size];   // this temporary array will store the current non-empty pages in the page frame
        int tempIndex = 0;  // it will keep track the number of non empty pages stored in temproray array
        int flag = 1;    // flag is to set 1 to indicate a page fault
        for (int j = 0; j < size; j++)
        {
            if (myVector[j] == myRef[i]) //search for the current memory reference if found then flag set to 0.
            {
                flag = 0; // indicating there is no page fault
            }
            if (myVector[j] != 0)
            {
                temp[tempIndex] = myVector[j]; // non zero elements are copied in temproray array to maintain their order.
                tempIndex++;
            }
        }
        if (flag == 1) // page fault occured
        {
//There are two scenarios for handling the page fault:
//either there is an empty slot in myVector[], or all slots are occupied.
            if (myVector[0] == 0 || tempIndex < myFrame) // if there is an empty slot the current memory reference is added to the first empty slot
            {
                myVector[tempIndex] = myRef[i];
            }
//If all slots are occupied, the code predicts which page in myVector[] will not be used for the longest period in the future. It does this by searching for the next occurrence of each page in myVector[] after the current position (i) in the myRef[] array.
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
//If a page in myVector[] is not found in the remaining memory references (myRef[]), it means it will not be used in the future, and that page is replaced with the current memory reference.
                    if (index >= myFrame)
                    {
                        myFlag = 1;
                    }
                }
//If all pages in 'myVector[]' are predicted to be used in the future, then
//inds the furthest occurrence of any page in myVector[] and replaces that page with the current memory reference.
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
    fout = fopen("LFU.txt", "w");

    // Array to store the pages in the frames
    int myArray[myFrame];
    // Array to keep track of the frequency count of each page
    int countFrequency[myFrame];
    // Array to keep track of the time of the last access for each page
    int timeFrequency[myFrame];

    //loop to initialize the pages in the frames to -1 (empty), frequency count of each page to 0 & time of the last access for each page to 0 
    for (int j = 0; j < myFrame; j++)
    {
        myArray[j] = -1;
        countFrequency[j] = 0;
        timeFrequency[j] = 0;
    }

    for (int i = 0; i < size; i++)
    {
        // Flag to check if a page is present in the frames
        int flag = 1;
        
        for (int j = 0; j < myFrame; j++)
        {
            if (myRef[i] == myArray[j])
            {
                // Page is already present in the frames
                flag = 0;
                // Increment the frequency count of the page
                countFrequency[j]++;
                break;
            }
        }

        if (flag == 1)
        {
            // Index of the page with the minimum frequency count
            int min = 0;
            
            for (int k = 1; k < myFrame; k++)
            {
                if (countFrequency[k] < countFrequency[min])
                {
                    // Update the index of the page with the minimum frequency count
                    min = k;
                }
                else if (countFrequency[k] == countFrequency[min] && timeFrequency[k] > timeFrequency[min])
                {
                    // If two pages have the same frequency count, choose the one that was accessed earlier
                    min = k;
                }
            }
            
            // Replace the page with the minimum frequency count
            myArray[min] = myRef[i];
            // Reset the frequency count of the new page to 1 & time of the last access for the new page to 0
            countFrequency[min] = 1;
            timeFrequency[min] = 0;
        }

        printf("\n%d   :   ", myRef[i]);
        fprintf(fout, "%d   :   ", myRef[i]);

        if (flag == 0)
        {
             // Page is already present in the frames
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
                    // Increment the time of the last access for each page in the frames
                    timeFrequency[j]++;
                    printf("%d | ", myArray[j]);
                    fprintf(fout, "%d | ", myArray[j]);
                }
                
                else
                {
                    // Empty frame
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


