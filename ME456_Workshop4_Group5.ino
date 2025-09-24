#include <MeMCore.h>

MeUltrasonicSensor ultrasonic(PORT_4);
MeLineFollower lineFinder(PORT_3);
MeDCMotor motorLeft(M1);
MeDCMotor motorRight(M2);

bool saldiriModu = false;
unsigned long saldiriBaslangic = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int distance = ultrasonic.distanceCm();
  int lineSensor = lineFinder.readSensors();
  unsigned long simdikiZaman = millis();

  Serial.print("Mesafe (cm): ");
  Serial.print(distance);
  Serial.print("  |  Çizgi sensörü: ");
  Serial.print(lineSensor);
  Serial.print("  |  Saldırı modu: ");
  Serial.println(saldiriModu);

  // 1. Byeazdan kaç abi
  if (lineSensor == 3) {
    saldiriModu = false;

    motorLeft.run(150);     // geri
    motorRight.run(-150);
    delay(1100);

    motorLeft.run(150);     // dönüş
    motorRight.run(150);
    delay(1200);

    return;
  }

  // 2. Saldırıya başla
  if (!saldiriModu && distance > 0 && distance < 55) {
    saldiriModu = true;
    saldiriBaslangic = simdikiZaman;
  }

  // Saldırıya devam
  if (saldiriModu && (simdikiZaman - saldiriBaslangic < 2000)) {
    motorLeft.run(-150);   // ileri saldırı
    motorRight.run(150);
    return;
  }

  // 4. Saldırı Countdown
  saldiriModu = false;
  motorLeft.run(150);     // dönüş
  motorRight.run(150);
}
