void stage_01(char input_list[INPUT_LEN][BUF_SIZ])
{
  for (int i = 0; i< 7; i++){
    char tmp[50] = "";
    for (int j =0; j<7; j++){
      if(i!=j){
        strcpy(tmp, input_list[i]);
        strcat(tmp, input_list[j]);
        crack(1, tmp);
        char tmp[] = "";
      }
    }
  }
  return;
}
