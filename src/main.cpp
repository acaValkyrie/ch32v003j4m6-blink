#include <Arduino.h>

const int pin = C4;

#define T0H 0.25
#define T1H 0.6
#define T0L 1.0
#define T1L 0.65

#define NOP asm("nop")

void code_0(int pin){
  digitalWrite(pin, HIGH);
  Delay_Us(T0H);
  NOP;
  digitalWrite(pin, LOW);
  Delay_Us(T0L);
}

void code_1(int pin){
  digitalWrite(pin, HIGH);
  Delay_Us(T1H);
  digitalWrite(pin, LOW);
  Delay_Us(T1L);
}

void code_res(int pin){
  digitalWrite(pin, LOW);
  Delay_Us(100);
}

void intToBitArray(uint value, char* bitArray, int bitArraySize) {
  for (int i = 0; i < bitArraySize; i++) {
    bitArray[bitArraySize - 1 - i] = (value & (1 << i)) ? '1' : '0';
  }
  bitArray[bitArraySize] = '\0'; // Null-terminate the string
}

void makeCommand(char* command, int r, int g, int b){
  char red_str[8];
  char green_str[8];
  char blue_str[8];
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

void sendCommand(int pin, char* command, unsigned int length){
  for(unsigned int i = 0; i < length; i++){
    if(command[i] == 0){
      code_0(pin);
    }else{
      code_1(pin);
    }
  }
  code_res(pin);
}

void neoPixelWrite(int pin, int r, int g, int b){
  char command[24];
  makeCommand(&command[0], r, g, b);
  sendCommand(pin, &command[0], sizeof(command)/sizeof(command[0]));
  
}

void setup(){
  pinMode(pin, OUTPUT);
  // digitalWrite(pin, HIGH);
  GPIOSpeed_TypeDef speed = GPIO_Speed_50MHz;
}

void loop(){
  // for(int i = 0; i < 255; i++){
  //   neoPixelWrite(pin, 0, 0, i);
  //   delay(10);
  // }
  digitalWrite(pin, HIGH);
  for (int i = 0; i < 8; i++){
    NOP;
  }

  digitalWrite(pin, LOW);
  NOP;
}
