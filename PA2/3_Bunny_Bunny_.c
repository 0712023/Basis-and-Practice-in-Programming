#include <stdio.h>

const char BUNNY = '@';
const char EMPTY = '.'; 

const int FieldSize = 12;
int current_bunny_pos[2]; 
char Field[12][13] = {0, };

char get_command() {
	char cmd; 
	scanf(" %c", &cmd); 
	return cmd;
}
void up(){
  if(current_bunny_pos[0]!=0){
    current_bunny_pos[0]--;
  }
}

void down(){
  if(current_bunny_pos[0]!=11){
    current_bunny_pos[0]++;
  }
}

void right(){
  if(current_bunny_pos[1]!=11){
    current_bunny_pos[1]++;
  }
}

void left(){
  if(current_bunny_pos[1]!=0){
    current_bunny_pos[1]--;
  }
}

int func(char cmd){
  if(cmd == 'U'){
    up();
  } else if (cmd == 'D'){
    down();
  } else if (cmd == 'R'){
    right();
  } else if (cmd == 'L'){
    left();
  } 
}
int main() {
	int N; 
	char command; 
	scanf(" %d %d", &current_bunny_pos[0], &current_bunny_pos[1]); 
	scanf(" %d", &N);
	for (int i = 0; i < FieldSize; ++i) {
		for (int j = 0; j < FieldSize; ++j)
			Field[i][j] = '.'; 
	}
char cmd[100] = {};
for (int i = 0; i<N;i++){
  cmd[i] = get_command();
}

for (int i = 0; i<N; i++){
  func(cmd[i]);
}

	Field[current_bunny_pos[0]][current_bunny_pos[1]] = '@'; 
	for (int  i = 0; i < FieldSize; ++i) {
			printf("%s\n", Field[i]); 
	}
	
	return 0; 
}
