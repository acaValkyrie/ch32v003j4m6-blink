void intToBitArray(int value, int* bitArray, int bitArraySize) {
  for (int i = 0; i < bitArraySize; i++) {
    bitArray[bitArraySize - 1 - i] = (value & (1 << i)) ? '1' : '0';
  }
  bitArray[bitArraySize] = '\0'; // Null-terminate the string
}

void makeCommand(int* command, int r, int g, int b){
  int red_str[8];
  int green_str[8];
  int blue_str[8];
  // uint to bit array
  intToBitArray(r, red_str, 8);
  intToBitArray(g, green_str, 8);
  intToBitArray(b, blue_str, 8);
  for(int i = 0; i < 8; i++){
    command[i] = red_str[i];
    command[i+8] = green_str[i];
    command[i+16] = blue_str[i];
  }
}

#include <stdio.h>
int main(){
  int command[24];
  makeCommand(command, 128,128,255);
  for(int i = 0; i < 24; i++){
    printf("%c", command[i]);
  }
  printf("\n");
}