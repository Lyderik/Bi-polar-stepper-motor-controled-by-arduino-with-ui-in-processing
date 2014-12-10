//The pin configuration between the Arduino and the board.
int a = 10;
int b = 11;
int ai = 12;
int bi = 13;

//The delay between half-steps in microseconds.
int d = 500;
//The current positionstate of the motor, in half-stepping it's from 1-8.
int state = 8;

//The placeholder to store data received from serial readings.
char val; 

//A count value to control motorstaps, should be set after every motor movement action.
int lastAction = 0;

//A method ment to be called in the initialization of the program. It repeatedly sent out the current set steps per seconds until it get a response.
void establishContact() {
  while (Serial.available() <= 0) {
  Serial.println(100000/d*0.8);  
  delay(300);
  }
}
//A method which stops the motor by turning all outputs low, which means the motor will have have no power to hold its current position.
void motorOff(){
  digitalWrite(a, LOW);
  digitalWrite(ai, LOW);
  digitalWrite(b, LOW);
  digitalWrite(bi, LOW);
}
/**** A set of methods which is all 8 states of half-stepping ****/
void m1(){
  digitalWrite(ai, HIGH);
  delayMicroseconds(d);
  state = 1;
}
void m2(){
  digitalWrite(bi, LOW);
  delayMicroseconds(d);
  state = 2;
}
void m3(){
  digitalWrite(b, HIGH);
  delayMicroseconds(d);
  state = 3;
}
void m4(){
  digitalWrite(ai, LOW);
  delayMicroseconds(d);
  state = 4;
}
void m5(){
  digitalWrite(a, HIGH);
  delayMicroseconds(d);
  state = 5;
}
void m6(){
  digitalWrite(b, LOW);
  delayMicroseconds(d);
  state = 6;
}
void m7(){
  digitalWrite(bi, HIGH);
  delayMicroseconds(d);
  state = 7;
}
void m8(){
  digitalWrite(a, LOW);
  delayMicroseconds(d);
  state = 8;
}
//A method which checks the current state and move to the next one.
void mNextStep(){
  switch(state){
    case 1: m2(); break;
    case 2: m3(); break;
    case 3: m4(); break;
    case 4: m5(); break;
    case 5: m6(); break;
    case 6: m7(); break;
    case 7: m8(); break;
    case 8: m1(); break;
  };
}
//A method which checks the current state and moves to the previous step, relative to mNextStep().
void mPrewStep(){
  switch(state){
    case 1: m6(); break;
    case 2: m3(); break;
    case 3: m8(); break;
    case 4: m5(); break;
    case 5: m2(); break;
    case 6: m7(); break;
    case 7: m4(); break;
    case 8: m1(); break;
  };
}
//A method which checks the current state and moves to the previous step, relative to mNextStep()
void mDoSteps(int n){
  if(n > 0){
    for(int i=0; i < n; i++){
      mNextStep();
    }
  }else if(n < 0){
    for(int i=0; i > n; i--){ 
      mPrewStep();
    } 
  }
}
//Arduinos setup, initialize the output pins and serial port. Make sure there i contact before running the program.
void setup(){
  pinMode(a, OUTPUT); 
  pinMode(b, OUTPUT); 
  pinMode(ai, OUTPUT); 
  pinMode(bi, OUTPUT);
  Serial.begin(9600);
  establishContact();
}

//This loop runs over and over again forever and it's in here you write your program to control the motor.
void loop() {
  if (Serial.available())           // If data is available to read.
  {
    val = Serial.read();            // Read data and store it in val
   
    /*** Checks the data and do the what ever the data means ***/
    if (val == 'L'){
      mDoSteps((int)(18000/d));     //Magic calculation thats ensure a continuous movement of motor.
      lastAction = 1000;
    }else if(val == 'R'){
      mDoSteps(-(int)(18000/d));    //Magic calculation thats ensure a continuous movement of motor.
      lastAction = 1000;
    }else if(val == 'U'){
      if(d > 400) d = d-10;         //First checks if the speed limit is reach, and if not change the delay aka speed.
      delay(100);                   //Look in associated processing code to undertand this delay.
      Serial.println(1000000/(d*2)); //Sent the current steps per second via serial connection
    }else if(val == 'D'){
      if(d < 1000) d = d+10;        //First checks if the speed limit is reach, and if not change the delay aka speed.
      delay(100);                   //Look in associated processing code to undertand this delay.
      Serial.println(1000000/(d*2)); //Sent the current steps per second via serial connection
    }
  }
  /*** 
      This will result in a delay with a motorOff() after it unless a other motor action performed before the end of the delay, the reason way you do not turn the motor off right after you doing steps,
      is that it got moment of inertia and will continue moving a uncontrolled few steps more than commanded. 
      In general it is not good idea to turn off the motor, but a voltage regulator on the motor would be required due to the impedance in the motor if not turned off.
      lastAction must be set after every motor movement.
  ***/
  if(lastAction == 0){
    motorOff();
    lastAction--;
  }else if(lastAction > 0){
    lastAction--;
  }
}
