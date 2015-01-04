#include <DRV8835MotorShield.h>

/*
 * This example uses the DRV8835MotorShield library to drive each motor with the
 * Pololu DRV8835 Dual Motor Driver Shield for Arduino forward, then backward. 
 * The yellow user LED is on when a motor is set to a positive speed and off when
 * a motor is set to a negative speed.
 */

#define LED_PIN 13

#define M1_SCALING 0.95
#define M2_SCALING 1.00

DRV8835MotorShield motors;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  
  Stop();
  
  Wait(9); // 9 second delay
  digitalWrite(LED_PIN, HIGH);   // Turn on LED
  Wait(1);
  
  // if your motors' directions need to be flipped
  motors.flipM1(true);
  motors.flipM2(false);
}

void loop()
{
    MoveForward(150, 5);
    TurnRight(400, 0.2);
    MoveForward(150, 4);
    TurnLeft(400, 0.2);

    Wait(5);
}

// Function to move the robot forward.

void MoveForward(int speed, int seconds)
{
  motors.setM1Speed(speed*M1_SCALING);
  motors.setM2Speed(speed*M2_SCALING);
  
  Wait(seconds);
  
  // Slow down
  for (int ramp=speed; speed >= 0; speed--)
  {
    motors.setM1Speed(ramp);
    motors.setM2Speed(ramp);
    delay(2);
  }
  
  Stop();
}
  
// Function to move the robot backwards.  
  
void MoveBackward(int speed, float seconds)
{
  speed = -speed;
  
  motors.setM1Speed(speed*M1_SCALING);
  motors.setM2Speed(speed*M2_SCALING);
  
  Wait(seconds);
  
  // Slow down
  for (int ramp=speed; speed <= 0; speed++)
  {
    motors.setM1Speed(ramp);
    motors.setM2Speed(ramp);
    delay(2);
  }
  
  Stop();
}
 
 
// Function to stop the robot.

void Stop()
{
  motors.setM1Speed(0);
  motors.setM2Speed(0);
}

// Function to turn the robot left.  
  
void TurnLeft(int speed, float seconds)
{ 
  motors.setM1Speed(-speed*M1_SCALING);
  motors.setM2Speed(+speed*M2_SCALING);
  
  Wait(seconds);
  
  Stop();
}

// Function to turn the robot right.  

void TurnRight(int speed, float seconds)
{ 
  motors.setM1Speed(+speed*M1_SCALING);
  motors.setM2Speed(-speed*M2_SCALING);
  
  Wait(seconds);
  
  Stop();
}

// Function to wait for a number of seconds
void Wait(float seconds)
{
  delay(seconds*1000);
}
