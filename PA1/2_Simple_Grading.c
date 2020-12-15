#include <stdio.h>
float avg(int n1, int n2, int n3, int n4){
  return (n1+n2+n3+n4)/4.00;
}

char getGrade(int num) {
  if (num>=90){
    return 'A';
  } else if (num>=80){
    return 'B';
  } else if (num>=70){
    return 'C';
  } else if (num>=60){
    return 'D';
  } else {
    return 'F';
  }
}
int main()
{
	int n1, n2, n3, n4;
	printf("Enter 4 scores: ");
	scanf("%d %d %d %d", &n1, &n2, &n3, &n4);	
	printf("\nAverage : %.1f\n", avg(n1,n2,n3,n4));
	printf("1st student : (%d, %c)\n", n1, getGrade(n1));
	printf("2nd student : (%d, %c)\n", n2, getGrade(n2));
	printf("3rd student : (%d, %c)\n", n3, getGrade(n3));
	printf("4th student : (%d, %c)\n", n4, getGrade(n4));	
	return 0;
}