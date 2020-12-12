#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char map[64]="";
int pos = 0;
int goal = 0;
int a = 0;
int b = 0;
int c = 0;
int d = 0;
int tail[100] = {0};
int tailpos = 0;
void up(){
  if(pos>7 && map[pos-8] != 'W'){
    pos-=8;
  }
}

void down(){
  if(pos<55 && map[pos+8] != 'W'){
    pos+=8;
  }
}

void right(){
  if(pos%8!=7 && map[pos+1] != 'W'){
    pos++;
  }
}

void left(){
  if(pos%8!=0 && map[pos-1] != 'W'){
    pos--;
  }
}

void func(char tmp){
  if(tmp == 'U'){
    up();
  } else if (tmp == 'D'){
    down();
  } else if (tmp == 'R'){
    right();
  } else if (tmp == 'L'){
    left();
  }
}

void slope_func(char tmp){
  if(tmp == 'U'){
    if(map[pos-8]>'A' && map[pos-8] <= 'D'){
      map[pos-8]--;
    } else if(map[pos-8] == 'A'){
      map[pos-8] ='E';
    } else if(map[pos-8] == 'E') {
      up();
    }
  } else if (tmp == 'D'){
    if(map[pos+8]>'A' && map[pos+8] <= 'D'){
      map[pos+8]--;
    } else if(map[pos+8] == 'A'){
      map[pos+8] ='E';
    } else if(map[pos+8] == 'E') {
      down();
    }
  } else if (tmp == 'R'){
    if(map[pos+1]>'A' && map[pos+1] <= 'D'){
      map[pos+1]--;
    } else if(map[pos+1] == 'A'){
      map[pos+1] ='E';
    } else if(map[pos+1] == 'E') {
      right();
    }
  } else if (tmp == 'L'){
    if(map[pos-1]>'A' && map[pos-1] <= 'D'){
      map[pos-1]--;
    } else if(map[pos-1] == 'A'){
      map[pos-1] ='E';
    } else if(map[pos-1] == 'E') {
      left();
    }
  }
}

void tail_func(char tmp){
  if(tmp == 'U'){
    if(pos>7 && map[pos-8] != 'W'){
      tail[tailpos] = pos;
      if(tailpos>4){
        map[tail[tailpos-5]] = 'E';
      }
      tailpos++;
      pos-=8;
    }
  } else if (tmp == 'D'){
    if(pos<55 && map[pos+8] != 'W'){
      tail[tailpos] = pos;
      if(tailpos>4){
        map[tail[tailpos-5]] = 'E';
      }
      tailpos++;
      pos+=8;
    }
  } else if (tmp == 'R'){
    if(pos%8!=7 && map[pos+1] != 'W'){
      tail[tailpos] = pos;
      if(tailpos>4){
        map[tail[tailpos-5]] = 'E';
      }
      tailpos++;
      pos++;
    }
  } else if (tmp == 'L'){
    if(pos%8!=0 && map[pos-1] != 'W'){
      tail[tailpos] = pos;
      if(tailpos>4){
        map[tail[tailpos-5]] = 'E';
      }
      tailpos++;
      pos--;
    }
  }
}

void check_stuck(){
  if(pos<8 && map[pos+1] == 'W' && map[pos-1] == 'W' && map[pos+8] == 'W'){
    exit(0);
  } else if(pos>55 && map[pos+1] == 'W' && map[pos-1] == 'W' && map[pos-8] == 'W') {
    exit(0);
  } else if(pos%8==7 && map[pos-8] == 'W' && map[pos-1] == 'W' && map[pos+8] == 'W') {
    exit(0);
  } else if(pos%8==0 && map[pos-8] == 'W' && map[pos+1] == 'W' && map[pos+8] == 'W') {
    exit(0);
  } else if(pos<8 && pos%8==7 && map[pos-1] == 'W' && map[pos+8] == 'W'){
    exit(0);
  } else if(pos%8==7 && pos>55 && map[pos-8] == 'W' && map[pos-1] == 'W'){
    exit(0);
  } else if(pos>55 && pos%8==0 && map[pos-8] == 'W' && map[pos+1] == 'W'){
    exit(0);
  } else if(pos%8==0 && pos<8 && map[pos+1] == 'W' && map[pos+8] == 'W'){
    exit(0);
  } else if(map[pos-8] == 'W' && map[pos+8] == 'W' && map[pos+1] == 'W' && map[pos-1] == 'W'){
    exit(0);
  }
}
void stage_1(){
  //현재위치, 목표위치 설정
  int x, y, xg, yg;
  scanf("%d %d", &x, &y);
	scanf("%d %d", &xg, &yg);
  pos = x*8+y;
  goal = xg*8+yg;

  //map 입력
  for(int i = 0 ; i< 128; i++){
		char ch = getchar();
    if(ch != ' '){
      map[i/2]=ch;
    }
	}

  //G입력
  map[goal] = 'G';

  //turn 입력
  int turn = 0;
	scanf("%d", &turn);

  //cmd라는 array에 명령어 쌓기
	int cnt = 0;
	char *cmd = (char*) calloc(turn, sizeof(char));
	while(cnt < turn){
    char input[7] = "";
    gets(input);
    if(strncmp(input, "WIND", 4) == 0){
			cmd[cnt-1]='\0';
      strcat(cmd, &input[5]);
    } else if(strncmp(input, "CMD", 3) == 0){
      cnt+=1;
      strcat(cmd, &input[4]);
    }
  }

  //첫 map 출력
  map[pos] = 'P';
  for(int i=0; i<64; i++) {
		printf("%c ", map[i]);
    if(i%8==7){
      printf("\n");
    }
  }
  printf("\n");

  //main print loop
  for(int k = 0; k<turn; k++){
    //P를 E로 바꿔주기 (P가 없는 지도 위에 명령마다 위치pos에 P를 입력)

    map[pos] = 'E';

    //move 실행
    func(cmd[k]);

    //map 출력
    map[pos] = 'P';
    for(int i=0; i<64; i++) {
  		printf("%c ", map[i]);
      if(i%8==7){
        printf("\n");
      }
    }
    printf("\n");

    //goal과 비교
    if(pos == goal){
			exit(0);
    }

  //end of main loop
  }
  free(cmd);
//end of stage 1
}

