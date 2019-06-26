#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <unistd.h>

//SEMAPHORES
sem_t empty;
sem_t full;
sem_t mutex;
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


void *Producer(void *threadid){
        while(1){    
            long tid;
            tid = (long)pthread_self();
            sem_wait(&empty);
            sem_wait(&mutex);
            
            //Critical Section
             if(itemProduce < produceItems){
                printf("p:<%u>, item: X, at %d \n" ,tid, counter); 
                theBuffer[counter] = 'X';
                itemProduce++;
                counter++;         
             }else{
                sem_post(&mutex);
                sem_post(&full);
                pthread_exit(NULL);
             }

            sem_post(&mutex);
            sem_post(&full);
        }
}


void *Consumer(void* threadid){
    while(1){
        long tid;
        tid = pthread_self();
        sem_wait(&full);
        sem_wait(&mutex);
        //Critical Section
        
        if(counter > 0){
        counter--;
        printf("c:<%u>, item: X, at %d \n" ,tid, counter); 
        theBuffer[counter] = ' ';
        }
        else if(counter == 0){
        counter--;
        sem_post(&mutex);
        sem_post(&empty);
        pthread_exit(NULL);
        }
        else
        {
            sem_post(&mutex);
            sem_post(&empty);
            pthread_exit(NULL);
        }
        sem_post(&mutex);
        sem_post(&empty);
    } 
}

int main(int argc, char* argv[]){
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

    //INITALIZE SEMAPHORES
    sem_init(&mutex, 0,1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, bufferSize);
    
    theBuffer.resize(bufferSize);

    //HOW MANY THREADS FOR CON/PRO
    pthread_t consumer[consumerSize];
    pthread_t producer[producerSize];

    

    //CREATE PRO
    for(int i = 0; i < producerSize; i++){
       int num = pthread_create(&producer[i], NULL, Producer, (void*)i);  
    }
    //CREATE CON
    for(int i = 0; i < consumerSize; i++){
       int num = pthread_create(&consumer[i], NULL, Consumer, (void*)i);  
    }

       //JOIN PRO
    for(int i = 0; i < producerSize; i++){
       int num = pthread_join(producer[i], NULL);  
    }

    	//JOIN COS
	for(int i = 0; i < consumerSize; i++){
       int num = pthread_join(consumer[i], NULL);  
    }

    
    return 0;
}
