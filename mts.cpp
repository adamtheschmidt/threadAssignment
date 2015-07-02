#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <pthread.h>
#include <ctime>
#include <unistd.h>



using namespace std;

struct thread_data{
    int id;
    string input;
    int priority;
//0 low 1 high
    int direction;
//0 east 1 west
    int timeToLoad;
    int timeToCross;
    int loaded;
//0 false 1 true

};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t track = PTHREAD_MUTEX_INITIALIZER;

time_t timr;
clock_t startTime;
clock_t currTime;
int lastDirection = 0;


void *timeFunction( void* thingy){

    while(1){
        currTime = clock();
    }

}

struct thread_data td[101]; // global access to the thread data

void *threadFunction( void * inStruct){

    struct thread_data *thisThread;
    thisThread = (struct thread_data *)inStruct;
    
    
   //time moves faster, but the count is faster too - this is for runtime efficiency 
    usleep(100000 * (thisThread ->timeToLoad));

    float seconds = 1 * float(currTime - startTime)/CLOCKS_PER_SEC;
    
    string dir;
    if(thisThread -> direction == 0){
            dir = "East";
    }else{
            dir = "West";
    }
    

    if(seconds < 0.01){
        seconds = 0.00;
    }
    //cout.precision(3);

    pthread_mutex_lock( &mutex );
    if (seconds < 10.00){
        cout << "00:00:0";
        printf("%.1f",seconds);
        cout << " Train " << thisThread -> id << " is ready to go " << dir << endl;
    }else{ 
        //cout.precision(4);
        cout << "00:00:";
        printf("%.1f",seconds);
        cout << " Train " << thisThread -> id << " is ready to go " << dir << endl;
    }
 

    pthread_mutex_unlock( &mutex );

    thisThread -> loaded = 1;
    
    //evaluate who the next train is
start:
    for(int i = 0; i<100; i++){
         if(td[i].loaded == 1){
          if(td[i].priority >= (thisThread -> priority)){
           if(td[i].timeToLoad >= (thisThread -> timeToLoad)){
            if(td[i].timeToLoad == (thisThread -> timeToLoad)){
             if(td[i].id == (thisThread -> id)){
               goto lable;

             }
             else if((thisThread -> direction) != lastDirection){
                goto lable;
             }
            }
           }
          }
         }
     }
          goto start;
         
         
     

    //if it's you procede to the track
    
lable: 
    pthread_mutex_lock( &track );
    seconds = 1 * float(currTime - startTime)/CLOCKS_PER_SEC;
    lastDirection = thisThread -> direction;
    thisThread -> loaded = 0;

    pthread_mutex_lock( &mutex );
    if (seconds < 10.00){
        cout << "00:00:0";
        printf("%.1f",seconds);
        cout << " Train " << thisThread -> id << " is ON the main track going " << dir << endl;
    }else{ 
        //cout.precision(4);
        cout << "00:00:";
        printf("%.1f",seconds);
        cout << " Train " << thisThread -> id << " is ON the main track going " << dir << endl;
    }
 

    pthread_mutex_unlock( &mutex );

    usleep(100000 * (thisThread ->timeToCross));

    seconds = 1 * float(currTime - startTime)/CLOCKS_PER_SEC;
    

    pthread_mutex_lock( &mutex );
    if (seconds < 10.00){
        cout << "00:00:0";
        printf("%.1f",seconds);
        cout << " Train " << thisThread -> id << " is OFF the main track after going " << dir << endl;
    }else{ 
        //cout.precision(4);
        cout << "00:00:";
        printf("%.1f",seconds);
        cout << " Train " << thisThread -> id << " is OFF the main track after going " << dir << endl;
    }
 

    pthread_mutex_unlock( &mutex );

    pthread_mutex_unlock( &track );

    


}



int main(int argc, char* argv[]){
    
    //there will be 2 arguments. filename and number of inputs

    ifstream is(argv[1]);
    int cap = atoi(argv[2]);

    pthread_t thread_id[cap];
    pthread_t timeThread;
    string line;
    char foo;
    int fooInt;
    string fooString;
   
    startTime = clock();
    pthread_create(&timeThread,NULL,timeFunction,NULL);

    for (int i = 0; i<cap; i++){
        
        getline(is,line);
        td[i].input = line;
        td[i].id = i;
        td[i].loaded = 0;
        foo = line[0];

        //create direction and priority

        switch(foo){
            case 'E' :
                td[i].priority = 1;
                td[i].direction = 0;
                break;
            case 'W' :
                td[i].priority = 1;
                td[i].direction = 1;
                break;
            case 'e' :
                td[i].priority = 0;
                td[i].direction = 0;
                break;
            case 'w' :
                td[i].priority = 0;
                td[i].direction = 1;
                break;
        }//endSwitch
        
        //figure out timings
        
        

        if(line[3] == ','){

            fooInt = line[2] - '0';
            td[i].timeToLoad = fooInt;
            
            fooString = line.substr(4);
            td[i].timeToCross = atoi( fooString.c_str() );
        }
        else{

            fooString = line.substr(2,2);
            td[i].timeToLoad = atoi( fooString.c_str() );

            fooString = line.substr(5);
            td[i].timeToCross = atoi( fooString.c_str() );
        }




        pthread_create(&thread_id[i], NULL, threadFunction, (void*)&td[i]);
    }

    for (int j = 0; j<cap; j++){
        
        pthread_join(thread_id[j], NULL);
    }

    is.close();

    return 0;
}
