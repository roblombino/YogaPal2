/******************************************************************************
Mux_Analog_Input
SparkFun Multiplexer Analog Input Example
Jim Lindblom @ SparkFun Electronics
August 15, 2016
https://github.com/sparkfun/74HC4051_8-Channel_Mux_Breakout

This sketch demonstrates how to use the SparkFun Multiplexer
Breakout - 8 Channel (74HC4051) to read eight, separate
analog inputs, using just a single ADC channel.

Hardware Hookup:
Mux Breakout ----------- Arduino
     S0 ------------------- 2
     S1 ------------------- 3
     S2 ------------------- 4
     Z -------------------- A0
    VCC ------------------- 5V
    GND ------------------- GND
    (VEE should be connected to GND)

The multiplexers independent I/O (Y0-Y7) can each be wired
up to a potentiometer or any other analog signal-producing
component.

Development environment specifics:
Arduino 1.6.9
SparkFun Multiplexer Breakout - 8-Channel(74HC4051) v10
(https://www.sparkfun.com/products/13906)
******************************************************************************/
/////////////////////
// Pin Definitions //
/////////////////////

//LED strips
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN       11
#define NUMPIXELS 24

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int selectPins[3] = {2, 3, 4}; // S0~2, S1~3, S2~4
const int zOutput = 5; 
const int zInput = A0; // Connect common (Z) to A0 (analog input)

int senVals[8] = {};

int avg = 0;
int sum = 0;

int senLocs[8][2]={ 

    //Test dd
    {21, 23}, //channel 0, ALU
    {18, 20}, //channel 1, ARU
    { 0,  2}, //channel 2, ALD
    { 3,  5}, //channel 3, ARD
    
    {15, 17}, //channel 4, BLU
    {12, 14}, //channel 5, BRU
    { 6,  8}, //channel 6, BLD
    { 9, 11}, //channel 7, BRD

}; //str, fin

void setup() 
{
  Serial.begin(9600); // Initialize the serial port
  // Set up the select pins as outputs:
  for (int i=0; i<3; i++)
  {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
  }
  pinMode(zInput, INPUT); // Set up Z as an input

  // Print the header:
  Serial.println("Y0\tY1\tY2\tY3\tY4\tY5\tY6\tY7");
  Serial.println("---\t---\t---\t---\t---\t---\t---\t---");

  //LED setup
  pixels.setBrightness(100);
  pixels.begin();
  pixels.show();
}

void loop() 
{
  sum = 0;
  avg = 0;
  
  // Loop through all eight pins.
  for (byte pin=0; pin<=7; pin++)
  {
    selectMuxPin(pin); // Select one at a time
    int inputValue = analogRead(A0); // and read Z
    Serial.print(String(inputValue) + "\t");
    senVals[pin] = inputValue;
  }
  Serial.println();
  
  for(int j = 0; j < 8; j ++){
    sum = sum + senVals[j];
  }

  avg = sum / 8;

  Serial.print("sum = ");
  Serial.println(sum);

  Serial.print("avg = ");
  Serial.println(avg);

  for(int k = 0; k <= 8; k ++){
      if( avg > 800 ){
        for(int y = 0; y <= 8; y ++){
          pixelColor( "Purple", senLocs[y][0], senLocs[y][1] );
        }
      }
      else if( (senVals[k] > 800) && (avg > 300) ){
        //set to Green
        pixelColor( "Green", senLocs[k][0], senLocs[k][1] );
      }
      else if( (senVals[k] < 800) && (avg > 300) ){
        //set to Red
        pixelColor( "Red", senLocs[k][0], senLocs[k][1] );
      }
      //else if( (senVals[k] < 800) && (avg < 300) ){
        //set to Blue
        //pixelColor( "Blue", senLocs[k][0], senLocs[k][1] );
      //}
      else{
        //set to Blue
        pixelColor( "Clear", senLocs[k][0], senLocs[k][1] );
      }
  }
  
  delay(10);
  /*for(int z = 0; z <= 8; z ++){
     pixelColor( "Clear", senLocs[z][0], senLocs[z][1] );
  }*/
  
  delay(1000);
}

// The selectMuxPin function sets the S0, S1, and S2 pins
// accordingly, given a pin from 0-7.
void selectMuxPin(byte pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
}

void pixelColor( String color, int str, int fin ) {

  for( int w = str; w <= fin; w++){
    if( color == "Red" ){
      redLED( w );
    } else if( color == "Green"){
      grnLED( w );
    } else if( color == "Blue"){
      bluLED( w );
    } else if( color == "Purple"){
      purLED( w );
    } else if( color == "Clear"){
      clrLED( w );
    } 
      delay(10);
  }
}

//turns LEDs red
void redLED( int x ) {
  //Red   = 255, 0, 0
  pixels.setPixelColor( x, pixels.Color( 255, 0, 0 ));
  pixels.show();
}

//turns LEDs green
void grnLED( int x ) {
  //Green = 0, 255, 0
  pixels.setPixelColor( x, pixels.Color( 0, 255, 0 ));
  pixels.show();
}

//turns LEDs blue
void bluLED( int x ) {
  //Blue  = 0, 0, 255
  pixels.setPixelColor( x, pixels.Color( 0, 0, 255 ));
  pixels.show();
}

//turns LEDs purple
void purLED( int x ) {
  //Purple  = 75, 0, 130
  pixels.setPixelColor( x, pixels.Color( 75, 0, 130 ));
  pixels.show();
}

//turns LEDs clear
void clrLED( int x ) {
  //Clear  = 0, 0, 0
  pixels.setPixelColor( x, pixels.Color( 0, 0, 0 ));
  pixels.show();
}
