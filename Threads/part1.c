#include "slow_functions.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	//instantiating a thread for slow_function2
    pthread_t slow;
	//instantiating an array of threads for slow_function1
    pthread_t threads[10];
   
	//checks if attributes are NULL while creating thread
    if(pthread_create(&slow,NULL,slow_function2,NULL))
    {
		perror("Error creating thread for slow function 2");
        return 1;
    }
	
	int i;
	
	//creates 10 threads for slow_function1 and checks if attributes are null and therefore can't create thread
    for(i = 0; i < 10; i++) {
        if(pthread_create(&threads[i], NULL, slow_function1, NULL))
        {
			perror("Error creating thread for slow function1");
            return 1;
        }
    }
    //checking if threads join
    if(pthread_join(slow, NULL))
    {
		perror("Error joining thread for SF2");
        return 2;
    }
	
	int j;
    for(j = 0; j < 10; j++)
    {
        if(pthread_join(threads[j], NULL))
        {
            perror("Error joining thread for SF1");
            return 2;
        }
        
    }
    return 0;

}
\
