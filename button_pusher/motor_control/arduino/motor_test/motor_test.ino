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


void setup(_pwm* pulse)
{
    pulse->count = 0;
    pulse->time_start = 0;
    pulse->time_curr = 0;
    pulse->refresh_rate = 20;
    pulse->forward_rate = 1.7;
    pulse->backward_rate = 1.3;
    pulse->stall_rate = 1.52;


}


void loop(_pwm* pulse)
{
    wait_refresh_rate(pulse);

}


void wait_pulse_duration(_pwm* pulse, int dur)
{
    int delta = 0;
    pulse->time_start = millis();
    while(delta >= dur)
    {
        pulse->time_curr = millis();
        delta = pulse->time_curr % pulse->time_start;
    }
}


void pulse_on(_pwm* pulse)
{
    // Set pin high
    // Call wait_pulse_duration
    // Set pin low
}