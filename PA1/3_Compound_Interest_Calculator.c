#include <stdio.h>

double calcComp(double air, int du){
double a = 1;
for (int i = 0;i<du;i++){
  a*=(1+air/100);
}
return a;
}

int main() {
	int pc, du;
	double air;
	printf("Enter Princial (won) : ");
	scanf("%d", &pc);
	printf("Enter Annual interest rate (%%) : ");
	scanf("%lf", &air);
	printf("Enter Duration (in year) : ");
	scanf("%d", &du);
		
	printf("Your total amount of interest is %d won.\n", (int)(calcComp(air, du)*pc - pc));
	return 0;
}
