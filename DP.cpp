#include <iostream>
#include <random>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

// 3 states of Philosophers
#define EATING 0
#define HUNGRY 1
#define THINKING 2

#define PHILOSOPHERS_COUNT 5

int state[PHILOSOPHERS_COUNT];

// semaphores
sem_t mutex;
sem_t S_P[PHILOSOPHERS_COUNT];

// To check whether forks are free to use for a given Philosopher and assigning state EATING id forks are free
void check(int phil_id){
  int left = (PHILOSOPHERS_COUNT + phil_id - 1)%PHILOSOPHERS_COUNT;
  int right = (phil_id + 1)%PHILOSOPHERS_COUNT;
  if (state[phil_id] == HUNGRY && state[left] != EATING && state[right] != EATING){
      state[phil_id] = EATING;
      sleep(rand()%4);
      cout<<"Philosopher "<<(phil_id+1)<<" is Eating with forks "<<phil_id+1<<" & "<<right+1<<endl;
      sem_post(&S_P[phil_id]);
  }
}

// for picking forks
void pickup_stick(int phil_id){
    sem_wait(&mutex);
    state[phil_id] = HUNGRY;
    cout <<"Philosopher "<<(phil_id+1)<<":Hungry"<< endl;

    check(phil_id);
    sem_post(&mutex);
    sem_wait(&S_P[phil_id]);
    sleep(rand()%4);
}

// for putting down forks
void put_down_stick(int phil_id){
    sem_wait(&mutex);
    state[phil_id] = THINKING;
    int left = (PHILOSOPHERS_COUNT + phil_id - 1)%PHILOSOPHERS_COUNT;
    int right = (phil_id + 1)%PHILOSOPHERS_COUNT;
    cout <<"Philosopher "<<(phil_id+1)<<" puts down fork "<<phil_id+1<<" & "<<right+1<<endl;
    cout <<"Philosopher "<<(phil_id+1)<< ": thinking"<<endl;

    check(left);
    check(right);
    sem_post(&mutex);
}

void* philospher_init(void* arg){
    while (1) {
        int phil_id = *((int*)arg);
        sleep(rand()%5);
        pickup_stick(phil_id);
        put_down_stick(phil_id);
    }
}

int main(){
    pthread_t philospher_thread[PHILOSOPHERS_COUNT];
    sem_init(&mutex, 0, 1);

    for(int i=0; i<PHILOSOPHERS_COUNT; i++) {
      sem_init(&S_P[i], 0, 0);
    }

    for (int i=0; i<PHILOSOPHERS_COUNT; i++) {
      // initializaing all philosophers with thinking state
      int p_i = i;
      pthread_create(&philospher_thread[i], NULL, philospher_init, &p_i);
      cout<<"Philosopher "<<p_i+1<<": thinking\n";
    }
    pthread_exit(NULL);
}
