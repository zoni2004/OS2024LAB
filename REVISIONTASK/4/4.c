#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_CUSTOMERS 5
#define NUM_CHEFS 2
#define MAX_ORDERS 10
#define TERMINATE_SIGNAL -1

typedef struct {
    int order_id;
    char* dish;
    int prep_time;
} Order;

Order order_queue[MAX_ORDERS];
int order_count = 0;
int next_order_id = 1;
int customers_done = 0;

pthread_mutex_t order_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t chef_semaphore;
sem_t orders_available;

char* menu[] = {"Pizza", "Burger", "Pasta", "Salad", "Steak"};
int prep_times[] = {3, 2, 4, 1, 5};

void* customer(void* arg) {
    int customer_id = *(int*)arg;
    int dish_index = rand() % 5;
    
    pthread_mutex_lock(&order_mutex);
    
    Order new_order;
    new_order.order_id = next_order_id++;
    new_order.dish = menu[dish_index];
    new_order.prep_time = prep_times[dish_index];
    
    order_queue[order_count++] = new_order;
    
    printf("Customer %d placed order #%d: %s (prep time: %ds)\n",
           customer_id, new_order.order_id, new_order.dish, new_order.prep_time);
    
    pthread_mutex_unlock(&order_mutex);
    sem_post(&orders_available);
    
    return NULL;
}

void* chef(void* arg) {
    int chef_id = *(int*)arg;
    
    while (1) {
        sem_wait(&orders_available);
        
        pthread_mutex_lock(&order_mutex);
        
        if (order_count == 0 && customers_done == NUM_CUSTOMERS) {
            pthread_mutex_unlock(&order_mutex);
            break;
        }
        
        if (order_count == 0) {
            pthread_mutex_unlock(&order_mutex);
            continue;
        }
        
        Order current = order_queue[0];
        for (int i = 0; i < order_count - 1; i++) {
            order_queue[i] = order_queue[i + 1];
        }
        order_count--;
        
        printf("Chef %d taking order #%d: %s\n", chef_id, current.order_id, current.dish);
        pthread_mutex_unlock(&order_mutex);
        
        printf("Chef %d preparing %s (will take %d seconds)...\n",
               chef_id, current.dish, current.prep_time);
        sleep(current.prep_time);
        
        printf("Chef %d completed order #%d: %s\n",
               chef_id, current.order_id, current.dish);
    }
    
    printf("Chef %d signing off for the day\n", chef_id);
    return NULL;
}

int main() {
    pthread_t customers[NUM_CUSTOMERS];
    pthread_t chefs[NUM_CHEFS];
    int customer_ids[NUM_CUSTOMERS];
    int chef_ids[NUM_CHEFS];
    
    srand(time(NULL));
    
    sem_init(&chef_semaphore, 0, NUM_CHEFS);
    sem_init(&orders_available, 0, 0);
    
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_ids[i] = i + 1;
        pthread_create(&customers[i], NULL, customer, &customer_ids[i]);
        sleep(rand() % 2);
    }
    
    for (int i = 0; i < NUM_CHEFS; i++) {
        chef_ids[i] = i + 1;
        pthread_create(&chefs[i], NULL, chef, &chef_ids[i]);
    }
    
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }
    
    pthread_mutex_lock(&order_mutex);
    customers_done = NUM_CUSTOMERS;
    pthread_mutex_unlock(&order_mutex);
    
    for (int i = 0; i < NUM_CHEFS; i++) {
        sem_post(&orders_available);
    }
    
    for (int i = 0; i < NUM_CHEFS; i++) {
        pthread_join(chefs[i], NULL);
    }
    
    sem_destroy(&chef_semaphore);
    sem_destroy(&orders_available);
    pthread_mutex_destroy(&order_mutex);
    
    printf("All orders processed. Kitchen closed.\n");
    return 0;
}
