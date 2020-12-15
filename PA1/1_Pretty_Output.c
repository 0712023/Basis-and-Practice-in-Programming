#include <stdio.h>
int isPrime(int number)
{
int remainder = 1;
for(int i = 2; i<number; i++){
  remainder*=number%i;
}
return remainder;
}

void printDivisors(int number)
{
printf("%d : 1, ",number);
for(int i=2;i<number;i++){
  if(number%i==0){
    if(isPrime(i)){
      printf("\"%d\", ",i);
    } else {
      printf("%d, ", i);
    }
  }
}
printf("%d", number);
}

int main()
{
	int number = 0;
	scanf("%d", &number);	
	printDivisors(number);
	return 0;
}