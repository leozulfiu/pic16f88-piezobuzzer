#include <xc.h>
#include "conf.h"

#define _XTAL_FREQ      4000000
#define TMR2PRESCALE    16

// TONES  ==========================================
#define  c     261
#define  d     293
#define  e     329
#define  f     349
#define  g     392
#define  a     440
#define  b     493
#define  C     523
#define  D     587
#define  E     659
#define  F     698
#define  G     784
// Define a special note, 'R', to represent a rest
#define  R     0
// ==================================================

int tempo = 2000;

int current_tone = 0; //current playing note
int current_tone_duration = 0; //current note duration

void delay_ms(unsigned int milliseconds) {
   while(milliseconds > 0) {
      milliseconds--;
       __delay_us(990);
   }
}

void SetupClock() {
    OSCCONbits.IRCF = 0b110; //set internal oscillator to 4 Mhz
    OSCCONbits.SCS = 0b00; //use oscillator defined by FOSC
}

void PWM1_Init(long desired_frequency) {
    PR2 = (_XTAL_FREQ / (desired_frequency * 4 * TMR2PRESCALE)) - 1;
    current_tone = desired_frequency;
}

int PWM_Max_Duty() {
    return (_XTAL_FREQ / (current_tone * TMR2PRESCALE));
}

void PWM1_Duty(unsigned int duty) {
    if (duty < 1024) {
        duty = ((float) duty / 1023) * PWM_Max_Duty();
        CCP1X = duty & 2;
        CCP1Y = duty & 1;
        CCPR1L = duty >> 2;
    }
}

void PWM1_Mode_On() {
    CCP1M3 = 1;
    CCP1M2 = 1;
}

void PWM1_Start() {
    PWM1_Mode_On();

#if TMR2PRESCALE == 1
    T2CKPS0 = 0;
    T2CKPS1 = 0;
#elif TMR2PRESCALE == 4
    T2CKPS0 = 1;
    T2CKPS1 = 0;
#elif TMR2PRESCALE == 16
    T2CKPS0 = 1;
    T2CKPS1 = 1;
#endif
    TMR2ON = 1;
}

void PWM1_Stop() {
    CCP1M3 = 0;
    CCP1M2 = 0;
}

void playTone() {
    //TODO: The Tone R is not implemented yet.
    if (current_tone > 0) {
        PWM1_Init(current_tone);
        PWM1_Duty(511);
        
        delay_ms(current_tone_duration);
        PWM1_Duty(0);
    }
}

int main(int argc, char** argv) {
    short melody[] = {g, g, a, g, C, b, g, g, a, g, D, C, g, g, G, E, C,
        b, a, F, F, E, C, D, C};
    char beats[] = {8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4,
        4, 4, 8, 8, 4, 4, 4, 2};

    char MELODY_LENGTH = sizeof (melody) / sizeof (melody[0]);

    SetupClock();
    TRISBbits.TRISB3 = 0; //set Pin B3 as output
    PORTB = 0x00;

    PWM1_Init(500); //exact value doesn't matter at all, just not 0
    PWM1_Duty(0);
    PWM1_Start();

    while (1) {
        for (int i = 0; i < MELODY_LENGTH; i++) {
            current_tone = melody[i];
            current_tone_duration = tempo / beats[i];

            playTone();
            delay_ms(tempo/200);
        }
        __delay_ms(2000);
    }
    return 0;
}