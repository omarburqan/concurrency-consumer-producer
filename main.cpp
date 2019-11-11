#include <iostream>
#include <pthread.h>
#include <stdlib.h>


static const int BOX_SIZE = 6;
static const int NUMBER_OF_PRODUCERS = 4;
static const int NUMBER_OF_CONSUMERS = 3;
static const int productsBoxSize = 6;
static const int MAX_PRODUCT_NUM = 1000000; 
static const int MAX_PRODUCTS = 120;

int numOfItems = 0;

using std::cout;
using std::endl;

// The products should be placed in a box of size 6.
int productsBox[productsBoxSize] = {-1,-1,-1,-1,-1,-1};
pthread_mutex_t mutex_thread = PTHREAD_MUTEX_INITIALIZER;


void* producer(void *args) { 

    for (int i = 0; i < MAX_PRODUCTS / NUMBER_OF_PRODUCERS; i++) {
    	pthread_mutex_lock(&mutex_thread);
		if (numOfItems < BOX_SIZE){
			productsBox[numOfItems] = rand() % MAX_PRODUCT_NUM;
			cout << "Thread " << *(int*)args << " Producing " << productsBox[numOfItems] << endl;
			++numOfItems;
		}
		pthread_mutex_unlock(&mutex_thread);
	}
    return NULL;
}

void* consumer(void *args) {
	
    for (int i = 0; i < MAX_PRODUCTS / NUMBER_OF_CONSUMERS; i++) {
		pthread_mutex_lock(&mutex_thread);
		if (numOfItems > 0 ){
			cout << "Thread " << *(int*)args << " Consuming " << productsBox[numOfItems] << endl;
			productsBox[numOfItems] = -1;
			--numOfItems;
		}
		pthread_mutex_unlock(&mutex_thread);
   	}
    return NULL;
}

int main(){
	pthread_t producer_id[NUMBER_OF_PRODUCERS];
    pthread_t consumer_id[NUMBER_OF_CONSUMERS];
    int index;
    

    
    // produceR
    for (index = 0; index < NUMBER_OF_PRODUCERS; ++index) {
    	int rc = pthread_create(&producer_id[index], NULL, producer, &index);
        if (rc) {
             cout << "Error:unable to create thread,producer" << rc << endl;
             exit(-1);
        }
    }
    
    // consumeR
    for (index = 0; index < NUMBER_OF_CONSUMERS; ++index) {
		int rc = pthread_create(&consumer_id[index], NULL, consumer, &index);
        if (rc) {
             cout << "Error:unable to create thread,consumer" << rc << endl;
             exit(-1);
        }
    }
 	
 	for (index = 0; index < NUMBER_OF_CONSUMERS ; ++index)
        pthread_join(consumer_id[index], NULL);

    for (index = 0; index < NUMBER_OF_PRODUCERS; ++index)
		pthread_join(producer_id[index], NULL);
	
    pthread_mutex_destroy(&mutex_thread);
	return 0;

}









