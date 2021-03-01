// Program for testing the Parallax High Speed Servo

#include <string.h>

/* ### Header ###*/
typedef struct _pwm {
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
void setup(void) { 
    Serial.begin(115200);
    init_pulse(&pulse);
    pinMode(pulse.pin_out,OUTPUT);
    int prog_flag = 1;
    char input = 'x';
    bool input_flag = false;
    bool motor_start_flag = false;
    bool demo_mode = false;
    char *str_in = "";
    char *msg = "{'arduino':'done'}";
    while (prog_flag) {
        if (!demo_mode) {
            // Wait for start flag
            read_serial(str_in);
            // Move the actuator
            Serial.println("{'arduino':'Moving forward for 5 second'}");
            move_test(&pulse, 'f');
            delay(1000);
            Serial.println("{'arduino':'Moving backward for 5 second'}");
            move_test(&pulse, 'b');
            // Send end flag
            Serial.write(msg);
        }
        else {
            Serial.flush();
            //        move_test3(&pulse);
            //        move_test2(&pulse);
            Serial.println("Moving forward for 5 second");
            move_test(&pulse, 'f');
            delay(1000);
            Serial.println("Moving backward for 5 second");
            move_test(&pulse, 'b');
            // Send end response
            Serial.println("Continue?");
            Serial.println("Input(y/n): ");
            input_flag = false;
            while (!input_flag){
                if (Serial.available() > 0){
                    input = Serial.read();
                    if (input == 'y' || input == 'n'){
                        input_flag = true;
                    }
                }
            }
            if (input == 'n'){
                prog_flag = 0;
            }
        }
    }
    Serial.println("END");
}

void loop(void) {
}

void init_pulse(_pwm* pulse) {     
    pulse->pin_out = 8;
    pulse->count = 0;
    pulse->time_start = 0;
    pulse->time_curr = 0;
    pulse->refresh_rate = 20000; // us
    pulse->forward_rate = 1550;  // us
    pulse->backward_rate = 1350; // us
    pulse->stall_rate = 1520;   // us
}

void _wait_pulse_duration(_pwm* pulse, int dur) {
    int delta = 0;
    unsigned long time_curr = 0;
    unsigned long time_start = micros();
    while (delta <= dur)
    {
        time_curr = micros();
        delta = time_curr - time_start;
    }
}

/* ### Motor Control Functions ### */
void move(_pwm* pulse, int dir_rate)
{
    // Arguments
    // pulse: pointer to pulse struct
    // dir_rate: amount of time to wait to define the direction

    digitalWrite(pulse->pin_out,HIGH);
    _wait_pulse_duration(pulse,dir_rate);
    digitalWrite(pulse->pin_out,LOW);
    _wait_pulse_duration(pulse,(pulse->refresh_rate - dir_rate));

}

void move_test(_pwm* pulse, char dir) {
    bool moving = true;
    unsigned long start_time = millis();
    unsigned long curr_time = 0;
    unsigned long delta = 0;
    int stop_time = 5000;
    while (moving) {
        curr_time = millis();
        delta = curr_time - start_time;
        if (delta >= stop_time) {
            moving = false;
            break;
        }
        if (dir == 'f') {
            move(pulse, pulse->forward_rate);  
        }
        else if (dir == 'b') { 
          move(pulse, pulse->backward_rate);
        }
        
    }
}

void move_test2(_pwm* pulse) {
    Serial.println("Moving forward");
    analogWrite(pulse->pin_out,20);
    delay(5000);
    Serial.println("Moving forward");
    analogWrite(pulse->pin_out, 17);
    delay(5000);
    analogWrite(pulse->pin_out, 0);
}

void move_test3(_pwm* pulse) {
  for (int i = 0; i<=200; i++) {
    digitalWrite(pulse->pin_out, HIGH);
    delay(1.25);
    digitalWrite(pulse->pin_out, LOW);
    delay(18.75);
  }
  
  for (int i = 0; i<=200; i++) {
    digitalWrite(pulse->pin_out, HIGH);
    delay(1.5);
    digitalWrite(pulse->pin_out, LOW);
    delay(18.5);
  }
  
  for (int i = 0; i<=200; i++) {
    digitalWrite(pulse->pin_out, HIGH);
    delay(1.75);
    digitalWrite(pulse->pin_out, LOW);
    delay(18.25);
  }
}

void read_serial(char* str_in){
/*
    Parses serial data formatted using opening and closing curly brackets.
    The data inside the curly brackets is concat to str_in

    Example: "{start}"

    Parameters:
        char* str_in
*/
    bool start_flag = false;
    bool end_flag = false;
    bool reading = false;
    // char str_in[255] = "";
    int str_in_cur_pos = strlen(str_in);
    char buffer[16] = "";
    char pattern_start[3] = "{";
    char pattern_end[3] = "}";
    // Loop to detect start flag
    while(reading){
        Serial.readBytes(buffer,1);
        if(start_flag == true){
            strcat(str_in,buffer);
        }
        else if(strcmp(buffer,pattern_start)){
            strcpy(buffer,"");
            start_flag = true;
        }
        else if(strcmp(buffer,pattern_end)){
            strcpy(buffer,"");
            end_flag = true;
            reading = false;
            break;
        }
    }
}
