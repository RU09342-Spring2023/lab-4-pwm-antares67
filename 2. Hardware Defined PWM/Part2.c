#include <msp430.h>


/*
 *
 *  Red
    Orange (Red + Green)
    Green
    Cyan (Green + Blue)
    Blue
    Purple (Blue + Red)
 *
 *
 * */

#include <msp430.h>

void portSetup();
void timerConfig();
void ccrSetup();
void switchMode();

unsigned char percentage = 60;
unsigned long percentIncrement = 105;
unsigned long CCR0Val = 1050;
unsigned long CCR1Val = 512;



int main(void){

    WDTCTL = WDTPW | WDTHOLD;

    portSetup();
    timerConfig();
    ccrSetup();
  //  switchMode();



    while(1){

    }

    return 0;
}

void portSetup(){

    //Setup ports
    P6DIR |= BIT0; //config port 3.0 output
    P6OUT &= ~BIT0; //clear

    P6DIR |= BIT1; //config port 3.0 output
    P6OUT &= ~BIT1; //clear

    P6DIR |= BIT2; //config port 3.0 output
    P6OUT &= ~BIT2; //clear


    PM5CTL0 &= ~LOCKLPM5; //enable GPIO


    // @TODO Initialize Button 2.3
    P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
    P4REN |= BIT1;                          // P2.3 pull-up register enable
    P4IES &= ~BIT1;                         // P2.3 Low --> High edge
    P4IE |= BIT1;                           // P2.3 interrupt enabled
    P4IFG &= 0;

}
void timerConfig(){

    //timer setup
    TB0CTL |= TBCLR; //clear timer and dividers
    TB0CTL |=TBSSEL__SMCLK; //clk source = submain
    TB0CTL |= MC__UPDOWN; //up mode
    TB0CCR0 = CCR0Val;
    TB0CCR1 = CCR1Val;

}
void ccrSetup(){

    //setup timer compare
    TB0CCTL0 |= CCIE; //enable overflow IRQ
    TB0CCTL0 &= ~CCIFG; //clear flag

    TB0CCTL1 |= CCIE; //enable overflow IRQ
    TB0CCTL1 &= ~CCIFG; //clear flag
    __enable_interrupt();

}




//ISR

#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void){
    P1OUT ^= BIT0;
    TB0CCTL0 &= ~CCIFG;
    TB0CCR0 = CCR0Val;
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void ISR_TB0_CCR1(void){
    P1OUT ^= BIT0;
    TB0CCTL1 &= ~CCIFG;
    TB0CCR1 = CCR1Val;
}

// Port 4 interrupt service routine
//#pragma vector=PORT4_VECTOR
//__interrupt void Port_4(void)
//{
//    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.
//    P4IFG &= ~BIT1;
//    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.
//
//    switch (speedTracker)
//    {
//    case 0:
//        speedTracker = 1;
//        offset = 5000;
//        break;
//    case 1:
//        speedTracker = 2;
//        offset = 2500;
//        break;
//    case 2:
//        speedTracker = 0;
//        offset = 1250;
//        break;
//
//    }
//
//}


// Port 4 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.
    P4IFG &= ~BIT1;
    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.

    switch(percentage){

    case 0:
        CCR0Val = 0;
        CCR1Val = CCR0Val;
        percentage = 10;
        break;
    case 10:
        TB0CCR0 = 1050;
        CCR0Val = 1050;
        TB0CCR1 = 1050;
        CCR1Val = 1050;

        TB0CCR1 -= percentIncrement;
        CCR1Val -= percentIncrement;
        percentage = 20;
        break;
    case 20:
        CCR1Val -= percentIncrement;
        percentage = 30;
        break;
    case 30:
        CCR1Val -= percentIncrement;
        percentage = 40;
        break;
    case 40:
        CCR1Val -= percentIncrement;
        percentage = 50;
        break;
    case 50:
        CCR1Val = (CCR0Val/2);
        percentage = 60;
        break;
    case 60:
        CCR1Val -= percentIncrement;
        percentage = 70;
        break;
    case 70:
        CCR1Val -= percentIncrement;
        percentage = 80;
        break;
    case 80:
        CCR1Val -= percentIncrement;
        percentage = 90;
        break;
    case 90:
        CCR1Val -= percentIncrement;
        percentage = 100;
        break;
    case 100:
        CCR1Val = 0;
        percentage = 0;
        break;

    }
}






