#include<stdio.h>

void print_arr(int array[], int size){
	for (int i = 0; i<size; i++){
    printf("%d ",array[i]);
  }
  printf("\n");
}

void rule_1(int array[], int size){
	for (int i = 0 ; i < size; i++){
    array[i] = i+1;
  }
  print_arr(array, size);
}

void rule_2(int array[], int size){
  for (int i = 0 ; i < size; i++){
    array[size-i-1] = i+1;
  }
  print_arr(array, size);
}

void rule_3(int array[], int size){
  int j = 0;
  for (int i = 0 ; j < size; i+=2){
    ++j;
    if(i>size-1){
      i=1;
    }
    array[i] = j;
  }
  print_arr(array, size);
}

void rule_4(int array[], int size){
  int j = 0;
	for (int i = 0 ; i < size; i++){
    if(i%2==0){
      array[j]=i+1;
    } else {
      array[size-j-1]=i+1;
      j++;
    }
  }
  print_arr(array, size);
}

int main(){
	int n;
	int arr[20];
	scanf("%d",&n);
	rule_1(arr,n);
	rule_2(arr,n);
	rule_3(arr,n);
	rule_4(arr,n);
	return 0;
}