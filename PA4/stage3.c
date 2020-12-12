char map[64];
int pos = 0;
int goal = 0;

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
