#include <Arduino.h>

const int pin = C4;

static uint8_t gpioForPin(pin_size_t pin);
static GPIO_TypeDef* gpioRegister(uint8_t gpio);
static uint8_t gpioPin(uint8_t gpio, pin_size_t pin);
// void code_0(GPIO_TypeDef* port, uint8_t p);
// void code_1(GPIO_TypeDef* port, uint8_t p);
void code_res(int pin);
void intToBitArray(int value, int* bitArray, int bitArraySize);
void makeCommand(int* command, int r, int g, int b);
void sendCommand(int pin, int* command, unsigned int length);
void neoPixelWrite(int pin, int r, int g, int b);


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

void code_res(int pin){
  digitalWrite(pin, LOW);
  Delay_Us(100);
}

void intToBitArray(int value, int* bitArray, int bitArraySize) {
  for (int i = 0; i < bitArraySize; i++) {
    bitArray[bitArraySize - 1 - i] = (value & (1 << i)) ? 1 : 0;
  }
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

#define T0H 0.25
#define T1H 0.6
#define T0L 1.0
#define T1L 0.65

void code_0(GPIO_TypeDef* port, uint8_t p) {
  port->BSHR = 1 << p; \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 7; i++) _NOP(); \
  port->BCR = 1 << p; \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 6; i++) _NOP();
}

void code_1(GPIO_TypeDef* port, uint8_t p) {
  port->BSHR = 1 << p; \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  for(int i = 0; i < 8; i++) _NOP(); \
  port->BCR = 1 << p; \
  for(int i = 0; i < 1; i++) _NOP(); 
}

#define code_IO(port, p, value) \
  if(value == 0){ code_0(port, p); }else if(value == 1){ code_1(port, p);}

void sendCommand(int pin, int* command, unsigned int length){
  uint8_t gpio = gpioForPin(pin);
  GPIO_TypeDef* port = gpioRegister(gpio);
  uint8_t p = gpioPin(gpio, pin);
  int v0 = command[0];
  int v1 = command[1];
  int v2 = command[2];
  int v3 = command[3];
  int v4 = command[4];
  int v5 = command[5];
  int v6 = command[6];
  int v7 = command[7];
  int v8 = command[8];
  int v9 = command[9];
  int v10 = command[10];
  int v11 = command[11];
  int v12 = command[12];
  int v13 = command[13];
  int v14 = command[14];
  int v15 = command[15];
  int v16 = command[16];
  int v17 = command[17];
  int v18 = command[18];
  int v19 = command[19];
  int v20 = command[20];
  int v21 = command[21];
  int v22 = command[22];
  int v23 = command[23];
  code_IO(port, p, v0);
  code_IO(port, p, v1);
  code_IO(port, p, v2);
  code_IO(port, p, v3);
  code_IO(port, p, v4);
  code_IO(port, p, v5);
  code_IO(port, p, v6);
  code_IO(port, p, v7);
  code_IO(port, p, v8);
  code_IO(port, p, v9);
  code_IO(port, p, v10);
  code_IO(port, p, v11);
  code_IO(port, p, v12);
  code_IO(port, p, v13);
  code_IO(port, p, v14);
  code_IO(port, p, v15);
  code_IO(port, p, v16);
  code_IO(port, p, v17);
  code_IO(port, p, v18);
  code_IO(port, p, v19);
  code_IO(port, p, v20);
  code_IO(port, p, v21);
  code_IO(port, p, v22);
  code_IO(port, p, v23);

  port->BCR = 1 << p;
  for(int i = 0; i < 8; i++) _NOP();
}

void neoPixelWrite(int pin, int r, int g, int b){
  int command[24];
  makeCommand(command, r, g, b);
  sendCommand(pin, command, 24);
}

void setup(){
  SETUP_SYSTICK_HCLK;
  pinMode(pin, OUTPUT);
}

namespace mymath{
  // ch32v003 does not supoport, including sin function, some math libraries.
  // https://community.platformio.org/t/ch32v-arduino-support-issue/38128/9
  float sin(float x){
    if (x < -3.14159){
      x += 6.283185;
    }else if (x > 3.14159){
      x -= 6.283185;
    }
    // Maclaurin 7th order approximation of the sin function
    return x - x*x*x/6.0 + x*x*x*x*x/120.0 - x*x*x*x*x*x*x/5040.0;
  }
}

void loop(){
  for(int t = -180; t < 180; t++){
    float a = 5;
    int r = (int)(a * (1 + mymath::sin(3.14159 * (t + 0) / 180.0)));
    int g = (int)(a * (1 + mymath::sin(3.14159 * (t + 120) / 180.0)));
    int b = (int)(a * (1 + mymath::sin(3.14159 * (t + 240) / 180.0)));
    neoPixelWrite(pin, r, g, b);
    neoPixelWrite(pin, g, b, r);
    neoPixelWrite(pin, b, r, g);
    
    delay(100);
  }
}
