#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4
#define NPRESENTS 500000

/*
If you want to track progress, uncomment the print statements on lines 73, 100, 114, 127. it will be slow if you do, i recommend reducing present count first.
*/

struct present;

struct present
{
    struct present *next;
    int present_number;
};

int nextbag = 0;
struct present bag[NPRESENTS];
pthread_mutex_t baglock;

int notecount = 0;
int listcount = 0;
struct present *list; 
pthread_mutex_t listlock;

void swap(struct present *one, struct present *two)
{
    struct present tmp;
    tmp.next = one->next;
    tmp.present_number = one->present_number;
    one->next = two->next;
    one->present_number = two->present_number;
    two->next = tmp.next;
    two->present_number = tmp.present_number;
}

void init_shuffle_presents()
{
    for(int i=0;i<NPRESENTS;i++)
    {
        bag[i].present_number = i;
        bag[i].next = NULL;
    }

    for(int i=0;i<NPRESENTS;i++)
        swap(&bag[i], &bag[(random() % NPRESENTS)]);
}

void *servant(void *x)
{
    int id = (int)x;

    while(nextbag < NPRESENTS || notecount < NPRESENTS)
    {
        struct present *p;
        pthread_mutex_lock(&baglock);

        // if there is a present in the bag, get it and link it in the list
        if (nextbag < NPRESENTS)
        {
            p = &bag[nextbag];
            nextbag++;
        }
        else
        {
            p = NULL;
        }

        //if (p) printf("%d: got %d and nextbag is %d\n", id, p->present_number, nextbag);

        pthread_mutex_unlock(&baglock);

        // we got a present put in list
        if (p)
        {
            pthread_mutex_lock(&listlock);
            if (list == NULL || list->present_number > p->present_number)
            {
                p->next = list;
                list = p;
            }
            else
            {
                struct present *current = list;
                while (current->next != NULL &&
                        current->next->present_number < p->present_number)
                {
                    current = current->next;
                }
                p->next = current->next;
                current->next = p;
            }

            listcount++;

            //printf("%d: added %d, listcount %d\n", id, p->present_number, listcount);

            pthread_mutex_unlock(&listlock);
        }

        // remove a random present from the queue and write a note
        pthread_mutex_lock(&listlock);
        if (listcount)
        {
            int n = random()%listcount;
            struct present *c = list, *p = NULL;

            if (n==0)
            {
                //printf("%d: note written for %d\n", id, c->present_number);
                list = list->next;
            }
            else
            {
                for(int i=0;i<n;i++)
                {
                    p = c;
                    c = c->next;
                }

                p->next = c->next;

                //printf("%d: note written for %d\n", id, c->present_number);

            }
            notecount++;
            listcount--;
        }
        pthread_mutex_unlock(&listlock);
    }

printf("%d: notecount %d\n", id, notecount);
//printf("%d: nextbag %d\n", id, nextbag);

    return NULL;
}

int is_in_list(int n)
{
    struct present *p = NULL;
    int ret = 0;

    pthread_mutex_lock(&listlock);

    p = list;
    while(p)
    {
        if (p->present_number == n)
        {
            ret = 1;
            break;
        }
        p = p->next;
    }

    pthread_mutex_unlock(&listlock);

    return ret;
}

int main()
{
    pthread_t array[NTHREADS];

    srandom(time(NULL));
    init_shuffle_presents();

    pthread_mutex_init(&baglock, NULL);
    pthread_mutex_init(&listlock, NULL);

    for(int i=0;i<NTHREADS;i++)
        pthread_create(&array[i], NULL, servant, (void*)i);

    //arbitrarily assuming the minotaur only asks 100 times whether a random present is in the list, can be adjusted. 
    //it's almost never on the list with this implementation though. so it doesn't matter
    //because presents are pulled back off as soon as they're put on, there's only a very small window
    //where they will show up as present in this search
    for (int i=0; i<100; i++)
    printf("present %d is %s the list\n", random()%(NPRESENTS+1), is_in_list(random()%(NPRESENTS+1)) ? "in" : "not in");

    for(int i=0;i<NTHREADS;i++)
        pthread_join(array[i], NULL);

    return 0;
}
