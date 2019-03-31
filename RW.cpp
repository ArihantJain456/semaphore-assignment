#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fstream>
#include<random>

using namespace std;

sem_t s_mutex;
sem_t s_count;

string file_line;
fstream file;

int r_c;

void *reader(void *arg){
	sem_wait(&s_count);

	if(r_c == 0)
		sem_wait(&s_mutex);
	r_c++;

	sem_post(&s_count);

	file.open("file.txt", ios::in);
	bool file_success = file.is_open();

	if(file_success){
		cout<<"Reading File"<<endl;
		while(getline(file, file_line))
    	{
      		cout<<file_line<<endl;
   		}
    	file.close();
		cout <<"Read Complete"<< endl;
	}else{
		cout <<"File Error"<<endl;
	}

	sem_wait(&s_count);
	r_c--;
	if (r_c == 0)
    	sem_post(&s_mutex);

	sem_post(&s_count);
	sleep(rand()%2);
}

void *writer(void *arg){
	sem_wait(&s_mutex);

	file.open("file.txt", ios::app);
	int data = *((int*)arg);
	cout<<"Writing in File"<<endl;
	file<<"Data written by writer"<<data<<endl;
	file.close();
	cout <<"Writing Complete"<<endl;
	sem_post(&s_mutex);
	sleep(rand()%2);
}

int main(){
	pthread_t reader_thread[10];
  pthread_t writer_thread[10];

	sem_init(&s_mutex, 0, 1);
	sem_init(&s_count, 0, 1);

	for(int i=0; i<10; i++) {
		pthread_create(&writer_thread[i], NULL, writer, &i);
		pthread_create(&reader_thread[i], NULL, reader, NULL);
	}

	pthread_exit(NULL);
}
