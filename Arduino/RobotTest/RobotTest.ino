#include <DRV8835MotorShield.h>

/*
 * Based on the DRV8835MotorShield library example.
 */

#define PIN_LED 13

#define M1_SCALING 1.07   // Right
#define M2_SCALING 1.00   // Left

#define PIN_ENVELOPE_IN A1

#define QUIET 100
#define CLAP  300

DRV8835MotorShield motors;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(LED_LED, HIGH);   // Turn on LED
  
  Stop();
    
  // if your motors' directions need to be flipped
  motors.flipM1(true);
  motors.flipM2(false);

  Serial.begin(9600);
}

void loop()
{
    Stop();    
    WaitForClap();
    MoveForward(150, 0);
    
    //TurnRight(400, 0.2);
    //MoveForward(150, 4);
    //TurnLeft(400, 0.2);

//    Wait(5);
}

// Function to move the robot forward.

void MoveForward(int speed, float seconds)
{
  motors.setM1Speed(speed*M1_SCALING);
  motors.setM2Speed(speed*M2_SCALING);
  
  if (seconds == 0)
  {
     WaitForClap();
  }
  else
  {
    Wait(seconds);
  }
  
  // Slow down
  for (int ramp=speed; speed >= 0; speed--)
  {
    motors.setM1Speed(ramp*M1_SCALING);
    motors.setM2Speed(ramp*M2_SCALING);
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
    motors.setM1Speed(ramp*M1_SCALING);
    motors.setM2Speed(ramp*M2_SCALING);
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


// Function to wait for a clap
void WaitForClap()
{
  int value=0;
  
  // Part 1 - Wait for quiet. 
  value = analogRead(PIN_ENVELOPE_IN); 
  
  while (value > QUIET) 
  {
    // Keep waiting.
    value = analogRead(PIN_ENVELOPE_IN);
  }
  Serial.println("Quiet before.");

  // Part 2 - Wait for clap
   value = analogRead(PIN_ENVELOPE_IN); 
  
  while (value < CLAP) 
  {
    // Keep waiting.
    value = analogRead(PIN_ENVELOPE_IN);
  }
  Serial.println("Clap!");
  
  // Part 3 - Wait for quiet again.  
  value = analogRead(PIN_ENVELOPE_IN); 
  
  while (value > QUIET) 
  {
    // Keep waiting.
    value = analogRead(PIN_ENVELOPE_IN);
  }
  Serial.println("Quiet after."); 
}

void BarGraph()
{
  do
  {
    
   // Check the envelope input
   int value = analogRead(PIN_ENVELOPE_IN);

   Serial.print("Value: ");
   
   int bar = value/10;
   
   for (int i=0; i<bar; i++)
   {
      Serial.print("*");
   }
   
   Serial.println("");
  } while (true);
}

