#include <ezButton.h>
#define num0 B11000000 //.GFEDCBA arrangement
#define num1 B11111001 //.GFEDCBA arrangement
#define num2 B10100100 //.GFEDCBA arrangement
#define num3 B10110000 //.GFEDCBA arrangement
#define num4 B10011001 //.GFEDCBA arrangement
#define num5 B10010010 //.GFEDCBA arrangement
#define num6 B10000010 //.GFEDCBA arrangement
#define num7 B11111000 //.GFEDCBA arrangement
#define num8 B10000000 //.GFEDCBA arrangement
#define num9 B10010000 //.GFEDCBA arrangement
#define dp B01111111 //.GFEDCBA arrangement
#define base_num B00000001 // base digit
#define tens_num B00000010 // tens digit
#define hund_num B00010000 // hundred digit
#define thsn_num B01000000 // thousand digit
uint8_t number[] = {num0,num1,num2,num3,num4,num5,num6,num7,num8,num9,dp};
uint8_t digit [] = {thsn_num,hund_num,tens_num,base_num};
ezButton alarm(4);
ezButton jamplus(3);
ezButton menitplus(2);
unsigned long detikraw = 0;
int jam = 0;
int menit = 0;
int detik = 0;
int detiknow = 0;
int detiklast = 0;
unsigned long pressed = 0;
unsigned long released = 0;
long durasi = 0;

void setup() {
  TCCR1B |= (1 << WGM12); 
  TIMSK |= (1 << OCIE1A); 
  OCR1A  = 15624; //15624
  TCCR1B |= (1 << CS11)|(1 << CS10); 
  DDRB = 0xFF;
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  alarm.setDebounceTime(100);
  jamplus.setDebounceTime(100);
  menitplus.setDebounceTime(100);

}

ISR(TIMER1_COMPA_vect){
  detikraw = millis();
  detik = detikraw/1000;
  detiknow = detik-detiklast;
  if(detiknow==60){
    detiklast=detik;
    menit++;
    if (menit==60){
      jam++;
      menit=0;
    }
  }
  if (jam==24){
    menit=0;
    jam=0;
  }
}

void alarmon(){
}

void blanking(){
  PORTD = B00000000; //Blanking Signal
  PORTB = B11111111; //Blanking Signal
  delay(1); //note : don't add too much unless you want blinking 7segment...
}

void tampiljam(){
  int ha = jam%10;
  int hb = jam/10%10;
  int ma = menit%10;
  int mb = menit/10%10;

  PORTD = digit[0];
  PORTB = number[ma];
  delay(1); 
  blanking();//
  PORTD = digit[1];
  PORTB = number[mb];
  delay(1); 
  blanking();//delay(3); 
  PORTD = digit[2];
  PORTB = number[ha];
  delay(1); 
  blanking();//delay(3); 
  PORTD = digit[2];
  PORTB = dp;
  delay(1); 
  blanking();//delay(3); 
  PORTD = digit[3];
  PORTB = number[hb];
  delay(1); 
  blanking();//delay(5); 
 
}
void loop() {
  tampiljam();
  jamplus.loop();
  menitplus.loop();

  if (jamplus.isReleased()){
    jam++;
    if (jam == 60){
      jam = 0;
    }
  }
  if (menitplus.isReleased()){
    menit++;
    if (menit == 60){
      menit = 0;
    }
  }

}
