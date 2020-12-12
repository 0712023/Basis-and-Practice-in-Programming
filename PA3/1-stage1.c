void stage_1() {
	int cnt = 0;
	int *pnt = &cnt;
  int len = 0;
  scanf("%d\n", &len);
  char *str = (char*) calloc(len, sizeof(char));
  fgets(str, len+1, stdin);
  for(int i = 0; i<len; i++){
    for(int j = 0;j<len-1;j++){
      if(str[j]>str[j+1]){
        char tmp;
        tmp = str[j];
        str[j] = str[j+1];
        str[j+1] = tmp;
      }
    }
  }
	for(int k = 0; k<len;k++){
		for(int i = 0; i< len-1; i++){
			if(str[i] == str[i+1]){
				*pnt+=1;
				for(int j = i+1; j< len-1 ; j++){
					char tmp;
					tmp = str[j];
					str[j] = str[j+1];
					str[j+1] = tmp;
				}
      	str[len-1] = 0;
			}
		}
	}
	printf("%s\n",str);
  free(str);
}
