#define _XTAL_FREQ 4000000

#include <xc.h>

// BEGIN CONFIG
#pragma config FOSC = ECH       // Oscillator Selection (ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin)
#pragma config WDTE = ON        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)
//END CONFIG

void echo(){
    int triggered=0;
    while(1){
        if(!triggered && !RB7){
            RB6=1;
            __delay_us(10);
            RB6=0;
            triggered=1;
        }
        //circuit anti rebond
        if(RB7 && triggered){
            __delay_us(500);
            if(RB7){
              //  RB2=1;
                __delay_ms(2);
                return;
            }else{
                RB2=0;
                RB3=0;
               // RB2=0;
                __delay_ms(38);
                
            }
            triggered=0;
        }
        __delay_ms(1);
    }
}
void avancer(int nb){
    while(1){
        echo();
        if(!RB4 && !RB5){ //avancer
            RB2=1;
            RB3=1;
            __delay_ms(3);
        }
        else if(RB4 && !RB5){ //tourner gauche
            while(RB4){
                RB2=0;
                RB3=1;
                __delay_ms(2);
            }
        }
        else if(RB5 && !RB4){ //tourner droite
            while(RB5){
                RB2=1;
                RB3=0;
                __delay_ms(2);
            }
        }
        else{
           __delay_ms(1); 
        }
    }
}

int main()
{
    
  TRISB2 = 0; //RB2 output : moteur gauceh
  TRISB3 = 0; // RB3 output : moteur droit
  TRISBbits.TRISB4 = 1; //RB4 input IR sensor gauche
  TRISBbits.TRISB5 = 1; //RB5 input IR sensor droit
  ANSELB = 0; // B = digital 
  
//  TRISB6 = 0; // trigger ultrason
//  TRISB7 = 1; //echo ultrason
//  echo();
  avancer(1);
  return 0;
}
