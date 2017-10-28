//=================================================================
/* 4-Bit mode 16x2 LCD Module Software      */
/* 2nd Dec 2005                             */
/* Copyright 2005 Circuits4You.com          */ 
/* WWW - http://www.circuits4you.com        */
/* Email - info@circuits4you.com            */
 
/* LCD Pin-5(R/W) must be connected to ground*/
//=================================================================
#define F_CPU 16000000UL
#include <avr/io.h>
#include <string.h>
#include <avr/myInputOutput.h>
#include <util/delay.h>

#define E   PC5
#define RS   PC4

void display(char string[16], char LineNo);
void displaybyte(char D);
void dispinit(void);
void epulse(void);
void delay_ms(unsigned int de);
void clear(void);
void moveCursorRight(void);
void moveCursorLeft(void);
void moveCursorUp(void);
void moveCursorDown(void);
void enableCursor(void);

//=================================================================
//        Main Function
//=================================================================
int main(void)
{
 DDRC = 0x03F;  //Set LCD Port Direction   
  //delay_ms(500);  //Initiaize LCD
  _delay_ms(500);
  dispinit();
  _delay_ms(200);
  //delay_ms(200);
 display(" Hi There....",1);
 display("Circuits4You.com",2);
 pinMode('D',5,0);
 pinMode('D',2,0); //For enable cursor
 pinMode('D',3,1);
 pinMode('D',6,0); //For take input move cursor left
 pinMode('D',7,0); //For take input move cursor right
 pinMode('D',0,0); //For take input move cursor up 
 pinMode('D',1,0); //For take input move cursor down

 int input;
 int left_input = 0;
 int right_input = 0;
 int up_input = 0;
 int down_input = 0;
 int cursor_input = 0;

  while(1) {
  	//delay_ms(25000);
	//display("Hello Bocho",2);
  	/*display("Hello Bocho",1);
	delay_ms(5000);
	display("I love you bocho",1)*/;
	input = digitalRead('D',5);
	if(input == 1) {
		clear();
		_delay_ms(500);
		display("Button Clicked",2);
		_delay_ms(500);
		//display("Circuits4You.com",2);
	}

	/*input2 = digitalRead('D',2);
	if(input2 == 1) {
		digitalWrite('D',3,1);
		delay_ms(3000);
	}
	else {
		digitalWrite('D',3,0);
		delay_ms(3000);
	}*/
	
	//Move cursor left
	left_input = digitalRead('D',6);
	if(left_input == 1) {
		moveCursorLeft();
		_delay_ms(100);
	}

	//Move cursor right
	right_input = digitalRead('D',7);
	if(right_input == 1) {
		moveCursorRight();
		_delay_ms(100);
	}

	//Move cursor up
	up_input = digitalRead('D',0);
	if(up_input == 1) {
		moveCursorUp();
		_delay_ms(1);
	}

	//Move cursor down
	down_input = digitalRead('D',1);
	if(down_input == 1) {
		moveCursorDown();
		_delay_ms(1);
	}

	//Enable cursor
	cursor_input = digitalRead('D',2);
	if(cursor_input == 1) {
		enableCursor();
		_delay_ms(1);
	}
  }
}

//=================================================================
//        LCD Display Initialization Function
//=================================================================
void dispinit(void)
{
 int count;
 char init[]={0x43,0x03,0x03,0x02,0x28,0x01,0x0C,0x06,0x02,0x02};
  
 PORTC &= ~(1<<RS);           // RS=0
 for (count = 0; count <= 9; count++)
  {
 displaybyte(init[count]);
  }
 PORTC |= 1<<RS;    //RS=1
}


//=================================================================
//        Enable Pulse Function
//=================================================================
void epulse(void)
{
 PORTC |= 1<<E;
 _delay_ms(3); //Adjust delay if required
 PORTC &= ~(1<<E);
 _delay_ms(3); //Adjust delay if required
}


//=================================================================
//        Send Single Byte to LCD Display Function
//=================================================================
void displaybyte(char D)
{
 //data is in Temp Register
  char K1;
  K1=D;
  K1=K1 & 0xF0;
  K1=K1 >> 4;  //Send MSB
  
  PORTC &= 0xF0; 
  PORTC |= (K1 & 0x0F); 
 epulse();

  K1=D;
  K1=K1 & 0x0F;  //Send LSB
  PORTC &= 0xF0;  
  PORTC |= K1;
 epulse();
}

//=================================================================
//        Display Line on LCD at desired location Function
//=================================================================
void display(char string[16], char LineNo)
{
 int len,count;

 PORTC &= ~(1<<RS);           // RS=0 Command Mode

 if(LineNo==1)
 {
  displaybyte(0x80);   //Move Coursor to Line 1
 }
 else
 { 
  displaybyte(0xC0);   //Move Coursor to Line 2
 }
 PORTC |= (1<<RS);           // RS=1 Data Mode
 _delay_ms(5);

        len = strlen(string);
   for (count=0;count<len;count++)
  {
    displaybyte(string[count]);
 }
}


//=================================================================
//        Delay Function
//=================================================================
void delay_ms(unsigned int de)
{
unsigned int rr,rr1;
   for (rr=0;rr<de;rr++)
   {
  
  for(rr1=0;rr1<30;rr1++)   //395
  {
   asm("nop");
  }
   
   }
}

void clear(void) {
	PORTC &= ~(1<<RS);           // RS=0
 	displaybyte(0x01);
  	PORTC |= 1<<RS;    //RS=1
}

//=================================================================
//        Move Cursor Right Function
// This function is used for move cursor right
//=================================================================
void moveCursorRight(void) {
	PORTC &= ~(1<<RS);           // RS=0
 	displaybyte(0x14);
  	PORTC |= 1<<RS;    			 // RS=1
	_delay_ms(1);
}

//=================================================================
//        Move Cursor Left Function
// This function is used for move cursor left
//=================================================================
void moveCursorLeft(void) {
	PORTC &= ~(1<<RS);           // RS=0
 	displaybyte(0x10);
  	PORTC |= 1<<RS;    			 // RS=1
	_delay_ms(1);
}

//=================================================================
//        Move Cursor Up Function
// This function is used for move cursor line 2 to line 1
//=================================================================
void moveCursorUp(void) {
	PORTC &= ~(1<<RS);           // RS=0
 	displaybyte(0x80);
  	PORTC |= 1<<RS;    			 // RS=1
	_delay_ms(1);
}

//=================================================================
//        Move Cursor Down Function
// This function is used for move cursor line 1 to line 2
//=================================================================
void moveCursorDown(void) {
	PORTC &= ~(1<<RS);           // RS=0
 	displaybyte(0xC0);
  	PORTC |= 1<<RS;    			 // RS=1
	_delay_ms(1);
}

//=================================================================
//       Enable Cursor Function
// This function is used for enable cursor
//=================================================================
void enableCursor(void) {
	PORTC &= ~(1<<RS);           // RS=0
 	displaybyte(0x0E);
  	PORTC |= 1<<RS;    			 // RS=1
	_delay_ms(1);

	PORTC &= ~(1<<RS);           // RS=0
 	displaybyte(0x0F);
  	PORTC |= 1<<RS;    			 // RS=1
	_delay_ms(1);
}
