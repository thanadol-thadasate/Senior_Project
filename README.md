Senior Project (Srinakharinwirot University, The Faculty of Science, Computer Science)

Name: A Development of Water Quality Monitoring System Using IoT (การพัฒนาระบบตรวจสอบคุณภาพน้ำด้วยเทคโนโลยี IoT)

Purposes of Research (วัตถุประสงค์)
1. เพื่อให้ประชาชนสามารถตรวจสอบคุณภาพน้ำในแหล่งน้ำบริเวณที่พักอาศัยของตนได้ด้วยตัวเอง 
2. พัฒนาระบบตรวจสอบคุณภาพน้ำที่ส่งข้อมูลแบบไร้สายโดยใช้เทคโนโลยี IoT
3. เพื่อเรียนรู้การทำงานของ pH Sensor, Temperature Sensor, Turbidity Sensor และไมโครคอนโทรลเลอร์ NodeMCU

Scope of Research (ขอบเขตของงานวิจัย)
1. สามารถตรวจสอบค่าความเป็นกรด-เบส อุณหภูมิ และความขุ่นของน้ำได้
2. เก็บรวบรวมข้อมูลที่ได้จากการตรวจสอบคุณภาพน้ำไว้ที่ Cloud
3. แสดงผลการตรวจสอบแบบ Real-Time ไปยังโทรศัพท์มือถือผ่านแอปพลิเคชัน Blynk
4. ทำการแจ้งเตือนผ่านบริการ Line Notify เมื่อพบดัชนีค่าความบริสุทธิ์ของน้ำมีความผิดปกติ

#HardWare

Controller
- NodeMCU Dev kit V.1 (Ref: http://thaieasyelec.com/dev-board/nodemcu-development-kit-v2-detail.html)

Sensors
- Analog pH Meter Pro (Ref: http://thaieasyelec.com/products/sensors/ph-d-o-orp/analog-ph-meter-pro-detail.html)
- Temperature Sensor - Waterproof (DS18B20) [เซนเซอร์ตรวจวัดอุณหภูมิในน้ำ] (Ref: http://thaieasyelec.com/products/sensors/humidity-temperature/temperature-sensor-waterproof-ds18b20-detail.html)
- Gravity: Analog Turbidity Sensor [เซนเซอร์ตรวจวัดความขุ่น] (Ref: http://thaieasyelec.com/products/sensors/ph-d-o-orp/gravity-analog-turbidity-sensor-detail.html)
- ADS1115 16-Bit ADC Module with Programmable Gain Amplifier (Ref: http://thaieasyelec.com/products/interface-modules/adc/ads1115-16-bit-adc-module-with-programmable-gain-amplifier-detail.html)

#SoftWare

IDE
- Arduino IDE 1.8.5 (Ref: https://www.arduino.cc/en/Main/Software)

Cloud
- ThingSpeak (Ref: https://medium.com/@mintkhwan/iot-%E0%B8%AA%E0%B9%88%E0%B8%87%E0%B8%82%E0%B9%89%E0%B8%AD%E0%B8%A1%E0%B8%B9%E0%B8%A5%E0%B8%88%E0%B8%B2%E0%B8%81-nodemcu-esp8266-%E0%B9%84%E0%B8%9B%E0%B8%A2%E0%B8%B1%E0%B8%87-thingspeak-951348531c5f)

Notify
- Line Notify (Ref: https://www.ioxhop.com/article/47/esp8266-esp8285-%E0%B8%81%E0%B8%B1%E0%B8%9A%E0%B8%81%E0%B8%B2%E0%B8%A3%E0%B8%AA%E0%B9%88%E0%B8%87%E0%B8%81%E0%B8%B2%E0%B8%A3%E0%B9%81%E0%B8%88%E0%B9%89%E0%B8%87%E0%B9%80%E0%B8%95%E0%B8%B7%E0%B8%AD%E0%B8%99%E0%B9%80%E0%B8%82%E0%B9%89%E0%B8%B2-line)

Visualize
- Blynk (Mobile Application) (Ref: https://www.youtube.com/watch?v=7XmLW5oGm0Q&t=33s&list=PL6lE9Vvf1aWaYlskv06r59tEHSopF70M0&index=1)
