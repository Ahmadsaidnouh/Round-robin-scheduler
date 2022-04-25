#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Process: Each process has a name and has an execution time.
 *          We keep track of how long the process still needs
 *          to run on the processor. "remaining_time" is the
 *          remaining execution time of the process.
 */
typedef struct
{
    char *name;
    int starting_time;
    int remaining_time;
} Process;
/*
 *
 */
typedef struct Node
{
    Process data;
    struct Node* next;
} Node;
/*
 *
 */
typedef struct
{
    Node *head , *tail;
} Queue;
/*
 *
 */
Queue* init()
{
    Queue * q = malloc(sizeof(Queue));
    q->head = q->tail = NULL;
    return q;
}
/*
 *
 */
 Node* newNode(Process process)
 {
     Node *n = malloc(sizeof(Node));
     Process p = process;
     n->data = p;
     n->next = NULL;
     return n;
 }
int isEmpty(Queue *q)
{
    if(q->tail == NULL)
        return 1;
    return 0;

}
/*
 *
 */
Process dequeue(Queue *q)
{
    if(q->head)
    {
        Node *temp = q->head;
        q->head = temp->next;
        if(q->head==NULL)
            q->tail=NULL;
        Process pro = temp->data;
        free(temp);
        return pro;
    }
    else
        return ;
}
/*
 *
 */
void enqueue(Queue *q, Process x)
{
    Node *n = newNode(x);
    if(q->head == NULL)
    {
        q->head = q->tail = n;
    }
    else
    {
        q->tail->next = n;
        q->tail = n;
    }
}
/*
 *
 */
void destroy(Queue *q)
{
    free(q->head);
    free(q->tail);
    free(q);
}
/*
 * RoundRobin Scheduling
 */
int numberOfProcesses(char *filename)
{
    FILE *f = fopen(filename, "r");
    int count = 0;
    char temp[100];
    while(!feof(f))
    {

        fgets(temp,100,f);
        count++;
    }
    fclose(f);
    return count - 1;
}
Queue* loadQueue(char *filename)
{
    Queue *q = init();
    FILE *f = fopen(filename, "r");
    char *line = malloc(100);
    fgets(line,100,f);
    Process process;
    int i = 0;
    char *token[numberOfProcesses(filename)];
    while(!feof(f))
    {
        token[i] = strtok( fgets(line,100,f), " ");
        process.name = malloc(10);
        strcpy(process.name, token[i]);
        token[i] = strtok( NULL, " ");
        process.starting_time = atoi(token[i]);
        token[i] = strtok( NULL, " ");
        process.remaining_time = atoi(token[i]);
        enqueue(q,process);
        i++;
    }
    return q;
    fclose(f);
}
int queueLength(Queue *q)
{
    int length = 0;
    Node *ptr = q->head;
    while(ptr)
    {
        length++;
        ptr = ptr->next;
    }
    return length;
}
void printQueue(Queue *q)
{
    Node*temp=q->head;
    while(temp!=NULL)
    {
        printf("%s %d %d\n",temp->data.name,temp->data.starting_time,temp->data.remaining_time);
        temp=temp->next;
    }
}
int timeSlts(char *filename)
{
    int i;
    FILE *f = fopen(filename,"r");
    char *firstLine = malloc(100);
    fgets(firstLine,100,f);
    fclose(f);
    char *token = strtok(firstLine," ");
    for(i = 1; i <=4; i++)
        token = strtok(NULL," ");
    return atoi(token);

}
void RoundRobin(char* filename)
{
    int i, queLength, j;
    int timeSlots = timeSlts(filename);
    Queue *q = loadQueue(filename);

    printf("**************\n");
    printf("Watching time slots = %d\n",timeSlots);
    printQueue(q);
    printf("**************\n");

    for(i = 0; i < timeSlots; i++)
    {
        int isProcessorFree = 1;
        queLength = queueLength(q);
        for(j = 0; j < queLength; j++)
        {

            Node *hold = q->head;
            if(hold->data.starting_time <= i)
            {
                isProcessorFree = 0;
                printf("%s      (%d-->%d)",hold->data.name,i,i+1);
                hold->data.remaining_time--;
                if(hold->data.remaining_time == 0)
                {
                    printf(" %s aborts",hold->data.name);
                    dequeue(q);
                }
                else
                {
                    if(queueLength(q) != 1)
                    {
                        q->head = hold->next;
                        q->tail->next = hold;
                        q->tail = hold;
                        q->tail->next = NULL;
                    }
                }

                break;
            }
            if(queueLength(q) != 1)
            {
                q->head = hold->next;
                q->tail->next = hold;
                q->tail = hold;
                q->tail->next = NULL;

            }
        }
        if(isProcessorFree)
            printf("idle    (%d-->%d)",i,i+1);
        printf("\n");

    }
    printf("Stop\n");
    destroy(q);
}
/*
 *
 */
int main()
{
    char filename[261];
    puts("Enter file name or Ctrl+Z to exit:");
    puts("----------------------------------");
    while(fgets(filename, 260, stdin) != NULL)
    {
        filename[strlen(filename)-1]='\0';
        if(fopen(filename,"r"))
            RoundRobin(filename);
        else
        {
            puts("File Not Found!");
            puts("----------------------------------");
        }
        puts("Enter file name or Ctrl+Z to exit:");
        puts("----------------------------------");
    }

    return 0;
}
