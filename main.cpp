#include <iostream>
#include <pthread.h>
#include <stdlib.h>

#define  BOX_SIZE 6
#define  NUMBER_OF_PRODUCERS 4
#define  NUMBER_OF_CONSUMERS 3
#define  MAX_PRODUCT_NUM 1000000 
#define  MAX_PRODUCTS 120

  
using std::cout;
using std::endl;

pthread_mutex_t mutex_thread = PTHREAD_MUTEX_INITIALIZER;  

class ProductBox
{
	public:
		ProductBox(){
			 numOfItems = 0;
		}
		static void* Produce (void *args)
		{
		   int i = 0;
   			ProductBox& pBox = *(ProductBox*)args;
			while ( i < MAX_PRODUCTS / NUMBER_OF_PRODUCERS) {
				pthread_mutex_lock(&mutex_thread);
				if (pBox.numOfItems < BOX_SIZE){
					
					pBox.productsBox[pBox.numOfItems] = rand() % MAX_PRODUCT_NUM;
					cout << "Thread " << pthread_self() << " Producing " << pBox.productsBox[(pBox.numOfItems)++] << endl;
					++i;
					
				}
				pthread_mutex_unlock(&mutex_thread);
				
			}
			pthread_exit(NULL);
		}
		static void* Consume (void *args)
		{
			int i = 0;
			ProductBox& pBox = *(ProductBox*)args;
			while ( i < MAX_PRODUCTS / NUMBER_OF_CONSUMERS ) {
				pthread_mutex_lock(&mutex_thread);
				if (pBox.numOfItems > 0 ){
					
					cout << "Thread " << pthread_self() << " Consuming " << pBox.productsBox[--(pBox.numOfItems)] << endl;
					pBox.productsBox[pBox.numOfItems] = -1;
					++i;
					
				}
				pthread_mutex_unlock(&mutex_thread);

		   	}
			pthread_exit(NULL);
		}
	private:
		int numOfItems;
		int productsBox[BOX_SIZE];	
};


int main(){

	pthread_t producer_id[NUMBER_OF_PRODUCERS];
    pthread_t consumer_id[NUMBER_OF_CONSUMERS];
    int index;
	ProductBox* pBox = new ProductBox();
    // produceR
    for (index = 0; index < NUMBER_OF_PRODUCERS; ++index) {
    	
    	int rc = pthread_create(&producer_id[index], NULL, ProductBox::Produce, pBox);
        if (rc) {
             cout << "Error:unable to create thread,producer" << rc << endl;
             exit(-1);
        }
    }
    
    // consumeR
    for (index = 0; index < NUMBER_OF_CONSUMERS; ++index) {
		int rc = pthread_create(&consumer_id[index], NULL, ProductBox::Consume, pBox);
        if (rc) {
             cout << "Error:unable to create thread,consumer" << rc << endl;
             exit(-1);
        }
    }
 	for (index = 0; index < NUMBER_OF_PRODUCERS; ++index)
		pthread_join(producer_id[index], NULL);
 	for (index = 0; index < NUMBER_OF_CONSUMERS ; ++index)
        pthread_join(consumer_id[index], NULL);
 	
	delete pBox;
	return 0;

}










