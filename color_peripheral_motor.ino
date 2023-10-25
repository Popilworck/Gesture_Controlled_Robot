

#include <ArduinoBLE.h>

int in1A = 12;
int in2A = 11;
int in1B = 10;
int in2B = 9;
const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";
int gesture = -1;
int LEDR = 4;
int LEDG =3;
int LEDB = 2;
BLEService gestureService(deviceServiceUuid); 
BLEByteCharacteristic gestureCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite);


void setup() {
  //pinMode(pwmA, OUTPUT);
  //pinMode(pwmB, OUTPUT);
  pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  pinMode(in1B, OUTPUT);
  pinMode(in2B, OUTPUT);
  Serial.begin(9600);
  while (!Serial);  
  
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  /*digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  digitalWrite(LED_BUILTIN, LOW);*/

  
  if (!BLE.begin()) {
    Serial.println("- Starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setLocalName("Arduino Nano 33 BLE (Peripheral)");
  BLE.setAdvertisedService(gestureService);
  gestureService.addCharacteristic(gestureCharacteristic);
  BLE.addService(gestureService);
  gestureCharacteristic.writeValue(-1);
  BLE.advertise();

  Serial.println("Nano 33 BLE (Peripheral Device)");
  Serial.println(" ");
}

void loop() {
  BLEDevice central = BLE.central();
  Serial.println("- Discovering central device...");
  delay(1000);

  if (central) {
    Serial.println("* Connected to central device!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    while (central.connected()) {
      if (gestureCharacteristic.written()) {
         gesture = gestureCharacteristic.value();
         writeGesture(gesture);
       }
    }
    
    Serial.println("* Disconnected to central device!");
  }
}

void writeGesture(int gesture) {
  Serial.println("- Characteristic <gesture_type> has changed!");
  
   switch (gesture) {
      case 0:
        Serial.println("* Actual value: STOP (red LED on)");
        Serial.println(" ");
        //led1(255,0,0);
        stop();
        break;
      case 1:
        Serial.println("* Actual value: GO (green LED on)");
        Serial.println(" ");
        //led1(0,255,0);
        fwd();
        break;
      case 3:
        Serial.println("* Actual value: LEFT (blue LED on)");
        Serial.println(" ");
        //led1(0,0,255);
        left();
        break;
      case 4:
        Serial.println("* Actual value: RIGHT (built-in LED on)");
        Serial.println(" ");
        //led1(0,255,255);
        right();
        break;
      case 5:
        Serial.println("* Actual value: None (LED off)");
        Serial.println(" ");
        //led1(0,0,0);
        stop();
      default:
        //stop();
        break;
    }      
}
void fwd(){
  digitalWrite(in1A,HIGH);
  digitalWrite(in2A,LOW);
  digitalWrite(in1B,HIGH);
  digitalWrite(in2B,LOW);
  //analogWrite(pwmA,225);
  //analogWrite(pwmB,255);
  Serial.println("FORWARD A");
}

void bkd(){
  
  digitalWrite(in1A,LOW);
  digitalWrite(in2A,HIGH);
  digitalWrite(in1B,LOW);
  digitalWrite(in2B,HIGH);
  //analogWrite(pwmA,225);
  //analogWrite(pwmB,255);
  Serial.println("Backward A");

}
void right(){
  digitalWrite(in1A,LOW);
  digitalWrite(in2A,HIGH);
  digitalWrite(in1B,HIGH);
  digitalWrite(in2B,LOW);
  //analogWrite(pwmA,225);
  //analogWrite(pwmB,255);
  Serial.println("RIGHT A");

}
void left(){
  digitalWrite(in1A,HIGH);
  digitalWrite(in2A,LOW);
  digitalWrite(in1B,LOW);
  digitalWrite(in2B,HIGH);
  //analogWrite(pwmA,225);
  //analogWrite(pwmB,255);
  Serial.println("left A");

}

void stop(){
  digitalWrite(in1A,LOW);
  digitalWrite(in2A,LOW);
  digitalWrite(in1B,LOW);
  digitalWrite(in2B,LOW);
 // analogWrite(12,200);
  Serial.println("Stopped");
}
void led1(int a1,int a2,int a3){
  analogWrite(LEDR,a1);
  analogWrite(LEDG,a2);
  analogWrite(LEDB,a3);
}