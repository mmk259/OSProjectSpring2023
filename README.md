# Project Report:
# ______________________________________________________
# Page Replacement Algorithm by Kernel Threads


# INTRODUCTION:
Page replacement algorithms are the techniques used to replace the pages in a page frame that are no longer required by the program running in the operating system. These algorithms are used to minimize the number of page faults, which is the situation when a requested page is not found in the memory and needs to be swapped in from disk. This project aims to implement three different page replacement algorithms: First-In-First-Out (FIFO), Optimal, and Least Frequently Used (LFU).
The input to the program is taken from a text file containing the reference string, which is the sequence of pages requested by the program. The program reads this file twice, first to calculate the size of the reference string and second to store the reference string in an integer array. The user is then prompted to input the number of frames to be used for page replacement and to choose which algorithm to use.

# DESCRIPTION:
The first algorithm implemented is the FIFO rule. In this algorithm, the page that has been in the memory for the longest time is replaced. The program creates a temporary array of size equal to the number of frames and initializes it with 0. For each page in the reference string, the program checks if the page is already present in the temporary array. If it is, there is no page fault, and the program moves on to the next page. If it is not, the program replaces the page that has been in the memory for the longest time with the new page. The program also maintains a head pointer that points to the oldest page in the temporary array. If the head pointer reaches the end of the array, it is set back to the beginning. The program outputs each page request and the contents of the frames, indicating whether there was a page fault or not, and writes this information to a text file named output_FIFO.txt.

The second algorithm implemented is the Optimal rule. In this algorithm, the page that will not be needed for the longest time in the future is replaced. The program creates an array of size equal to the reference string and initializes it with 0. For each page in the reference string, the program checks if the page is already present in the array. If it is, there is no page fault, and the program moves on to the next page. If it is not, the program searches the array to find the page that will not be needed for the longest time in the future. If such a page is found, it is replaced with the new page. If not, the page that was requested the longest time ago is replaced. The program outputs each page request and the contents of the frames, indicating whether there was a page fault or not, and writes this information to a text file named output_Optimal.txt.

The third algorithm implemented is the LFU rule. In this algorithm, the page that has been used the least number of times is replaced. The program creates a temporary array of size equal to the number of frames and initializes it with 0. For each page in the reference string, the program checks if the page is already present in the temporary array. If it is, the program increments a counter associated with the page. If it is not, the program replaces the page that has been used the least number of times with the new page. If multiple pages have been used the same number of times, the program replaces the one that has been in the memory for the longest time. The program outputs each page request and the contents of the frames, indicating whether there was a page fault or not, and writes this information to a text file named output_LFU.txt.

# Implementation:

# Optimal Algorithm Implementation:

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


# Conclusion:

In conclusion, this project successfully implements three different page replacement algorithms: First-In-First-Out (FIFO), Optimal, and Least Frequently Used (LFU). The program takes input from a text file containing a sequence of page requests, and simulates the behavior of each algorithm by keeping track of which pages are currently in memory and which ones have been evicted.

Based on the experimental results, it can be concluded that the Optimal algorithm provides the best overall performance in terms of minimizing page faults, followed by LFU and then FIFO. However, the choice of which algorithm to use may depend on the specific characteristics of the system being simulated. For example, if memory is very limited, the LFU algorithm may be more effective because it prioritizes evicting pages that are used infrequently.

Overall, this project demonstrates the importance of page replacement algorithms in managing memory and optimizing system performance. By understanding the trade-offs between different algorithms and their specific use cases, system administrators and developers can make informed decisions about how to design and optimize their systems for maximum efficiency.
