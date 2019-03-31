#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <random>
#include <unistd.h>

using namespace std;

sem_t tobacco_and_paper;
sem_t matches_and_paper;
sem_t tobacco_and_matches;
sem_t done_smoking, lock;
int smoker_ids[] = {1, 2, 3};
string smoker_materials[] = {"Matches", "Paper", "Tobacco"};

void *smoker(void *arg){
    int smoker_id = *(int *)(arg);
    while(true){
        switch (smoker_id) {
          case 1:
            sem_wait(&tobacco_and_paper);
            sem_wait(&lock);
            break;
          case 2:
            sem_wait(&tobacco_and_matches);
            sem_wait(&lock);
           break;
          case 3:
            sem_wait(&matches_and_paper);
            sem_wait(&lock);
           break;
        }

        cout<<"Smoker "<<smoker_id<<" takes "<<smoker_materials[smoker_id-1]<<endl;
        cout << "Smoker "<<smoker_id<<" smokes"<<endl;
        sem_post(&done_smoking);
        sem_post(&lock);
    }
}

void *agent(void *arg){
    while(true){
        sem_wait(&lock);
        int random_number = rand()%3 + 1;
        switch (random_number)
        {
            case 1:
                sem_post(&tobacco_and_matches);
                break;
            case 2:
                sem_post(&tobacco_and_paper);
                break;
            case 3:
                sem_post(&matches_and_paper);
                break;
            default:
                break;
        }
        sem_post(&lock);
        sem_wait(&done_smoking);
    }
}


int main(){

    pthread_t smoker_thread[3];
    pthread_t agent_thread;

    sem_init(&done_smoking, 0, 1);
    sem_init(&lock, 0, 1);
    sem_init(&tobacco_and_matches, 0, 0);
    sem_init(&matches_and_paper, 0, 0);
    sem_init(&tobacco_and_paper, 0, 0);

    pthread_create(&agent_thread, NULL, agent, NULL);
    pthread_create(&smoker_thread[0], NULL, smoker, &smoker_ids[0]);
    pthread_create(&smoker_thread[1], NULL, smoker, &smoker_ids[1]);
    pthread_create(&smoker_thread[0], NULL, smoker, &smoker_ids[2]);

    pthread_exit(NULL);
}
