#include "../ecu.h"

void speedometer(){
	int shmid = shmget(1000,sizeof(int),0666|IPC_CREAT);
    int *speed = shmat(shmid,NULL,0);
	
	int shmid			= shmget(3000,sizeof(long),0666|IPC_CREAT);
    long *spd_pulses	= shmat(shmid,NULL,0);
	
	while(1){
		*spd_pulses	+= irandom(10);
		usleep(3000);
	}
}