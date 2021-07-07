
// 201800384 전자공학과 강희 정상동작
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100001

// 공유 데이터
int buffer[BUFFER_SIZE] = {
	    0,
};
int in = 0, out = 0;
unsigned long long total = 0;
int count[100] = {
	    0,
};
int pro_fin = 0, con_fin = 0;
// 세마포 생성
sem_t mutex, empty, full;
sem_t mutex_c, mtx_pro, mtx_con;

void *producer(void *arg);
void *consumer(void *arg);
int main()
{
	    int state = 0, i;

		pthread_t p1, p2, c1, c2, c3; // 5개의 스레드 선언
		
		char P1[] = "P1";
		
		char P2[] = "P2";
		
		char C1[] = "C1";
		
		char C2[] = "C2";
		
		char C3[] = "C3";
		
		void *result_t = NULL;

		
		/* 세마포 초기화 */
		
		state = sem_init(&mutex, 0, 1);
		
		if (state != 0)
		
			perror("sem_init Error\n");
			
		state = sem_init(&empty, 0, BUFFER_SIZE);
		
		if (state != 0)
		
			perror("sem_init Error\n");
			
		state = sem_init(&full, 0, 0);
		
		if (state != 0)
		
			perror("sem_init Error\n");
			
		state = sem_init(&mutex_c, 0, 1);
		
		if (state != 0)
					
			perror("sem_init Error\n");
			
		state = sem_init(&mtx_pro, 0, 1);
		
		if (state != 0)
		
			perror("sem_init Error\n");
			
		state = sem_init(&mtx_con, 0, 1);
		
		if (state != 0)
		
			perror("sem_init Error\n");


		
		/* 스레드 생성 */
		
		pthread_create(&p1, NULL, producer, P1);
		
		pthread_create(&p2, NULL, producer, P2);
		
		pthread_create(&c1, NULL, consumer, C1);
		
		pthread_create(&c2, NULL, consumer, C2);
		
		pthread_create(&c3, NULL, consumer, C3);

		
		/* 스레드 종료되길 기다림 */
		
		pthread_join(p1, &result_t);
		
		pthread_join(p2, &result_t);
		
		pthread_join(c1, &result_t);
		
		pthread_join(c2, &result_t);
		
		pthread_join(c3, &result_t);

	
		/* total, count 값 */
		
		printf("\ntotal: %llu\n\n", total);
		
		for (i = 0; i < 100; i++)
		
		{
		
			printf("count[%d] : %d\n", i, count[i]);
			
		}

		
		/* 세마포 제거 */
										    
		sem_destroy(&mutex);
								
		sem_destroy(&empty);
		
		sem_destroy(&full);
		
		sem_destroy(&mutex_c);
		
		sem_destroy(&mtx_con);
		
		sem_destroy(&mtx_pro);
		
		return 0;
}
void *producer(void *arg)
{
	    int input;
		
		printf("%s starts!\n", (char *)arg);

		
		while (1)
		
		{
			if(pro_fin ==1) break;	// 종료
		
			scanf("%d", &input);

			
			sem_wait(&empty);
			
			sem_wait(&mutex);
			
			buffer[in] = input;
			
			in = (in + 1) % BUFFER_SIZE;
			
		//	printf("%s buffer : %d\n",(char*)arg,input);
			        
			sem_post(&mutex);
			
			sem_post(&full);
			        
			/* 종료되야 하는지 확인 */
			 
			sem_wait(&mtx_pro);
			
			if(input == -999) {
			
				pro_fin = 1;
				
				sem_post(&mtx_pro);
				
				break;
				
			}
			
			sem_post(&mtx_pro);
				
			/* 잠시 쉼 */
			
			if (!strcmp(((char *)arg), "P1"))
			
				usleep(1000);
				
			else
			
				usleep(2000);
				
		}
		
	//	printf("%s fin!\n", (char *)arg);
		
		pthread_exit(NULL);
}
void *consumer(void *arg)
{
	    printf("%s starts!\n", (char *)arg);
		
		int data, i;
		
		while (1)
		
		{
		
			if(con_fin == 1) break; // 종료

			/* 데이터 꺼내오기 */
			
			sem_wait(&full);
			
			sem_wait(&mutex);
			
			data = buffer[out];
			
			out = (out + 1) % BUFFER_SIZE;
			
		       
			/* 종료되어야 하는지 확인 */
			
			sem_wait(&mtx_con);
			
			if(data == -999){
			
				con_fin = 1;
				
				sem_post(&mtx_con);
				
				sem_post(&mutex);

				sem_post(&empty);

				break;
				
			}
			sem_post(&mtx_con);

			sem_post(&mutex);
			
			sem_post(&empty);

			
			/* 카운트 하기 */
			
			sem_wait(&mutex_c);
			
			total += data;
			
			for (i = 1; i <= 100; i++)
			
			{
			
				if(i <= 99){
					if (data < i * 10000 && data >= (i - 1) * 10000)
				
						count[i - 1] += 1;
				}
				else
					if (data >= 990000)
						count[99] += 1;
					
			}
			
			sem_post(&mutex_c);


			
			if (!strcmp(((char *)arg), "C1"))
			
				usleep(3000);
				
			else if (!strcmp(((char *)arg), "C2"))
			
				usleep(4000);
				
			else
			
				usleep(5000);
				
		}
		
	//	printf("%s fin!\n", (char *)arg);
		
		pthread_exit(NULL);
}


