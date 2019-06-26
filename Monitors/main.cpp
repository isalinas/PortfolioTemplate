#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <unistd.h>

//Condition Structure
struct cond{
    sem_t x_sem;
    int x_count;
};

//Condition Variables
cond has_empty;
cond has_full;

//SEMAPHORE
sem_t mutex;
sem_t next;
int next_count;

//BUFFER
std::vector<char> theBuffer;
//COMMANDS
int bufferSize = 0;
int producerSize = 0;
int consumerSize = 0;
int produceItems = 0;
//COUNTERS
int counter = 0;
int itemProduce = 0;
bool flag = false;
//Cond Wait
void wait(cond &cv){

    cv.x_count++;
    if(next_count > 0){
        sem_post(&next);
    }else{
        sem_post(&mutex);
    }
    sem_wait(&cv.x_sem);
    cv.x_count--;
}

//Cond Signal
void signal(cond &cv){

    if(cv.x_count > 0){
        next_count++;
        sem_post(&cv.x_sem);
        sem_wait(&next);
        next_count--;
    }

}


//MONITOR INSERT
void mon_insert(char alpha){
    
    sem_wait(&mutex);
    //Begin Monitor
    if(itemProduce < produceItems){     
    long tid;
    tid = (long)pthread_self();

    while(counter == bufferSize)
        wait(has_empty); 

    if(itemProduce == produceItems){
        exit(0);
    }
    
    theBuffer[counter] = alpha;
    printf("p:<%u>, item: %c, at %d \n" ,tid, alpha, counter);
    itemProduce++;
    counter++;
    signal(has_full);
    
    }else{
            flag = true;
            if(has_full.x_count > 0){
                signal(has_full);
            }   
            if(next_count > 0){      
                sem_post(&next);
            }else{
                sem_post(&mutex);
            }
            pthread_exit(NULL);
    }
    //END MONITOR

    if(next_count > 0){
        sem_post(&next);
    }
    else{
        sem_post(&mutex);
    }

}

//MONITOR REMOVE
char mon_remove(){
    char result;
    long tid = (long)pthread_self();
    
    //BEGIN MONITOR
    sem_wait(&mutex); 

    while(counter == 0){
        
        if(flag == true){    
            if(next_count > 0){      
                sem_post(&next);
            }else{
                sem_post(&mutex);
            }
           pthread_exit(NULL);
        }
        else{
           wait(has_full);
           if(flag == true){
                exit(0);
           }
        }
    }
    counter--;
    result = theBuffer[counter];
    theBuffer[counter] = ' ';
    printf("c:<%u>, item: %c, at %d \n" ,tid, result, counter);
    signal(has_empty);

    
    //END MONITOR    
    if(next_count > 0){
        sem_post(&next);
    }
    else{
        sem_post(&mutex);
    }
    return result;
}

char randomLetter(){
    char letter;
    int num = rand() % 2;
    int r;
    if(num == 0){
        //Lowercase
        r = rand() % 26;
        letter = 'a' + r;
    }
    else{
        //Uppercase
        r = rand() % 26;
        letter = 'A' + r;
    }
    return letter;
}

void popStruct(cond &data){
    sem_init(&data.x_sem,0,0);
    data.x_count = 0;
}

void *producer(void *threadid){
    char alpha;
    while(1){
        alpha = randomLetter();
        mon_insert(alpha);
    }
}

void *consumer(void *threadid){
    char result;
    while(1){
        result = mon_remove();
    }
}

int main(int argc, char* argv[]) {
    //CHECK FOR PROPER ARGUMENTS
    if(argc < 8){
        printf("Needs 8 arguments\n");
        return 0;
    }
    //PARSE COMMAND LINE
    for(int i = 1; i < argc; i++){
        std::string arg = argv[i];
        if(arg == "-b"){
            bufferSize = atoi(argv[i+1]);
        }
        else if(arg == "-p"){   
            producerSize = atoi(argv[i+1]);
        }
        else if(arg == "-c"){
            consumerSize = atoi(argv[i+1]);
        }
        else if(arg == "-i"){
            produceItems = atoi(argv[i+1]);
        }
    }
    //Populate Struct
    popStruct(has_full);
    popStruct(has_empty);

    sem_init(&mutex,0,1);
    sem_init(&next, 0,0);
    next_count = 0;
    //BUFFER SIZE
    theBuffer.resize(bufferSize);
    //HOW MANY THREADS FOR CON/PRO
    pthread_t cons[consumerSize];
    pthread_t prod[producerSize];



    //CREATE PRO
    for(int i = 0; i < producerSize; i++){
       int num = pthread_create(&prod[i], NULL, producer, (void*)i);  
    }
    //CREATE CON
    for(int i = 0; i < consumerSize; i++){
       int num = pthread_create(&cons[i], NULL, consumer, (void*)i);  
    }

    //JOIN
    for(int i = 0; i < producerSize; i++){
       int num = pthread_join(prod[i], NULL);  
    }
    //JOIN  
    for(int i = 0; i < consumerSize; i++){
       int num = pthread_join(cons[i], NULL);  
    }

    return 0;
}
