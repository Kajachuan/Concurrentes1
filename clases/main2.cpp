#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <errno.h>
#include <string.h>


const int HIJOS = 1;

int calcularRandom(int i) {
	int resultado = 0;
	for (int j = 0; j < i; j++) {
		srand((int) time(NULL));
		resultado = (unsigned int) (rand() % 5 + 1);
	}
	return resultado;
}

void randSleep(int i) {
	int wait_time = calcularRandom(getpid());
	key_t key = ftok("test.cpp", i);
	if (key < 0) {
		std::cerr << "CHILD TOK ERROR: " << strerror(errno) << std::endl;
		exit(-1);
	}
	int shmid = shmget(key, sizeof(int), 0644|IPC_CREAT|IPC_EXCL);
	if (shmid < 0) {
		std::cerr << "CHILD GET ERROR " << strerror(errno) << std::endl;
		exit(-1);
	}
	int* point = (int*) shmat(shmid, NULL, 0);
	if (*point == -1) {
		std::cerr << "CHILD AT ERROR " << strerror(errno) << std::endl;
		exit(-1);
	}
	*point = wait_time;
	shmdt(point);
	shmctl(shmid, IPC_RMID, NULL);
	std::cout << "Inicia el hijo " << getpid() << " y va a dormir por " << wait_time << " segundos " << std::endl;
	sleep(wait_time);
	exit(i);
}

int main() {
	for (int i = 0; i < HIJOS; i++) {
		int pid = fork();
		if (pid == 0) {
			randSleep(i + 1);
		}
	}
	for (int i = 0; i < HIJOS; i++) {
		int status;
		pid_t ended_pid = wait(&status);
		key_t key = ftok("test.cpp", WEXITSTATUS(status));
		if (key < 0) {
			std::cerr << "TOK ERROR " << strerror(errno) << std::endl;
			exit(-1);
		}
		int shmid = shmget(key, sizeof(int), 0644|IPC_CREAT|IPC_EXCL);
		if (shmid < 0) {
			std::cerr << "GET ERROR " << strerror(errno) << std::endl;
			exit(-1);
		}
		int* point = (int*) shmat(shmid, NULL, 0);
		if (*point == -1) {
			std::cerr << "AT ERROR " << strerror(errno) << std::endl;
			exit(-1);
		}
		std::cout << "Finalizo el proceso hijo " << ended_pid << " luego de dormir " << *point << " segundos" << std::endl;
		shmdt(point);
		shmctl(shmid, IPC_RMID, NULL);
	}
}
