//
//
// Program for testing the Parallax High Speed Servo
//
//


/* ### Models ### */
typedef struct _pwm{
    int pin_out;    //OUTPUT
    int refresh_rate;
    float forward_rate;
    float backward_rate;
    float stall_rate;  
    int count;
    unsigned long time_start;
    unsigned long time_curr;
} _pwm;

/* ### Globals ### */
_pwm pulse;


/* ### Main ### */
void setup(void)
{
    init_pulse(&pulse);
    pinMode(pulse.pin_out,OUTPUT);
}


void loop(void)
{
    _wait_pulse_duration(&pulse, pulse.refresh_rate);

}


void init_pulse(_pwm* pulse)
{     
    pulse->pin_out = 13;
    pulse->count = 0;
    pulse->time_start = 0;
    pulse->time_curr = 0;
    pulse->refresh_rate = 20000; // us
    pulse->forward_rate = 1700;  // us
    pulse->backward_rate = 1300; // us
    pulse->stall_rate = 15200;   // us
}


void _wait_pulse_duration(_pwm* pulse, int dur)
{
    int delta = 0;
    pulse->time_start = micros();
    while(delta >= dur)
    {
        pulse->time_curr = micros();
        delta = pulse->time_curr % pulse->time_start;
    }
}

/* ### Motor Control Functions ### */

void move(_pwm* pulse, int dir_rate)
{
    // Arguments
    // pulse: pointer to pulse struct
    // dir_rate: amount of time to wait to define the direction

    digitalWrite(pulse->pin_out,LOW);
    _wait_pulse_duration(pulse,pulse->refresh_rate);
    digitalWrite(pulse->pin_out,HIGH);
    _wait_pulse_duration(pulse,dir_rate);
    digitalWrite(pulse->pin_out,LOW);
}
