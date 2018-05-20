// First we include the libraries
#include <ESP8266WiFi.h>

#include <BlynkSimpleEsp8266.h>

#include <Wire.h>
#include <Adafruit_ADS1015.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Time.h>
#include <TimeLib.h>

// Data wire is plugged into pin 4 on the NodeMCU
#define ONE_WIRE_BUS 2

BlynkTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "10a79685a54f444c83af892a08a7864c";

char ssid[] = "AIS 4G Pocket Wifi_627121";    // ssid ของ Wi-Fi ที่เราต้องการเชื่อมต่อ
char pass[] = "37627121";          // password ของ Wi-Fi ที่เราต้องการเชื่อมต่อ
bool Connected2Blynk = false;

char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "0O3WMXHUJX4YS698";    // เอา Write API Key ของเรามาใส่
WiFiClient client;                          // เรียกใช้การเชื่อมต่อกับ Wi-Fi
String Msg_pH;
String Msg_temp;
String Msg_turb;
String data;

String LINE_TOKEN = "Ihqmiws8oF4ntECpYWv7YG6UblWJLF69Dlv0mWNN1vN";

Adafruit_ADS1115 ads1115(0x48);   /* Use this for the 16-bit version */

int16_t adc3;   // we read from the ADC, we have a sixteen bit integer as a result

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

unsigned long int avgValue;
int buf[10], buff[10], tmp;
float phValue;

float temp;

int sensorValue;
float voltage;
float mapV;
float ntu;
float turbi;

int timezone = 7 * 3600;  // ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;  // กำหนดค่า Date Swing Time

struct tm* p_tm;
int num_min;
int num_sec;

void setup(void)
{

  // start serial port
  Serial.begin(115200);
  // Start up the library
  Blynk.begin(auth, ssid, pass);
  Blynk.connect(3333);  // timeout set to 10 seconds and then continue without Blynk
  while (Blynk.connect() == false) {
    // Wait until connected
    delay(10);
  }
  timer.setInterval(30000L, blynk_write); // check if still connected every 15 second
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov"); // ดึงเวลาจาก NTP Server
  sensors.begin();
  Wire.begin(4, 5);
  ads1115.setGain(GAIN_ONE); // 1x gain   +/- 4.096V  1 bit = 0.125mV // การตั้งค่า Gain = 1x หรือช่วงวัด +/- 4.096V ความละเอียด 0.125mV/bit
  ads1115.begin();
  ESP.wdtDisable();
  ESP.wdtEnable(WDTO_8S);

}

void CheckConnection() {
  Connected2Blynk = Blynk.connected();
  if (!Connected2Blynk) {
    Blynk.connect(3333);  // timeout set to 10 seconds and then continue without Blynk
  }
}

void blynk_write()
{
  pH();
  temperature();
  turbidity();
  thingspeak();
  CheckConnection();
  Blynk.virtualWrite(V1, phValue);
  delay(20);
  Blynk.virtualWrite(V2, temp);
  delay(20);
  Blynk.virtualWrite(V3, turbi);
  delay(20);
  ESP.wdtFeed();
}

void loop()
{
  if (Connected2Blynk) {
    Blynk.run();
  }
  timer.run();
}

void pH()
{
  for (int i = 0; i < 10; i++)              // Get 10 sample value from the sensor for smooth the value
  {
    adc3 = ads1115.readADC_SingleEnded(3);  // address GND (0x048) pin 3
    buf[i] = adc3;
    delay(10);
  }

  for (int i = 0; i < 9; i++)               // sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        tmp = buf[i];
        buf[i] = buf[j];
        buf[j] = tmp;
      }
    }
  }

  avgValue = 0;
  for (int i = 2; i < 8; i++)                           // take the average value of 6 center sample
    avgValue += buf[i];
  phValue = ((float)avgValue * (3.3 / 21500.0)) / 6;    // convert the analog into millivolt
  phValue = 3.5 * phValue;                              // convert the millivolt into pH value

  if (phValue < 5.0)
  {
    String txt = "pH: " + (String)phValue + " is Acidic.";
    Line_Notify(LINE_TOKEN, txt);
  }
  else if (phValue > 9.0)
  {
    String txt = "pH: " + (String)phValue + " is Alkaline.";
    Line_Notify(LINE_TOKEN, txt);
  }
}

void temperature()
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus

  sensors.requestTemperatures();    // Send the command to get temperature readings

  temp = sensors.getTempCByIndex(0);

  // You can have more than one DS18B20 on the same bus.
  // 0 refers to the first IC on the wire

  if (temp < 23.0)
  {
    String txt = "Temp: " + (String)temp + " is Cool.";
    Line_Notify(LINE_TOKEN, txt);
  }
  else if (temp > 32.0)
  {
    String txt = "Temp: " + (String)temp + " is Hot.";
    Line_Notify(LINE_TOKEN, txt);
  }
}

void turbidity()
{
  for (int i = 0; i < 10; i++)              // Get 10 sample value from the sensor for smooth the value
  {
    buff[i] = analogRead(A0);                // read the input on analog pin 0:
    delay(10);
  }

  for (int i = 0; i < 9; i++)               // sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buff[i] > buff[j])
      {
        tmp = buff[i];
        buff[i] = buff[j];
        buff[j] = tmp;
      }
    }
  }

  sensorValue = 0;
  for (int i = 2; i < 8; i++)                      // take the average value of 6 center sample
    sensorValue += buff[i];
  voltage = (sensorValue * (5.0 / 1024.0)) / 6;
  mapV = mapf(voltage, 0, 5, 4.2, 2.5);
  ntu = (-1120.4 * pow(mapV, 2)) + (5742.3 * mapV) - 4352.9;
  turbi = mapf(ntu, 0, 3000, 0, 100);

  if (turbi >= 80)
  {
    String txt = "Turbidity: " + (String)turbi + " is Dirty.";
    Line_Notify(LINE_TOKEN, txt);
  }
}

void thingspeak()
{
  Msg_pH = (String)phValue;
  Msg_temp = (String)temp;   // เนื่องจาก temp เป็น float ซึ่งจะส่งค่าไม่ได้ เราจึกต้องแปลงเป็น String ก่อนค่ะ
  Msg_turb = (String)turbi;
  data = "field1=" + Msg_pH + "&field2=" + Msg_temp + "&field3=" + Msg_turb;   // ข้อมูล String ที่เราจะส่งค่าไปยัง ThingSpeak ค่ะ

  time_t now = time(nullptr);
  tm* p_tm = localtime(&now);       // ไปรับค่าเวลา
  num_min = p_tm->tm_min;           // เก็บค่านาทีมาไว้ในตัวแปร NUM
  num_sec = p_tm->tm_sec;           // เก็บค่าวินาทีมาไว้ในตัวแปร NUM

  if (num_min % 30 == 0)   // check time for minute equals 00 or 30
  {
    if (num_sec <= 60)     // check time for second less than 30
    {
      if (client.connect(thingSpeakAddress, 80))
      {
        client.print("POST /update HTTP/1.1\n");    // การส่งค่าข้อมูลผ่าน HTTP
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(data.length());
        client.print("\n\n");
        client.print(data);     // path ข้อมูลที่เราจะส่งค่าไปยัง ThingSpeak
      }
    }
  }
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
  req += "Connection: close\r\n";
  req += "Authorization: Bearer " + String(LINE_Token) + "\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(msg.length()) + "\r\n";
  req += "\r\n";
  req += msg;

  lineclient.print(req);

  delay(20);

  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      break;
    }
  }
}

float mapf(float val, float in_min, float in_max, float out_min, float out_max)
{
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
