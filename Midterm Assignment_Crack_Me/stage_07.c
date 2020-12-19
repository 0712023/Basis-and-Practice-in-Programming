void stage_07(char input_list[INPUT_LEN][BUF_SIZ])
{
  char buffer[50] = {};
  for(int i = 0;i<6;i++){
    strcpy(buffer, input_list[i]);
    for(int j = 0 ; j<strlen(input_list[i]);j++){
      if(buffer[j]>96){
        buffer[j]-=32;
      }
    }
    crack(7,buffer);
  }
  return;
}
