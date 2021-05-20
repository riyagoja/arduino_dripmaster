//runs all the time, plugged in
//wifi sheild

#include <SPI.h>
#include <WiFi.h>

char ssid[] = "yourNetwork"; //  your network SSID (name)
char pass[] = "secretPassword";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "api.dripmaster.com";    // name address for web server that has the information on the growth cycle (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;


//drip master controls
const int MOISTURE_SENSOR_0_PIN = A0;
const int MOISTURE_SENSOR_1_PIN = A1;
const int MOISTURE_SENSOR_2_PIN = A2;
const int MOISTURE_SENSOR_3_PIN = A3;


const int SOLENOID_0_PIN = 0;
const int SOLENOID_1_PIN = 1;
const int SOLENOID_2_PIN = 14;
const int SOLENOID_3_PIN = 15;

bool GROWTH_CYCLE_PLANTING = false;
bool GROWTH_CYCLE_VEG = false;
bool GROWTH_CYCLE_EARLY = false;
bool GROWTH_CYCLE_MATURE = false;

//wifi module code for uploading to cloud, for now im temporarily using toggle switches, we will have to get wifi shield for arduino later in order to get data from web app abt the growth cycles

//these values we will change based on when we find right research, these are random temporary values i set
const int dry_value_for_growth_cycle_planting = 1.0;
const int wet_value_for_growth_cycle_planting = 2.0;

const int dry_value_for_growth_cycle_vegetative = 1.5;
const int wet_value_for_growth_cycle_vegetative = 2.2;

const int dry_value_for_growth_cycle_early = 1.1;
const int wet_value_for_growth_cycle_early = 2.8;

const int dry_value_for_growth_cycle_mature = 0.9;
const int wet_value_for_growth_cycle_mature = 2.9;







void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // serial port setup

  initilizeWiFi();

  pinMode(MOISTURE_SENSOR_0_PIN, INPUT);
  pinMode(MOISTURE_SENSOR_1_PIN, INPUT);
  pinMode(MOISTURE_SENSOR_2_PIN, INPUT);
  pinMode(MOISTURE_SENSOR_3_PIN, INPUT);

  pinMode(SOLENOID_0_PIN, OUTPUT);
  pinMode(SOLENOID_1_PIN, OUTPUT);
  pinMode(SOLENOID_2_PIN, OUTPUT);
  pinMode(SOLENOID_3_PIN, OUTPUT);


  //set the output pins for the solenoids to LOW (meaning off) to start with
  digitalWrite(SOLENOID_0_PIN, LOW);
  digitalWrite(SOLENOID_1_PIN, LOW);
  digitalWrite(SOLENOID_2_PIN, LOW);
  digitalWrite(SOLENOID_3_PIN, LOW);

  //in the arduino circuit, use a 10k pull down resistors for the solenoid pins, if you dont then if you turn it on and off, turning off does not make it 0, it may have residual voltage, so this resistor makes sure it comes down 0.

}



void loop() {


  getWebServerResponse();

  if ( GROWTH_CYCLE_PLANTING  == true) { //switch turned on
    controlSolenoids( dry_value_for_growth_cycle_planting, wet_value_for_growth_cycle_planting);
  }

  if ( GROWTH_CYCLE_VEG  == true) {
    controlSolenoids( dry_value_for_growth_cycle_vegetative, wet_value_for_growth_cycle_vegetative);
  }


  if ( GROWTH_CYCLE_EARLY  == true) {
    controlSolenoids( dry_value_for_growth_cycle_early, wet_value_for_growth_cycle_early);
  }


  if ( GROWTH_CYCLE_MATURE == true ) {
    controlSolenoids( dry_value_for_growth_cycle_mature, wet_value_for_growth_cycle_mature);
  }


  delay(100);
}


