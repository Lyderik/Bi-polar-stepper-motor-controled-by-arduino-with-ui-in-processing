//The pin configuration between the Arduino and the board.
int a = 10;
int b = 11;
int ai = 12;
int bi = 13;

//The delay between half-steps in microseconds.
int d = 500;
//The current positionstate of the motor, in half-stepping it's from 1-8.
int state = 8;

//A method which stops the motor by turning all outputs low, which means the motor will have have no power to hold its current position.
void motorOff(){
  digitalWrite(a, LOW);
  digitalWrite(ai, LOW);
  digitalWrite(b, LOW);
  digitalWrite(bi, LOW);
}
/******* A set of methods which is all 8 states of half-stepping *******/
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
//Arduinos setup, initialize the output pins.
void setup(){
  pinMode(a, OUTPUT); 
  pinMode(b, OUTPUT); 
  pinMode(ai, OUTPUT); 
  pinMode(bi, OUTPUT);
}

//This loop runs over and over again forever and it's in here you write your program to control the motor.
void loop() {
  /***** Example code *****/

  mDoSteps(100); // Move motor 100 half-steps.
  /*** 
  	  This is a 1 second delay with a motorOff() in it, the reason way you do not turn the motor off right after you doing steps, 
  	  is that it got moment of inertia and will continue moving a uncontrolled few steps more than commanded. 
	    In general it is not good idea to turn off the motor, but a voltage regulator on the motor would be required due to the impedance in the motor if not turned off.
  ***/
  delay(50);
  motorOff();
  delay(950);

  mDoSteps(-200);// Move motor -200 half-steps.
  delay(50);  //
  motorOff(); // Delay 1 second.
  delay(450); //

  /***** End of example code *****/

}
