#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <thread>
#include <ostream>
#include <iostream>

#define NTHREADS 8
#define HOURS_DURATION 10

int threadID = -1;
int temps[NTHREADS*60];
int hours_passed = 0;

//-100F to 70F
void taketemp(int threadID)
{
    temps[threadID] = -100 + rand() % (( 70 + 1 ) + 100);
    //std::cout<<"temp is " << temps[threadID] << std::endl; 
}

void maxtemps()
{
    int maxtemps[5] = {0, 0, 0, 0, 0};
    for(int i = 0; i<NTHREADS*60; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(temps[i]> maxtemps[j])
            {
                //std::cout<<"temps[i] = " <<temps[i] << "and maxtemps [j] = " <<maxtemps[j] <<std::endl;
                maxtemps [j] = temps [i];
                break;
            }
        }
    }

    printf("max temps in hour %d are: ", hours_passed+1);
    for(int i = 0; i < 5; i++)
    {
        printf("%d,", maxtemps[i]);
    }
    printf("\n");
}

void mintemps()
{
    int mintemps[5] = {0, 0, 0, 0, 0};
    for(int i = 0; i<NTHREADS*60; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(temps[i]< mintemps[j])
            {
                //std::cout<<"temps[i] = " <<temps[i] << "and mintemps [j] = " <<mintemps[j] <<std::endl;
                mintemps [j] = temps [i];
                break;
            }
        }
    }

    printf("min temps in hour %d are: ", hours_passed+1);
    for(int i = 0; i < 5; i++)
    {
        printf("%d,", mintemps[i]);
    }
    printf("\n");
}

void maxinterval()
{

    int maxdiff = 0;
    int maxinterval = -1;

    for (int x=0;x < 60-10; x++)
    {
        int diff = 0;
        int min = 171, max = -100;

        for (int m=x; m < x+10; m++)
        {
            for (int i=0; i < NTHREADS; i++)
            {
                if (temps[x*8+i] < min)
                    min = temps[x*8+i];
                if (temps[x*8+i] > max)
                    max = temps[x*8+i];
            }
        }
        if (max-min > maxdiff)
        {
            maxdiff = max-min;
            maxinterval = x;
        }
    }

    std::cout << "max range: " << maxdiff << " at minute " << maxinterval << std::endl;


}

int main()
{
    std::thread array[NTHREADS];
    srand(time(0));

    while(hours_passed != HOURS_DURATION)
    {
        int minutes_passed = 0;
        while(minutes_passed != 60)
        {
            for(int i = 0; i<NTHREADS; i++)
            {
                array[i] = std::thread(taketemp, i+(minutes_passed*8));
                array[i].join();
            }
            minutes_passed++;
            //std::cout<<minutes_passed<<std::endl;
        }
        
        maxtemps();
        mintemps();
        maxinterval();
        hours_passed++;
        //std::cout<<"hour: "<< hours_passed<<std::endl;
    }


    

/*    for(int i=0;i<NTHREADS;i++)
    {
        threadID = 0;
        for(int i=0; i < )
        threadID++;
        pthread_create(&array[i], NULL, taketemp, threadID);
    }
*/
}