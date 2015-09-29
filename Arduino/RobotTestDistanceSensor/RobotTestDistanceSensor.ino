/*
 * Based on the DRV8835MotorShield library example amnd the
 * Example Sketch for VL6180x time of flight range finder.
 */
 
#include <DRV8835MotorShield.h>
#include <Wire.h>
#include <SparkFun_VL6180X.h>

#define PIN_LED 13

#define M1_SCALING 1.07   // Right
#define M2_SCALING 1.00   // Left

#define PIN_ENVELOPE_IN A1

#define QUIET 100
#define CLAP  300

// Motors
DRV8835MotorShield motors;

// Distance Sensor
#define VL6180X_ADDRESS 0x29
VL6180xIdentification identification;
VL6180x sensor(VL6180X_ADDRESS);

void setup()
{ 
  Stop();
    
  // if your motors' directions need to be flipped
  motors.flipM1(true);
  motors.flipM2(false);

  Serial.begin(115200);
  
  Wire.begin(); //Start I2C library
  delay(100); // delay .1s
  
  if(sensor.VL6180xInit() != 0)
  {
    Serial.println("FAILED TO INITALIZE"); //Initialize device and check for errors
  }
  
  sensor.VL6180xDefautSettings(); //Load default settings to get started.
 
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);   // Turn on LED to show ready
}

void loop()
{
    Stop();    
    WaitForClap();
    
    while (true)
    {
      MoveForwardToDistance(150, 16.0);
      TurnLeft(135, 0.1);
    }
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


// Function to move the robot forward until a certain distance from an object in cm.

void MoveForwardToDistance(int speed, float target_distance)
{
  motors.setM1Speed(speed*M1_SCALING);
  motors.setM2Speed(speed*M2_SCALING);
  
  while (true)
  {
    float current_distance = sensor.getDistance()/10.0;
    
    if (current_distance <= target_distance)
    {
       Stop();
       return;
    }
  }
  
  Stop();
}
