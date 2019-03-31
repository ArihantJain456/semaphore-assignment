#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fstream>

using namespace std;

sem_t s_mutex;
sem_t s_count;
sem_t s_queue;
string file_line;
fstream file;

int r_c;

void *reader(void *arg){
	sem_wait(&s_queue);
	sem_wait(&s_count);

	if(r_c == 0)
		sem_wait(&s_mutex);
	r_c++;

	sem_post(&s_queue);
	sem_post(&s_count);

	file.open("file.txt", ios::in);
	bool file_success = file.is_open();

	if(file_success){
		cout << "Reading started" << endl;
		while(getline(file, file_line))
    	{
      		cout << file_line << endl;
   		}
    	file.close();
		cout << "Reading Complete" << endl;
	}else{
		cout << "File Error" << endl;
	}

	sem_wait(&s_count);
	r_c--;
	if (r_c == 0)
    	sem_post(&s_mutex);
	sem_post(&s_count);
}

void *writer(void *arg){
	sem_wait(&s_queue);
	sem_wait(&s_mutex);
	sem_post(&s_queue);

	file.open("file.txt", ios::app);
	int data = ((intptr_t)arg);
	cout << "Writing started" << endl;
	file << data << endl;
	file.close();
	cout << "Writing Complete" << endl;
	sem_post(&s_mutex);
}

int main(){
	pthread_t reader_thread[10];
       	pthread_t writer_thread[10];

	sem_init(&s_mutex, 0, 1);
	sem_init(&s_count, 0, 1);
	sem_init(&s_queue, 0, 1);

	for(int i=0; i<3; i++)
		pthread_create(&reader_thread[i], NULL, reader, NULL);

	for(int i=0; i<3; i++)
		pthread_create(&writer_thread[i], NULL, writer, (void *)(intptr_t)i);

	for(int i=3; i<6; i++){
		pthread_create(&reader_thread[i], NULL, reader, NULL);
		pthread_create(&writer_thread[i], NULL, writer, (void *)(intptr_t)i);
	}

	for(int i=6; i<8; i++)
		pthread_create(&writer_thread[i], NULL, writer, (void *)(intptr_t)i);

	for(int i=6; i<8; i++)
		pthread_create(&reader_thread[i], NULL, reader, NULL);

	for(int i=8; i<10; i++){
		pthread_create(&writer_thread[i], NULL, writer, (void *)(intptr_t)i);
		pthread_create(&reader_thread[i], NULL, reader, NULL);
	}
	pthread_exit(NULL);
}
