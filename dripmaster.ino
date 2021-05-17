//runs all the time, plugged in


const int MOISTURE_SENSOR_0_PIN = A0;
const int MOISTURE_SENSOR_1_PIN = A1;
const int MOISTURE_SENSOR_2_PIN = A2;
const int MOISTURE_SENSOR_3_PIN = A3;


const int SOLENOID_0_PIN = 0;
const int SOLENOID_1_PIN = 1;
const int SOLENOID_2_PIN = 14;
const int SOLENOID_3_PIN = 15;

const int GROWTH_CYCLE_PLANTING_TOGGLE_SWITCH_PIN = 16;
const int GROWTH_CYCLE_VEG_TOGGLE_SWITCH_PIN = 17;
const int GROWTH_CYCLE_EARLY_TOGGLE_SWITCH_PIN = 18;
const int GROWTH_CYCLE_MATURE_TOGGLE_SWITCH_PIN = 17;

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


  pinMode(MOISTURE_SENSOR_0_PIN, INPUT);
  pinMode(MOISTURE_SENSOR_1_PIN, INPUT);
  pinMode(MOISTURE_SENSOR_2_PIN, INPUT);
  pinMode(MOISTURE_SENSOR_3_PIN, INPUT);

  pinMode(SOLENOID_0_PIN, OUTPUT);
  pinMode(SOLENOID_1_PIN, OUTPUT);
  pinMode(SOLENOID_2_PIN, OUTPUT);
  pinMode(SOLENOID_3_PIN, OUTPUT);

  pinMode(GROWTH_CYCLE_PLANTING_TOGGLE_SWITCH_PIN, INPUT);
  pinMode(GROWTH_CYCLE_VEG_TOGGLE_SWITCH_PIN, INPUT);
  pinMode(GROWTH_CYCLE_EARLY_TOGGLE_SWITCH_PIN, INPUT);
  pinMode(GROWTH_CYCLE_MATURE_TOGGLE_SWITCH_PIN, INPUT);



  //set the output pins for the solenoids to LOW (meaning off) to start with
  digitalWrite(SOLENOID_0_PIN, LOW);
  digitalWrite(SOLENOID_1_PIN, LOW);
  digitalWrite(SOLENOID_2_PIN, LOW);
  digitalWrite(SOLENOID_3_PIN, LOW);

  //in the arduino circuit, use a 10k pull down resistors for the solenoid pins, if you dont then if you turn it on and off, turning off does not make it 0, it may have residual voltage, so this resistor makes sure it comes down 0.

}



void loop() {


  if ( GROWTH_CYCLE_PLANTING_TOGGLE_SWITCH_PIN == HIGH ) { //switch turned on
    controlSolenoids( dry_value_for_growth_cycle_planting, wet_value_for_growth_cycle_planting);
  }

  if ( GROWTH_CYCLE_VEG_TOGGLE_SWITCH_PIN == HIGH ) {
    controlSolenoids( dry_value_for_growth_cycle_vegetative, wet_value_for_growth_cycle_vegetative);
  }


  if ( GROWTH_CYCLE_EARLY_TOGGLE_SWITCH_PIN == HIGH ) {
    controlSolenoids( dry_value_for_growth_cycle_early, wet_value_for_growth_cycle_early);
  }


  if ( GROWTH_CYCLE_MATURE_TOGGLE_SWITCH_PIN == HIGH ) {
    controlSolenoids( dry_value_for_growth_cycle_mature, wet_value_for_growth_cycle_mature);
  }


  delay(100);
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
