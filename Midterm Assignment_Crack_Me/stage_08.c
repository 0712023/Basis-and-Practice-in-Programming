void stage_08(char input_list[INPUT_LEN][BUF_SIZ])
{
  char buffer[50] = {};
  for(int i = 0;i<6;i++){
    strcpy(buffer, input_list[i]);
    for(int j = 0 ; j<strlen(input_list[i]);j++){
      if(buffer[j]>47 && buffer[j]<51){
        buffer[j]+=7;
      } else if (buffer[j]>50 && buffer[j]<58){
        buffer[j]-=3;
      }
    }
    crack(8,buffer);
  }
  return;
}
