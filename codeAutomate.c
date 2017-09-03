#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdlib.h>

#define moteurG RB2
#define moteurD RB3
#define senseurG RB4
#define senseurD RB5
#define trigger RB6
#define pinEcho RB7
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

void echo(int *ptLastEchoCall){
    while(1){
        if(!pinEcho && *ptLastEchoCall>38){  
            trigger=1;
            __delay_us(10);
            trigger=0;
            *ptLastEchoCall=0;
            __delay_us(500);
        }
        if(pinEcho){
            return;
        }
        else{
            __delay_ms(38);
            *ptLastEchoCall=39;
            moteurG=0;
            moteurD=0;
        }
    }
}
void avancer(){
    int lastEchoCall=39;
    while(1){
        if(lastEchoCall>38){
            echo(&lastEchoCall);
        }
        if(!senseurG && !senseurD){ //avancer
            moteurG=1;
            moteurD=1;
            __delay_ms(2);
            lastEchoCall=lastEchoCall+2;
        }
        else if(senseurG && !senseurD){ //tourner gauche
            moteurG=0;
            moteurD=1;
            __delay_ms(40);
            lastEchoCall=39;
            if(senseurG && !senseurD){
                __delay_ms(40);
            }
        }
        else if(senseurD && !senseurG){ //tourner droite
            moteurG=1;
            moteurD=0;
            __delay_ms(40);
            lastEchoCall=39;
            if(senseurD && !senseurG){
                __delay_ms(40);
            }
        }
        else{
           __delay_ms(1); 
           lastEchoCall=lastEchoCall+1;
        }
    }
}

int main()
{
  moteurG = 0; //output 
  moteurD = 0;
  senseurG = 1; //input 
  senseurD = 1;
  trigger = 0;
  pinEcho = 1; 
  ANSELB = 0; // B = digital 
  
  avancer();
  return 0;
}
