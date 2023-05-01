#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CUSTOMERS 10
#define MAX_FLAVORS 3

int n_customers;
int n_flavors;
int in_stock[MAX_FLAVORS];
int requested[MAX_CUSTOMERS][MAX_FLAVORS];
sem_t stock_mutex[MAX_FLAVORS];
sem_t request_mutex;
sem_t ice_cream_ready[MAX_CUSTOMERS];

void* customer(void* arg) {
    int customer_id = *(int*)arg;
    int i, j;
    int ordered_flavors[MAX_FLAVORS];
    
    // Pick random ice cream flavors
    for (i = 0; i < n_flavors; i++) {
        ordered_flavors[i] = rand() % 2;
    }
    
    // Request ice cream
    sem_wait(&request_mutex);
    for (i = 0; i < n_flavors; i++) {
        requested[customer_id][i] = ordered_flavors[i];
    }
    sem_post(&request_mutex);
    
    // Wait for ice cream to be ready
    sem_wait(&ice_cream_ready[customer_id]);
    
    // Enjoy ice cream
    printf("Customer %d is enjoying the ice cream...\n", customer_id);
    sleep(2);
    printf("Customer %d finished the ice cream.\n", customer_id);
    
    pthread_exit(NULL);
}

void* manufacturer(void* arg) {
    int flavor_id = *(int*)arg;
    int i;
    
    while (1) {
        // Produce ice cream
        sleep(1);
        in_stock[flavor_id]++;
        printf("Manufacturer produced one more %d flavor ice cream. Total: %d\n", flavor_id, in_stock[flavor_id]);
        
        // Notify waiting customers
        sem_post(&stock_mutex[flavor_id]);
    }
}

void* server(void* arg) {
    int i, j;
    
    while (1) {
        // Check if any customer requested ice cream
        sem_wait(&request_mutex);
        for (i = 0; i < n_customers; i++) {
            int can_serve = 1;
            for (j = 0; j < n_flavors; j++) {
                if (requested[i][j] == 1 && in_stock[j] == 0) {
                    can_serve = 0;
                    break;
                }
            }
            if (can_serve) {
                for (j = 0; j < n_flavors; j++) {
                    if (requested[i][j] == 1) {
                        in_stock[j]--;
                        printf("Server served customer %d with %d flavor ice cream. Remaining stock: %d\n", i, j, in_stock[j]);
                    }
                }
                sem_post(&ice_cream_ready[i]);
            }
        }
        sem_post(&request_mutex);
        
        // Check for deadlock
        int all_waiting = 1;
        for (i = 0; i < n_flavors; i++) {
            int value;
            sem_getvalue(&stock_mutex[i], &value);
            if (value == 0) {
                all_waiting = 0;
                break;
            }
        }
        if (all_waiting) {
            printf("Deadlock detected! Exiting program.\n");
            exit(1);
        }
        
        // Wait for a while before checking again
        sleep(2);
    }
}

int main() {
	int i, j;
    
	// Initialize variables and semaphore
	n_customers = 5;
	n_flavors = 3;

	for (i = 0; i < n_flavors; i++) {
		in_stock[i] = 5;
    		sem_init(&stock_mutex[i], 0, 0);
	}
	sem_init(&request_mutex, 0, 1);

	pthread_t customers[n_customers];
	pthread_t manufacturers[n_flavors];
	pthread_t server_thread;

	// Create customer threads
	int customer_ids[n_customers];
	for (i = 0; i < n_customers; i++) {
    		customer_ids[i] = i;
    		sem_init(&ice_cream_ready[i], 0, 0);
    		pthread_create(&customers[i], NULL, customer, &customer_ids[i]);
	}

	// Create manufacturer threads
	int flavor_ids[n_flavors];
	for (i = 0; i < n_flavors; i++) {
    		flavor_ids[i] = i;
    		pthread_create(&manufacturers[i], NULL, manufacturer, &flavor_ids[i]);
	}

	// Create server thread
	pthread_create(&server_thread, NULL, server, NULL);

	// Wait for all threads to finish
	for (i = 0; i < n_customers; i++) {
    		pthread_join(customers[i], NULL);
	}
	for (i = 0; i < n_flavors; i++) {
    		pthread_cancel(manufacturers[i]);
	}
	pthread_cancel(server_thread);

	// Destroy semaphores
	sem_destroy(&request_mutex);
	for (i = 0; i < n_flavors; i++) {
    		sem_destroy(&stock_mutex[i]);
	}
	for (i = 0; i < n_customers; i++) {
    		sem_destroy(&ice_cream_ready[i]);
	}

	return 0;
}

