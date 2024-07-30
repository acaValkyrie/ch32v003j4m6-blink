#include <Arduino.h>
#include <ch32v003fun.h>

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
  SETUP_SYSTICK_HCLK;
  pinMode(pin, OUTPUT);
}

static uint8_t gpioForPin(pin_size_t pin){
  if(pin < 2){
    return 0;
  }else if (pin < 10){
    return 2;
  }else{
    return 3;
  }
}

static GPIO_TypeDef* gpioRegister(uint8_t gpio){
  switch(gpio){
    case 0:
      return GPIOA;
    case 2:
      return GPIOC;
    default:
      return GPIOD;
  }
}

static uint8_t gpioPin(uint8_t gpio, pin_size_t pin){
  switch (gpio)
  {
  case 0:
    return pin;
  case 2:
    return pin-2;
  default:
    return pin-10;
  }
}

void loop(){
  // for(int i = 0; i < 255; i++){
  //   neoPixelWrite(pin, 0, 0, i);
  //   delay(10);
  // }
  // neopixel_write(GPIOC, GPIO_Pin_4, colorData, LED_NUM);
  uint8_t gpio = gpioForPin(pin);
  GPIO_TypeDef* port = gpioRegister(gpio);
  uint8_t p = gpioPin(gpio, pin);

  port->BSHR = 1 << p;
  _NOP();
  port->BCR = 1 << p;
  _NOP();
  port->BSHR = 1 << p;
  _NOP();
  port->BCR = 1 << p;
  _NOP();
  port->BSHR = 1 << p;
  _NOP();
  port->BCR = 1 << p;
  _NOP();
}
