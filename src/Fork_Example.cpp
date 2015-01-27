// Prime number programming assignment 1
// Program written by Ben Pope
// This program takes in 1 argument between 1 and 49 and finds the sum of the primes
// found. It will find the number of primes you designate. It uses a Sieve of Eratosthenes
// to find primes.
// Example of program usage:
//      a.out 2
// Output of example:
//      CHILD: The sum of the first 2 prime numbers is: 5
//      CHILD: My PID is 16056 and it took me 462ns
//      PARENT: Child PID is 16056 and the child finished in 462ns
//      PARENT: My PID is 16055 and I finished in 4811ns
//      PARENT: The first 2 prime numbers are...
//      2
//      3
//      End of list. Program terminating.
// 
#include <iostream>
#include <sys/time.h>
#include <sys/resource.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

using namespace std;
bool doIsPrime(int num);
//For finding cpu time
double nanosecs()
{
        struct rusage rusage;
        getrusage(RUSAGE_SELF,&rusage);
        return rusage.ru_utime.tv_sec * 1 + rusage.ru_utime.tv_usec / 1;
}





int main(int argc,char *argv[]){
//Checks for arguments
if(argc != 2)
{
	cout << "Usage: a.out <1-49>" << endl;
	exit(1);
}

//Initialize variables
int n = atoi(argv[1]),
    sum,
    link[2],
    nbytes,
    counter,
    i;
double runTime = 0,
       childTime = 0;
pid_t pid;
//Checks for valid arguments
if(n > 49 || n < 1)
{
        cout << "Usage: a.out <1-49>" << endl;
        exit(1);
}

//Create pipe
pipe(link);
//Fork Process
if((pid = fork()) == -1)
{
	perror("fork");
        exit(1);
}


if(pid) //Parent Process
{

//Setting up  Parent
close(link[1]);
int primeHolder[n];
i = 0;
counter = 0;

//Find the prime numbers and store them in array primeHolder
while (counter < n)
{
 i++;
 if (doIsPrime(i))
  { 
    primeHolder[counter] = i;
    counter++;
  }
}
//Write array to child
write(link[0],primeHolder,sizeof(primeHolder));
//Read from child. This causes a wait for the parent process
nbytes = read(link[0],&childTime,sizeof(sum)); 
//Print information
cout << "PARENT: Child PID is " << pid << " and the child finished in " << childTime << "ns" << endl;
cout << "PARENT: My PID is " << getpid() << " and I finished in " << nanosecs() << "ns" << endl;
//Print n primes
cout << "PARENT: The first " << n << " prime numbers are..." << endl;
counter = 0;
while (counter < n)
{
 cout << primeHolder[counter] << endl;
 counter ++;
}
cout << "End of list. Program terminating." << endl;
}

else //Child Process
{
//Setting up Child
int readBuffer[n];
i = 0;
int childSum = 0;
close(link[0]);

//Read from Parent
nbytes = read(link[1], readBuffer, sizeof(readBuffer));
//Find sum
while(i < n)
{
childSum = childSum + readBuffer[i];
i++;
}
//Print information
cout << "CHILD: The sum of the first " << n << " prime numbers is: " << childSum << endl;
runTime = nanosecs();
cout << "CHILD: My PID is " << getpid() << " and it took me " << runTime << "ns" << endl;
//As soon as the child writes to the parent, it terminates.
write(link[1],&runTime,sizeof(childSum));
return(0);

}


                        
//end of main
return 0;
}
//To find out primes. Uses Sieve of Eratosthenes to find primes. Is not dynamic and 
//the accuracy goes down the higher the number. It is accurate for the 
//range of this program.
bool doIsPrime(int num){
int a = 0;
if (num == 1)
return 0;
if (num == 2 || num == 3 || num == 5 || num == 7 || num == 11 || num == 13)
return 1;
else{
a = num % 2;
if (a)
{
  a = num % 3;
  if (a) 
	{ 
          a = num % 5;
          if (a)
		{
                  a = num % 7;
                  if(a)
			{
			  a = num % 11;
                          if (a)
				{
				  a = num % 13;                 
			          if (a)  
				   return 1;
                                  else 
                                   return 0;
				}
			  else
			   return 0;
                        }
                  else
                   return 0;
             	}
          else
           return 0;
         }
   else
     return 0;
}
else
     return 0;
}
}
