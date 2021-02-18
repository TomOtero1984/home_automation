/*
Program for testing the Parallax High Speed Servo
*/



typedef struct _pwm{
    int refresh_rate;
    float forward_rate;
    float backward_rate;
    float stall_rate;  
    int count;
    unsigned long time_start;
    unsigned long time_curr;
} _pwm;

//Global
_pwm pulse;

void setup(void)
{
    init_pulse(&pulse);
}


void loop(void)
{
    _wait_pulse_duration(&pulse, pulse.refresh_rate);

}


void init_pulse(_pwm* pulse)
{     
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


void forward(_pwm* pulse, int dur)
{
    // pin low
    _wait_pulse_duration(pulse,pulse->refresh_rate);
    // pin high
    _wait_pulse_duration(pulse,pulse->forward_rate);
    // pin low    
}


void backward(_pwm* pulse, int dur)
{
    _wait_pulse_duration(pulse,pulse->refresh_rate);
    _wait_pulse_duration(pulse,pulse->backward_rate);    
}


void stall(_pwm* pulse, int dur)
{
    _wait_pulse_duration(pulse,pulse->refresh_rate);
    _wait_pulse_duration(pulse,pulse->stall_rate);    
}


void pulse_on(_pwm* pulse)
{
    // Set pin high
    // Call wait_pulse_duration
    // Set pin low
}
