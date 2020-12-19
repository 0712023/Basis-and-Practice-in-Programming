void stage_00(char input_list[INPUT_LEN][BUF_SIZ])
{
  for(int i = 0; i<7;i++){
    crack(0, input_list[i]);
  }
  return;
}
