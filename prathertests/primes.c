#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/* 
   Brute force prime factorization algorithm. 
   Worst case: BigTheta(2^n) where n is the number of bits in integer being factored 
*/
int isPrime(unsigned long long int x)
{
    unsigned long long int f;
	for (f = 2; f < x-1; f++)
	{
    	if (x % f == 0)
      		return 0;
    }
  	return 1;
 }

int main(int argc, char *argv[])
{
	pid_t cpid, mypid;
	pid_t pid = getpid(); /* get current processes PID */
	printf("Parent pid: %d\n", pid);

	cpid = fork();
	if (cpid > 0) 
	{ /* Parent Process */
		mypid = getpid();
		printf("[%d] parent of [%d] returning to shell\n", mypid, cpid);
		exit(0); //kill the parent to return execution to the shell
	} 
	else if (cpid == 0) 
	{ /* Child Process */
		mypid = getpid();
		//printf("[%d] child\n", mypid);
		unsigned long long int i;
		for(i = 0; i < 18446744073709000000ULL; i++)
		{
			unsigned long long int p = isPrime(i);

			//For testing:
			//printf("[%d] child running prime %d = %d\n", mypid, i, p);
			//if(i == 20)
			//    break;
		}
	} 
	else 
		{ /* Error */
		perror("Fork failed");
		exit(1);
	}
	exit(0);
}
