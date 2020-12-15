#include <stdio.h>
int main() {
  int power(int x, int n){
    if (n ==1 ){
      return x;
    } else if(n == 0){
      return 1;
    }
    return (x*power(x,n-1));
  }
  int len(int number){
    int cnt = 0;
    while (number>0){
      cnt++;
      number /= 10;
    }
    return cnt;
  }

  int num = 0;
  int shift = 0;
  scanf("%d %d",&num, &shift);
  if (shift > 0){
    if (shift>len(num)){
      shift-=(shift/len(num))*len(num);
    }
  } else {
    if(shift*(-1)>len(num)){
      shift+=((shift*(-1)/len(num))+1)*len(num);
    } else {
      shift+=len(num);
    }
  }

  int func(int number){
    if (shift>0){
      num = number/10 + (number%10)*power(10,len(number)-1);
      shift--;
      func(num);
    } else {
      return 0;
    }
  }
  func(num);
  printf("%d", num);
	return 0;
}
