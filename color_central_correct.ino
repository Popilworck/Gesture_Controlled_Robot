
#include <ArduinoBLE.h>// imports the ble library required for communication through bluetooth
#include <Arduino_APDS9960.h>// imports the libraries required to effectively use the gesture sensor

const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";//sets device ids, which the peripheral looks for
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";
int op=-1;
int oop=900;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
  }
  if (!BLE.begin()) {// makes sure the bluetooth ble module is working
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setLocalName("Nano 33 BLE (Central)");// sets local name
  BLE.advertise();

  Serial.println("Arduino Nano 33 BLE Sense (Central Device)");
  Serial.println(" ");
}

void loop() {
  connectToPeripheral();
  // check if a color reading is available
}
void peepeepoopoo(){

}
int colordetection() {
  if (APDS.colorAvailable()) {// makes sure the gesture sensor is avaiable
    int r,g,b;
    APDS.readColor(r,g,b);// reads the data
    //Serial.println(r);
   /// Serial.println(g);
    //Serial.println(b);
    if (r>g && r>b && r>100){
      //Serial.println("RED");
      return(0);
    }
    else if(g>r && g>b && g>100){
      //Serial.println("GREEN");
      return(1);
    }
    else{
      return(8);
    }
  }
}


void connectToPeripheral() {
  BLEDevice peripheral;

  Serial.println("- Discovering peripheral device...");

  do {// searches for a peripheral device with a matching device service id
    BLE.scanForUuid(deviceServiceUuid);
    peripheral = BLE.available();
  } while (!peripheral);

  if (peripheral) {// if a peripheral device is found, it prints all its information
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();// stops looking for new devices
    controlPeripheral(peripheral);// calls the function control peripheral
  }
  // if the peripheral gets disconnected, it starts looking for one again
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("- Connecting to peripheral device...");

  if (peripheral.connect()) {// if connection is a success, it prints accordingly
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
  } else {// otherwise it terminates the function
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    return;
  }

  Serial.println("- Discovering peripheral device attributes...");// searches for peripheral device attributes
  if (peripheral.discoverAttributes()) {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");
  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect();
    return;
  }

  BLECharacteristic gestureCharacteristic = peripheral.characteristic(deviceServiceCharacteristicUuid);

  if (!gestureCharacteristic) {// checks if the peripheral has a writeable gesture characterestic
    Serial.println("* Peripheral device does not have gesture_type characteristic!");
    peripheral.disconnect();
    return;
  } else if (!gestureCharacteristic.canWrite()) {
    Serial.println("* Peripheral does not have a writable gesture_type characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {//while the peripheral is connected
    while (! APDS.colorAvailable()) {
      delay(5);}
    // Serial.println(op);
    //Serial.println(oop);
    op = colordetection();
    if (op!=oop){
      oop=op;
      //Serial.println(op);
      Serial.println("YES");
      Serial.print("* Writing value to color_type characteristic: ");
      Serial.println(op);
      gestureCharacteristic.writeValue((byte)op);
      Serial.println("* Writing value to color_type characteristic done!");
      Serial.println(" ");
      // wait a bit before reading again
      }
      delay(200);
  }
  Serial.println("- Peripheral device disconnected!");// prints this statement if the peripheral device gets disconnected
}
