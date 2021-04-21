#include <Arduino.h>
#include <LowPower.h> 
#include <avr/interrupt.h>
boolean flaga1 = 0;
boolean flaga5 =0;
int flaga2 = 0;
uint8_t wartosc = 0;
long czas = 0;

void wstan()
{
    detachInterrupt(0); 
}
void spij()
{
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

void setup() {

    Serial.begin(9600);

    DDRD = (1 << PD3);
    DDRB = (1 << PB3) | (1 << PB2);
    PORTB = (1 << PORTB2);
    PORTC = (1 << PORTC5);

    ADMUX = (1 << REFS0) | (1 << ADLAR) ;
    DIDR0 = (1 << ADC0D) | (1 << ADC1D);
    //ADCSRA = (1 <<ADPS0) | (1 <<ADPS1);
    ADCSRA |= (1 << ADEN) ;
    pinMode(2, INPUT_PULLUP);
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);
    pinMode(10, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(2), wstan , LOW); 
}

void modulacja(uint8_t i)
{
    TCCR2A |= (1<< COM2B1) | (1<< WGM21) | (1<< WGM20);
    TCCR2B |= (1<<CS22) | (1<<CS20);
    OCR2B = i;
}

int przetwornik()
{   
    ADCSRA |= (1 << ADSC);
    while ((ADCSRA & _BV(ADSC)));
            return ADCH;
         
}

void loop()
{
    
    flaga1 = 0;
    flaga2 = 0;
    //flaga5 =0;
    int flaga3 = 0; 
    digitalWrite(10, HIGH);
    EIMSK = 0b00000000;
if ((PINC && 0x8)==0)
  {
     delay(150);
        if ((PINC && 0x8)==1)
          {
              czas = millis();
              while (flaga2 == 0)
              {
                  if ((PINC && 0x8)==0)
                   {
                       flaga3 = flaga3+1;
                        while((PINC && 0x8)==0){}
                       delay(150);
                   }
                    if ((flaga3==2) & (millis()-czas > 900 ))
                    {
                               ADMUX = 0b01100001;
                                wartosc = przetwornik();
                                Serial.println(wartosc);
                                    if (wartosc > 210)
                                        {
                                            for(uint8_t i = 0; i<5; i++ )
                                            {
                                            digitalWrite(5, HIGH);
                                            delay(200);
                                            digitalWrite(5, LOW);
                                            delay(200);
                                            }
                                            
                                        }
                                    else if(wartosc > 195)
                                        {
                                            for(uint8_t i = 0; i<4; i++ )
                                            {
                                            digitalWrite(5, HIGH);
                                            delay(200);
                                            digitalWrite(5, LOW);
                                            delay(200);
                                            }
                                        }
                                    else if(wartosc > 183)
                                        {
                                          for(uint8_t i = 0; i<3; i++ )
                                            {
                                            digitalWrite(5, HIGH);
                                            delay(200);
                                            digitalWrite(5, LOW);
                                            delay(200);
                                            }
                                        }
                                     else if(wartosc > 179)
                                        {
                                            for(uint8_t i = 0; i<2; i++ )
                                            {
                                            digitalWrite(5, HIGH);
                                            delay(200);
                                            digitalWrite(5, LOW);
                                            delay(200);
                                            }
                                        }
                                     else if(wartosc > 164)
                                        {
                                            digitalWrite(5, LOW);
                                            delay(200);
                                            digitalWrite(5, HIGH);
                                            
                                        }
                                    else if(wartosc < 164)
                                        {
                                            digitalWrite(5, LOW);
                                            delay(1500);
                                            digitalWrite(5, HIGH);
                                            
                                        }
        
                                flaga2=1;
                                ADMUX = 0b01100000;

                    }else if ((flaga3>=4) & (millis()-czas > 900 ))
                        {
                                for(uint8_t i = 0; i<4; i++ )
                                            {
                                            digitalWrite(5, HIGH);
                                            delay(200);
                                            digitalWrite(5, LOW);
                                            delay(200);
                                            } 
                                flaga2=1;
                                flaga5=!flaga5;

                    } else if (millis()-czas > 910 )
                        {   
                            flaga2=1;
                        }  
              }
              
          }
        uint8_t wartosc2 =0;
        boolean flaga6 =1;
    while (((PINC && 0x8)==0)&(flaga5 ==0))

                    {   while(flaga6 == 1){

                        ADMUX = 0b01100001;
                        wartosc2 = przetwornik();
                        ADMUX = 0b01100000;
                        flaga6=0;
                        }
                        if(wartosc2 > 164){
                        while(flaga1 == 0) 
                        {
                            wartosc = przetwornik();
                            Serial.println(wartosc);
                            flaga1 = 1;
                        }
                        modulacja(wartosc);
                        delay(50);
                        }
                        else
                        {
                              digitalWrite(5, LOW);
                              delay(1500);
                              digitalWrite(5, HIGH);
                              
                        }
                        
                    }
            
    digitalWrite(PB3, LOW);
      
  } 
  digitalWrite(10, LOW);
  EIMSK = 0b00000001;
  spij();
}