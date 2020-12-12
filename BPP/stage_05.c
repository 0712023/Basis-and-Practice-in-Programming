void stage_05(char input_list[INPUT_LEN][BUF_SIZ])
{
  char buffer[50] = "";
  for(int i = 0;i<6;i++){
    strcpy(buffer, input_list[i]);
    for(int j = 0; j<strlen(input_list[i]); j++){
      for(int k = 0; k<strlen(input_list[i])-j-1; k++){
        char tmp[2] = {};
        tmp[0] = buffer[k];
        buffer[k] = buffer[k+1];
        buffer[k+1] = tmp[0];
      }
    }
    crack(5, buffer);
  }
  return;
}