void stage_2(){
  //현재위치, 목표위치 설정
  int x, y, xg, yg;
  scanf("%d %d", &x, &y);
	scanf("%d %d", &xg, &yg);
  pos = x*8+y;
  goal = xg*8+yg;

  //map 입력
  for(int i = 0 ; i< 128; i++){
		char ch = getchar();
    if(ch != ' '){
      map[i/2]=ch;
    }
	}

  //G입력
  map[goal] = 'G';

  //turn 입력
  int turn = 0;
	scanf("%d", &turn);

  //cmd라는 array에 명령어 쌓기
	int cnt = 0;
	char *cmd = (char*) calloc(turn, sizeof(char));
	while(cnt < turn){
    char input[7] = "";
    gets(input);
    if(strncmp(input, "WIND", 4) == 0){
			cmd[cnt-1]='\0';
      strcat(cmd, &input[5]);
    } else if(strncmp(input, "CMD", 3) == 0){
      cnt+=1;
      strcat(cmd, &input[4]);
    }
  }

  //첫 map 출력
  map[pos] = 'P';
  for(int i=0; i<64; i++) {
		printf("%c ", map[i]);
    if(i%8==7){
      printf("\n");
    }
  }
  printf("\n");

  //main print loop
  for(int k = 0; k<turn; k++){
    //P를 E로 바꿔주기 (P가 없는 지도 위에 명령마다 위치pos에 P를 입력)

    map[pos] = 'E';

    //move 실행
    func(cmd[k]);

    //A,B,C,D 갯수 카운트
    if(map[pos] =='A'){
      a++;
    } else if(map[pos] =='B'){
      b++;
    } else if(map[pos] =='C'){
      c++;
    } else if(map[pos] =='D'){
      d++;
    }

    //map 출력
    map[pos] = 'P';
    for(int i=0; i<64; i++) {
  		printf("%c ", map[i]);
      if(i%8==7){
        printf("\n");
      }
    }
    printf("\n");

    //goal과 비교
    if(pos == goal){
      printf("%d %d %d %d ", a, b, c, d);
			exit(0);
    }

  //end of main loop
  }
  printf("%d %d %d %d ", a, b, c, d);
  free(cmd);
//end of stage 2
}

void stage_3(){
  //현재위치, 목표위치 설정
  int x, y, xg, yg;
  scanf("%d %d", &x, &y);
	scanf("%d %d", &xg, &yg);
  pos = x*8+y;
  goal = xg*8+yg;

  //map 입력
  for(int i = 0 ; i< 128; i++){
		char ch = getchar();
    if(ch != ' '){
      map[i/2]=ch;
    }
	}

  //G입력
  map[goal] = 'G';

  //turn 입력
  int turn = 0;
	scanf("%d", &turn);

  //cmd라는 array에 명령어 쌓기
	int cnt = 0;
	char *cmd = (char*) calloc(turn, sizeof(char));
	while(cnt < turn){
    char input[7] = "";
    gets(input);
    if(strncmp(input, "WIND", 4) == 0){
			cmd[cnt-1]='\0';
      strcat(cmd, &input[5]);
    } else if(strncmp(input, "CMD", 3) == 0){
      cnt+=1;
      strcat(cmd, &input[4]);
    }
  }

  //첫 map 출력
  map[pos] = 'P';
  for(int i=0; i<64; i++) {
		printf("%c ", map[i]);
    if(i%8==7){
      printf("\n");
    }
  }
  printf("\n");

  //main print loop
  for(int k = 0; k<turn; k++){
    //P를 E로 바꿔주기 (P가 없는 지도 위에 명령마다 위치pos에 P를 입력)

    map[pos] = 'E';

    //move 실행
    slope_func(cmd[k]);

    //map 출력
    map[pos] = 'P';
    for(int i=0; i<64; i++) {
  		printf("%c ", map[i]);
      if(i%8==7){
        printf("\n");
      }
    }
    printf("\n");

    //goal과 비교
    if(pos == goal){
			exit(0);
    }

  //end of main loop
  }
  free(cmd);
//end of stage 3
}

