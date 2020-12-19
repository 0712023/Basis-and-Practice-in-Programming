void stage_02(char input_list[INPUT_LEN][BUF_SIZ])
{
  char twolet[7][20] = {};
  for(int i = 0;i<7;i++){
    twolet[i][0] = input_list[i][0];
    twolet[i][1] = input_list[i][1];
  }
  for(int i = 0; i<7;i++){
    crack(2, twolet[i]);
  }
  return;
}
