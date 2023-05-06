/**

author: Thomas Rasa
March 2023

This code allows you to control a servo motor my pressing the pin 2.3 and 4.1 buttons 

*/
#include <msp430.h>

void pwmSweepRIGHT();
void pwmSweepLEFT();

int main(void)
{

    //button setup 2.3, 4.1

    P2DIR &= ~BIT3; //input
    P4DIR &= ~BIT1; //input



    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
    //pwm pin config
    P6DIR |= BIT0;                     // P6.0 output
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0;                    // P6.0 options select

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    TB3CCR0 = 20000+910;                         // PWM Period
    TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCR1 = 2100;                            // CCR1 PWM duty cycle
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR

    //button pin interrupt
         P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
         P2REN |= BIT3;                          // P2.3 pull-up register enable
         P2IES &= ~BIT3;                         // P2.3 Low --> High edge
         P2IE |= BIT3;                           // P2.3 interrupt enabled

         P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
         P4REN |= BIT1;                          // P2.3 pull-up register enable
         P4IES &= ~BIT1;                         // P2.3 Low --> High edge
         P4IE |= BIT1;                           // P2.3 interrupt enabled

    __enable_interrupt();



    while(1){
    }


}

void pwmSweepLEFT(){ //while within range continue sweeping

    if(TB3CCR1 < 3000 && TB3CCR1 > 800 ){
                TB3CCR1++;
            }
            else{
               __no_operation();
            }
    __delay_cycles(1000);

}

void pwmSweepRIGHT(){ //while within range continue sweeping
        if(TB3CCR1 < 3000 && TB3CCR1 > 800){
            TB3CCR1--;
        }
        else{
           __no_operation();
        }
    __delay_cycles(1000);

}


// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                         // Clear P2.3 IFG
    pwmSweepRIGHT();

}

// Port 2 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 IFG
    pwmSweepLEFT();

}

