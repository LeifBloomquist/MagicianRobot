/*
 * Robie Wifi
 * Based on the DRV8835MotorShield library example and the
 * Example Sketch for VL6180x time of flight range finder.
 */
 
#include <DRV8835MotorShield.h>
#include <Wire.h>
#include <SparkFun_VL6180X.h>
#include <SoftwareSerial.h>

#define M1_SCALING 1.25   // Right
#define M2_SCALING 1.00   // Left

#define PIN_LED 13
#define PIN_ENVELOPE_IN A1
#define PIN_RX          3
#define PIN_TX          4

#define QUIET 100
#define CLAP  300

// Motors
DRV8835MotorShield motors;

// Distance Sensor
#define VL6180X_ADDRESS 0x29
VL6180xIdentification identification;
VL6180x sensor(VL6180X_ADDRESS);

// Wifi
SoftwareSerial wifiSerial(PIN_RX, PIN_TX);

void setup()
{ 
    Stop();
    
    // if your motors' directions need to be flipped
    motors.flipM1(true);
    motors.flipM2(false);

    // Hardware Serial
    Serial.begin(115200);

    // Software Serial
    wifiSerial.begin(19200);
  
    Wire.begin(); //Start I2C library
    delay(100); // delay .1s
  
    if(sensor.VL6180xInit() != 0)
    {
        Serial.println("FAILED TO INITALIZE"); //Initialize device and check for errors
    }
  
    sensor.VL6180xDefautSettings(); //Load default settings to get started.
 
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);   // Turn on LED to show ready

    wifiSerial.println();
    wifiSerial.println("Roby is ready!");    
}

void loop()
{
    Stop(); 
    wifiSerial.println();
    
    while (true)
    {        
        wifiSerial.print("Roby is waiting for a command: ");        
        String command = GetInput();       

        if (command == "fw")
        {
            float seconds = HowManySeconds();
            MoveForward(150, seconds);
        }
        else if (command == "left")
        {
            float seconds = HowManySeconds();
            TurnLeft(50, seconds);
        }
        else if (command == "right")
        {
            float seconds = HowManySeconds();
            TurnRight(50, seconds);
        }
        else if (command == "bw")
        {
            float seconds = HowManySeconds();
            MoveBackward(150, seconds);
        }
        else if (command == "scan")
        {
            int distance = sensor.getDistance();
            wifiSerial.print("Distance is ");
            wifiSerial.print(distance / 10.0);
            wifiSerial.println(" cm");
        }
        else
        {
            wifiSerial.print("I don't understand: ");
            wifiSerial.println(command);
        }

        wifiSerial.println();
    }

/*    WaitForClap();
    
    while (true)
    {
      MoveForwardToDistance(150, 16.0);
      TurnLeft(135, 0.1);
    }
    */
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


// Wifi String handling

String GetInput()
{    
    String temp = GetInput_Raw();
    temp.trim();
    wifiSerial.println();    
    return temp;
}

String GetInput_Raw()
{
    char temp[50];

    int max_length = sizeof(temp);

    int i = 0; // Input buffer pointer
    char key;

    while (true)
    {
        key = ReadByte(wifiSerial); // Read in one character
        temp[i] = key;
        wifiSerial.write(key); // Echo key press back to the user.

        if (IsBackSpace(key) && (i > 0)) i -= 2; // Handles back space.        

        if (((int)key == 13) || (i == max_length - 1))   // The 13 represents enter key.
        {
            temp[i] = 0; // Terminate the string with 0.
            return String(temp);
        }
        i++;
        if (i < 0) i = 0;
    }
}

boolean IsBackSpace(char c)
{
    if ((c == 8) || (c == 20))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int ReadByte(Stream& in)
{
    while (in.available() == 0) {}
    return in.read();
}

float HowManySeconds()
{
    wifiSerial.print("How many seconds? ");
    float seconds = GetInput().toFloat();
    return seconds;
}