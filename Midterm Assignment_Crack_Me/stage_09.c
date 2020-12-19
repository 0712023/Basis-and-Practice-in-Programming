void stage_09(char input_list[INPUT_LEN][BUF_SIZ])
{
  char buffer[50] = {};
  for(int i = 0;i<6;i++){
    strcpy(buffer, input_list[i]);
    for(int j = 0 ; j<strlen(input_list[i]);j++){
      if(buffer[j]>64 && buffer[j]<77 || buffer[j]>96 && buffer[j]<109){
        buffer[j]+=14;
      } else if(buffer[j]>76 && buffer[j]<91 || buffer[j]>108 && buffer[j]<123){
        buffer[j]-=12;
      }
    }
    crack(9,buffer);
  }
  return;
}
