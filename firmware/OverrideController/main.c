// includes
#include <avr/io.h>
#include <avr/interrupt.h>

#define OVERRIDE_PORT PB4
#define RC_RECEIVER_PORT PB3
#define RC_RECEIVER_PCINT PCINT3


// Macros
#define OVERRIDE_ON PORTB |= (1 << OVERRIDE_PORT)
#define OVERRIDE_OFF PORTB &= ~(1 << OVERRIDE_PORT)
#define RISING_EDGE PINB & (1 << RC_RECEIVER_PORT)

// Function prototypes
void Calibrate_OSCILLATOR(void);
void Init_INTERRUPTS(void);
void Init_PORT(void);
void Override_Control(float, float);
float Percent_PWM(uint8_t);

// Global variables
volatile uint8_t override_pulse_ready = 0;
volatile uint8_t override_timer_count = 0;

int main(void)
{
    Calibrate_OSCILLATOR();
    Init_PORT();
    Init_INTERRUPTS();
    
    float override_high_trigger = 15.0;
    float override_low_trigger = 10.0;

    while (1)
    {       
        if (override_pulse_ready) {
            
            override_pulse_ready = 0;

            Override_Control(override_low_trigger, override_high_trigger);           // percent PWM hysteresis
            
            GIFR = (1 << PCIF);             // clear Pin Change Interrupt Flag. Might not be needed - Doc says flag is cleared after ISR is executed.
            GIMSK |= (1 << PCIE);           // Pin Change Interrupt Enable
        }
    }
}

float Percent_PWM(uint8_t count)
{
    // Returns a linear percent ratio of PWM high pulse with 
    // 1ms = 0% and 2ms = 100%
    // Time units are in ms
    float min = 1;
    float max = 2;
    float p = 0.016;    // ms/count. This is dependent on the timer prescaler (0.016 for prescaler of 128) 
    
    float pct = (p*count - min) / (max - min) * 100;
    
    pct = (pct < 0) ? 0 : pct;
    pct = (pct > 100) ? 100 : pct;
    
    return pct;
}

void Override_Control(float pct_low, float pct_high)
{    
    float pct;

    pct = Percent_PWM(override_timer_count);
    
    if (pct >= pct_high) 
    {
        OVERRIDE_ON;
    }
    else if (pct <= pct_low)
    {
        OVERRIDE_OFF;
    }
    else {} // Inside hysteresis band, maintain previous state
}

void Calibrate_OSCILLATOR(void)
{
    // Benefits to calibration defined on page 164.
    // OSCCAL value impacts described on page 32.
    // Recommended to calibrate to not higher than 8MHz (page 24).
    OSCCAL = 0x8e;                      
}

void Init_PORT(void)
{
    // Set pin directions
    // 0 = input; 1 = output
    DDRB |= (1 << OVERRIDE_PORT);
    DDRB &= ~(1 << RC_RECEIVER_PORT);
}

void Init_INTERRUPTS(void)
{
    GIMSK |= (1 << PCIE);
    PCMSK |= (1 << RC_RECEIVER_PCINT);     // Enable pin change interrupt
    sei();                      // Enable Global Interrupt flag
}

ISR(PCINT0_vect)
{
    if (RISING_EDGE)
    {   
        TCNT1 = 0;
        TCCR1 = (1 << CS13);    // start timer with prescaler CK/128 => 125 steps per 2ms
        //TCCR1 = (1 << CS12) | (1 << CS11) | (1 << CS10);   // start timer1 with prescaler CK/64 => 250 steps per 2ms
        
        return;
    }
    
    override_timer_count = TCNT1;
    TCCR1 = 0;
    GIMSK &= ~(1 << PCIE);
    
    override_pulse_ready=1;
}