void stage_4(){
  //현재위치, 목표위치 설정
  int x, y, xg, yg;
  scanf("%d %d", &x, &y);
	scanf("%d %d", &xg, &yg);
  pos = x*8+y;
  goal = xg*8+yg;

  //map 입력
  for(int i = 0 ; i< 136; i++){
		char ch = getchar();
    if(ch != ' '){
      map[(i-i/17)/2] = ch;
    }
	}

  //G입력
  map[goal] = 'G';

  //turn 입력
  int turn = 0;
	scanf("%d", &turn);

  //cmd라는 array에 명령어 쌓기
	int cnt = 0;
	char *cmd = (char*) calloc(turn, sizeof(char));
	while(cnt < turn){
    char input[7] = "";
    gets(input);
    if(strncmp(input, "WIND", 4) == 0){
			cmd[cnt-1]='\0';
      strcat(cmd, &input[5]);
    } else if(strncmp(input, "CMD", 3) == 0){
      cnt+=1;
      strcat(cmd, &input[4]);
    }
  }

  //첫 map 출력
  map[pos] = 'P';
  for(int i=0; i<64; i++) {
		printf("%c ", map[i]);
    if(i%8==7){
      printf("\n");
    }
  }
  printf("\n");

  //main print loop
  for(int k = 0; k<turn; k++){
    //P를 E로 바꿔주기 (P가 없는 지도 위에 명령마다 위치pos에 P를 입력)

    map[pos] = 'W';

    //move 실행
    func(cmd[k]);

    //stuck인지 체크
    check_stuck();

    //map 출력
    map[pos] = 'P';
    for(int i=0; i<64; i++) {
  		printf("%c ", map[i]);
      if(i%8==7){
        printf("\n");
      }
    }
    printf("\n");

    //goal과 비교
    if(pos == goal){
			exit(0);
    }

  //end of main loop
  }
  free(cmd);
//end of stage 4
}

void stage_5(){
  //현재위치, 목표위치 설정
  int x, y, xg, yg;
  scanf("%d %d", &x, &y);
	scanf("%d %d", &xg, &yg);
  pos = x*8+y;
  goal = xg*8+yg;

  //map 입력
  for(int i = 0 ; i< 136; i++){
		char ch = getchar();
    if(ch != ' '){
      map[(i-i/17)/2] = ch;
    }
	}

  //G입력
  map[goal] = 'G';

  //turn 입력
  int turn = 0;
	scanf("%d", &turn);

  //cmd라는 array에 명령어 쌓기
	int cnt = 0;
	char *cmd = (char*) calloc(turn, sizeof(char));
	while(cnt < turn){
    char input[7] = "";
    gets(input);
    if(strncmp(input, "WIND", 4) == 0){
			cmd[cnt-1]='\0';
      strcat(cmd, &input[5]);
    } else if(strncmp(input, "CMD", 3) == 0){
      cnt+=1;
      strcat(cmd, &input[4]);
    }
  }

  //첫 map 출력
  map[pos] = 'P';
  for(int i=0; i<64; i++) {
		printf("%c ", map[i]);
    if(i%8==7){
      printf("\n");
    }
  }
  printf("\n");

  //main print loop
  for(int k = 0; k<turn; k++){
    //P를 W로 바꿔주기 (P가 없는 지도 위에 명령마다 위치pos에 P를 입력)
    map[pos] = 'W';

    //stuck인지 체크
    check_stuck();

    //move 실행
    tail_func(cmd[k]);


    //map 출력
    map[pos] = 'P';
    for(int i=0; i<64; i++) {
  		printf("%c ", map[i]);
      if(i%8==7){
        printf("\n");
      }
    }
    printf("\n");

    //goal과 비교
    if(pos == goal){
			exit(0);
    }

  //end of main loop
  }
  free(cmd);
//end of stage 5
}


void main() {
	int stage = 0;
	scanf("%d", &stage);
	switch(stage){
		case 1:
			stage_1();
			break;
		case 2:
			stage_2();
			break;
		case 3:
			stage_3();
			break;
		case 4:
			stage_4();
			break;
		case 5:
			stage_5();
			break;
	}
}