void initilizeWiFi() {
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();

  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");

  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {

    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /growthStage");
    client.println("Host: api.dripmaster.com");
    client.println("Connection: close");
    client.println();
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void getWebServerResponse() {

  while (client.available()) {
    char c = client.read();
    Serial.write(c);

    GROWTH_CYCLE_PLANTING = false;
    GROWTH_CYCLE_VEG = false;
    GROWTH_CYCLE_EARLY = false;
    GROWTH_CYCLE_MATURE = false;


    if ( c == 'A') {
      GROWTH_CYCLE_PLANTING = true;
    }

    if ( c == 'B') {
      GROWTH_CYCLE_VEG = true;
    }

    if ( c == 'C') {
      GROWTH_CYCLE_MATURE = true;
    }


    if ( c == 'D') {
      GROWTH_CYCLE_EARLY = true;
    }
  }

  // if the server's disconnected, stop the client:

  if (!client.connected()) {

    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    // do nothing forevermore:
    while (true);
  }
}

void controlSolenoids(int dry_val, int wet_val) { //this function reads the four sensors in the irrigation line and controls the solenoids, stores them in each of the variables

  int soilMoistureValueAtLocationX1 = analogRead(MOISTURE_SENSOR_0_PIN); //arduino reads MOISTURE_SENSOR_0_PIN and stores value in soilMoistureValueAtLocationX1
  Serial.print("soil moisture level at locaion X1 = "); Serial.println(soilMoistureValueAtLocationX1);

  int soilMoistureValueAtLocationX2 = analogRead(MOISTURE_SENSOR_1_PIN);
  Serial.print("soil moisture level at locaion X2 = "); Serial.println(soilMoistureValueAtLocationX1);

  int soilMoistureValueAtLocationX3 = analogRead(MOISTURE_SENSOR_2_PIN);
  Serial.print("soil moisture level at locaion X3 = "); Serial.println(soilMoistureValueAtLocationX1);

  int soilMoistureValueAtLocationX4 = analogRead(MOISTURE_SENSOR_3_PIN);
  Serial.print("soil moisture level at locaion X4 = "); Serial.println(soilMoistureValueAtLocationX1);




  //X1 region
  if ( soilMoistureValueAtLocationX1 <= dry_val) {
    digitalWrite(SOLENOID_0_PIN, HIGH); // turn on the soleniod at location X1
  }

  if ( soilMoistureValueAtLocationX1 >= wet_val) {

    digitalWrite(SOLENOID_0_PIN, LOW); // turn off the soleniod at location X1
  }

  //X2 region
  if ( soilMoistureValueAtLocationX2 <= dry_val) {
    digitalWrite(SOLENOID_1_PIN, HIGH); // turn on the soleniod at location X2
  }

  if ( soilMoistureValueAtLocationX2 >= wet_val) {

    digitalWrite(SOLENOID_1_PIN, LOW); // turn off the soleniod at location X2
  }

  //X3 region
  if ( soilMoistureValueAtLocationX3 <= dry_val) {
    digitalWrite(SOLENOID_2_PIN, HIGH); // turn on the soleniod at location X3
  }

  if ( soilMoistureValueAtLocationX3 >= wet_val) {

    digitalWrite(SOLENOID_2_PIN, LOW); // turn off the soleniod at location X3
  }


  //X4 region
  if ( soilMoistureValueAtLocationX4 <= dry_val) {
    digitalWrite(SOLENOID_3_PIN, HIGH); // turn on the soleniod at location X4
  }

  if ( soilMoistureValueAtLocationX4 >= wet_val) {

    digitalWrite(SOLENOID_3_PIN, LOW); // turn off the soleniod at location X4
  }


}


//what is threshold for dry and wet, determange range of numbersn a drange of dry and wet
//#16 shows dry and wet for corn, second link, based on table

//Specifications for sensors:

//Operating Voltage: DC 3.3 - 5.5V
//Output Voltage: DC 0 - 3.0V
//Interface: PH2.0 - 3P
//Size: 99x16mm / 3.9x0.63"
//Quantity: 1 Set
