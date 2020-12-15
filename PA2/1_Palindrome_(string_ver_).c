#include <stdio.h>

int endIdx(char arr[]){
	int i;
	for(i = 0; i < 100; i++)
		if(arr[i] == 0)
			return (i-1);
}

int isPal(char arr[], int start, int end){
  while(start<end){
    if(arr[start]==arr[end]){
      start++;
      end--;
      isPal(arr,start,end);
    } else {
      return 0;
    }
  }
  return 1;

}

int main(){
	char str[100] = {0,};
	
	printf("Type a sentence: ");
	scanf("%s", str);
	
	printf("Your sentence ");
	if(isPal(str, 0, endIdx(str)))
		printf("is Palindrome!\n");
	else
		printf("is not Palindrome!\n");
	
	return 0;
}
