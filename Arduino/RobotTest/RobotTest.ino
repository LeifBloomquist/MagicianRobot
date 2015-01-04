#include <DRV8835MotorShield.h>

/*
 * This example uses the DRV8835MotorShield library to drive each motor with the
 * Pololu DRV8835 Dual Motor Driver Shield for Arduino forward, then backward. 
 * The yellow user LED is on when a motor is set to a positive speed and off when
 * a motor is set to a negative speed.
 */

#define LED_PIN 13

DRV8835MotorShield motors;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  
  motors.setM1Speed(0);
  motors.setM2Speed(0);
  
  // if your motors' directions need to be flipped
  motors.flipM1(true);
  motors.flipM2(false);
  
  MoveForward(150, 3);
}

void loop()
{
  
  
  
}

// Function to move the robot forward.

void MoveForward(int speed, int seconds)
{
  motors.setM1Speed(speed);
  motors.setM2Speed(speed);
  
  delay(seconds*1000);
  
  // Slow down
  for (int ramp=speed; speed >= 0; speed--)
  {
    motors.setM1Speed(speed);
    delay(2);
  }
  
  motors.setM1Speed(0);
  motors.setM2Speed(0);
}
  
