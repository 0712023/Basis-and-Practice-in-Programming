void stage_03(char input_list[INPUT_LEN][BUF_SIZ])
{
  for (int i = 0; i<6; i++){
    char tmp[100] = {};
    for (int j =0; j<6; j++){
      if(i!=j){
        for(int k = 0; k<6;k++){
          if(j!=k){
            strcpy(tmp, input_list[i]);
            strcat(tmp, input_list[j]);
            strcat(tmp, input_list[k]);
            crack(3, tmp);
            char tmp[] = {};
            strcpy(tmp, input_list[i]);
            strcat(tmp, input_list[j]);
          }
        }
        char tmp[] = {};
        strcpy(tmp, input_list[i]);
      }
    }
  }
  return;
}
