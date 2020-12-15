#include <stdio.h>
#include <math.h>

float sum(float arr[], int size){
  float sum=0;
  for(int i = 0;i<size;i++){
    sum+=arr[i];
  }
  return sum;
}

float avg(float arr[], int size){
  float avg=0;
  for(int i = 0;i<size;i++){
    avg+=arr[i];
  }
  return avg/size;
}

float stdev(float arr[], int size){
  float stdev = 0;
  for(int i = 0;i<size;i++){
    stdev+=(arr[i]-avg(arr, size))*(arr[i]-avg(arr, size));
  }
  return sqrt(stdev/size);
}

int main(){
	int n;
  float arr[10];
	scanf("%d",&n);
	for(int i=0;i<n;i++)
		scanf("%f",&arr[i]);
  printf("sum : %.2f\n", sum(arr, n));
  printf("avg : %.2f\n", avg(arr, n));
  printf("stdev : %.2f", stdev(arr, n));
	return 0;
}