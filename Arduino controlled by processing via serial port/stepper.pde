import processing.serial.*; //Import proper library.

Serial myPort;     //Define port object.
PFont f;           //Define font to gain text later on.
String val = "";   //Define a variable to hold reads from serial.

/** Runs one time as the progam initialize. **/
void setup() 
{
  size(200,200);                             //Make a 200 x 200 pixels big window.
  String portName = Serial.list()[1];        //Change the 0 to a 1 or 2 etc. to match your port.
  myPort = new Serial(this, portName, 9600); //Initialize the serial connection.
  myPort.bufferUntil('\n');                  //Wait for response on the conection.
  f = createFont("Arial",16,true);           //Initialize the font.
}
/** **/
void draw() {
  if ( myPort.available() > 0)               // If data is available.
  {  
    val = ""+myPort.readStringUntil('\n');   // Read it and store the data in val.
  }

  background(255);  //Set backgroundcolor.
  textFont(f,16);   //Set current font.
  fill(0);          //Set current fill property.
  text(val,10,100); //Display the input data from the serial connection.
  
  if(keyPressed == true){           //If a key is pressed.
    if (key == CODED) {
      if (keyCode == LEFT) {        //If the key is left arrow key,
        myPort.write('L');          //sent an L to via connection.
      }
      else if (keyCode == RIGHT) {  //If the key is right arrow key,
        myPort.write('R');          //sent an R to via connection.
      }
      else if (keyCode == UP) {     //If the key is up arrow key,
        myPort.write('U');          //sent an U to via connection.
        delay(100);                 //Delay 100ms to make a keyhold feel controlable for user.
      }
      else if (keyCode == DOWN) {   //If the key is down arrow key,
        myPort.write('D');          //sent an D to via connection.
        delay(100);                 //Delay 100ms to make a keyhold feel controlable for user.
      }
    }                               //You can at other keylisteners as so.
  }
}