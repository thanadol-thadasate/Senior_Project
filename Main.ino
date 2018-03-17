// First we include the libraries
#include <ESP8266WiFi.h>

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

#include <Wire.h>
#include <Adafruit_ADS1015.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <ThingSpeak.h>

#include <Time.h>
#include <TimeLib.h>

#include <StringSplitter.h>

#define BLYNK_PRINT Serial

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

const char* ssid = "Lenovo Z2 Plus"; //ssid ของ Wi-Fi ที่เราต้องการเชื่อมต่อค่ะ
const char* pass = "12345678"; //password ของ Wi-Fi ที่เราต้องการเชื่อมต่อค่ะ

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "c42219ce8e4d4e5e9e2187d1dd076358";

char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "D3DHCZC22G7068VS"; //เอา Write API Key ของเรามาใส่ค่ะ
WiFiClient client; //เรียกใช้การเชื่อมต่อกับ Wi-Fi

String LINE_TOKEN = "Ihqmiws8oF4ntECpYWv7YG6UblWJLF69Dlv0mWNN1vN";

Adafruit_ADS1115 ads1115(0x48); /* Use this for the 16-bit version */

int16_t adc3;  // we read from the ADC, we have a sixteen bit integer as a result

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

unsigned long int avgValue;
float b;
int buf[10], temp;
float phValue;

int sensorValue;
float voltage;
float ntu;

unsigned long lastSent = millis();

int timezone = 7 * 3600;  // ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;  // กำหนดค่า Date Swing Time

void setup(void)
{

  // start serial port
  Serial.begin(115200);
  pinMode(A0, OUTPUT);
  // Start up the library
  Blynk.begin(auth, ssid, pass);
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov"); // ดึงเวลาจาก Server
  sensors.begin();
  Wire.begin(4, 5);
  ads1115.setGain(GAIN_ONE); // 1x gain   +/- 4.096V  1 bit = 0.125mV // การตั้งค่า Gain = 1x หรือช่วงวัด +/- 4.096V ความละเอียด 0.125mV/bit
  ads1115.begin();

}

void loop()
{

  pH();
  temperature();
  turbidity();
  thingspeak();
  Blynk.run();
  delay(1000);

}

void pH()
{

  for (int i = 0; i < 10; i++)      //Get 10 sample value from the sensor for smooth the value
  {
    adc3 = ads1115.readADC_SingleEnded(3); // address GND (0x048) pin 3
    buf[i] = adc3;
    delay(100);
  }

  for (int i = 0; i < 9; i++)       //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  avgValue = 0;
  for (int i = 2; i < 8; i++)                     //take the average value of 6 center sample
    avgValue += buf[i];
  phValue = (float)avgValue * 3.3 / 27000.0 / 6;   //convert the analog into millivolt
  phValue = 3.5 * phValue;                    //convert the millivolt into pH value
  Blynk.virtualWrite(V1, phValue);
  Serial.println(" ");
  Serial.print("   pH: ");
  Serial.print(phValue, 2);
  Serial.println(" ");

  if (phValue < 5.0)
  {
    String txt = "pH: " + (String)phValue + " --> เข้าใกล้ความเป็นกรด";
    Line_Notify(LINE_TOKEN, txt);
  }
  else if (phValue > 9.0)
  {
    String txt = "pH: " + (String)phValue + " --> เข้าใกล้ความเป็นด่าง";
    Line_Notify(LINE_TOKEN, txt);
  }

  delay(1000);

}

void temperature()
{

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus

  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperature readings
  Serial.println("DONE");

  Serial.print("        Temperature is: ");

  float temperature = sensors.getTempCByIndex(0);

  Serial.println(temperature, 2); // Why "byIndex"?
  Blynk.virtualWrite(V2, sensors.getTempCByIndex(0));
  // You can have more than one DS18B20 on the same bus.
  // 0 refers to the first IC on the wire

  if (temperature < 25.0)
  {
    String txt = "Temperature: " + (String)(sensors.getTempCByIndex(0)) + " --> น้ำเย็นเกินไป";
    Line_Notify(LINE_TOKEN, txt);
  }
  else if (temperature > 32.0)
  {
    String txt = "Temperature: " + (String)(sensors.getTempCByIndex(0)) + " --> น้ำร้อนเกินไป";
    Line_Notify(LINE_TOKEN, txt);
  }

  delay(1000);

}

void turbidity()
{

  sensorValue = analogRead(A0);   // read the input on analog pin 0:
  voltage = sensorValue * (5.0 / 1024.0);   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  ntu = (-1120.4 * pow(voltage, 2)) + (5742.3 * voltage) - 4352.9;
  Blynk.virtualWrite(V3, ntu);
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" v");
  Serial.print("Turbidity: ");
  Serial.print(ntu);
  Serial.println(" ntu");

  if (ntu == 1000)
  {
    String txt = "Turbidity: " + (String)ntu + " --> น้ำเริ่มขุ่น";
    Line_Notify(LINE_TOKEN, txt);
  }
  else if (ntu > 1000)
  {
    String txt = "Turbidity: " + (String)ntu + " --> น้ำขุ่นมาก อยู่ในเกณฑ์อันตราย";
    Line_Notify(LINE_TOKEN, txt);
  }

  delay(1000);

}

void thingspeak()
{

  String pH = (String)phValue;
  String temp = (String)(sensors.getTempCByIndex(0)); //เนื่องจาก temp เป็น float ซึ่งจะส่งค่าไม่ได้ เราจึกต้องแปลงเป็น String ก่อนค่ะ
  String turb = (String)ntu; //Turbidity
  String data = "field1=" + pH + "&field2=" + temp + "&field3=" + turb; //ข้อมูล String ที่เราจะส่งค่าไปยัง ThingSpeak ค่ะ

  time_t noww = time(nullptr);    // ดึงค่าเวลาจาก ntp
  String tm  = ctime(&noww);      // convert to time
  char space = ' ';

  StringSplitter *splitter = new StringSplitter(tm, space, 5);    // splitter time (format example: fri march 16 hh:mm:ss 2018)
  String item = splitter->getItemAtIndex(3);                      // format hh:mm:ss

  StringSplitter *splitter2 = new StringSplitter(item, ':', 3);   // splitter hh:mm:ss
  String item2 = splitter2->getItemAtIndex(1);                    // storage minute
  String item3 = splitter2->getItemAtIndex(2);                    // storage second

  int mm = item2.toInt();   // convert string minute to int
  int ss = item3.toInt();   // convert string second to int

  if (mm % 30 == 0)  // check time for minute equals 00 or 30
  {
    if (ss <= 30)   // check time for second equals 00 or 15
    {
      if (client.connect(thingSpeakAddress, 80))
      {
        Serial.println(data);
        client.print("POST /update HTTP/1.1\n"); //การส่งค่าข้อมูลผ่าน HTTP ค่ะ
        client.print("Host: api.thingspeak.com\n");
        client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
        client.print("Connection: close\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(data.length());
        client.print("\n\n");
        client.print(data); //path ข้อมูลที่เราจะส่งค่าไปยัง ThingSpeak ค่ะ
      }
    }
  }

  delay(5000);

}

void Line_Notify(String LINE_Token, String message)
{

  String msg = String("message=") + message;

  WiFiClientSecure lineclient;
  if (!lineclient.connect("notify-api.line.me", 443))
  {
    Serial.println("Connection failed");
    return;
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_Token) + "\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(msg.length()) + "\r\n";
  req += "\r\n";
  req += msg;

  lineclient.print(req);

  delay(1000);

  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      break;
    }
  }

}
