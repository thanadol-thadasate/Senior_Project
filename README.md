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

#SoftWare

IDE
- Arduino IDE

Cloud
- ThingSpeak

Notify
- Line Notify

Visualize
- Blynk (Mobile Application)
