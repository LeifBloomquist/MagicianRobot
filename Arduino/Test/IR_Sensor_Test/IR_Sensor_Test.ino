/*
 * Sharp 2Y0A21 Test
 */

#define PIN_ENVELOPE_IN A1
#define PIN_IR_SENSOR_IN A3

void setup()
{
  Serial.begin(9600);
}

void loop()
{  
   int raw = analogRead(PIN_IR_SENSOR_IN);
   Serial.print("Raw Value: ");
   Serial.println(raw);
  
   BarGraph();
}


void BarGraph()
{
  do
  {
    
   // Check the envelope input
   int value = analogRead(PIN_IR_SENSOR_IN);
   
   double x = value;
   double y = x * 5.0 / 1024.0;
   double distance = 61.681 * pow(y,-1.133);

   Serial.print("Distance: ");
   Serial.print(distance);
   Serial.println("");
   
   continue;
   
   int bar = distance;
   
   for (int i=0; i<bar; i++)
   {
      Serial.print("*");
   }
   
   Serial.println("");
  } while (true);
}

