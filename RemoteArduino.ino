/**
This is dependent on the SimpleMessageSystem library, available 
at http://playground.arduino.cc/Code/SimpleMessageSystem
*/

#include <SimpleMessageSystem.h> 

void setup()
{
  // The following command initiates the serial port at 9600 baud. Please note this is VERY SLOW!!!!!! 
  // I suggest you use higher speeds in your own code. You can go up to 115200 with the USB version, that's 12x faster
  pinMode(13, OUTPUT);
  Serial.begin(115200); //Baud set at 9600 for compatibility, CHANGE!
  
}

void loop()
{
  if (messageBuild() > 0) { // Checks to see if the message is complete and erases any previous messages
    switch (messageGetChar()) { // Gets the first word as a character
    case 'r': // Read pins (analog or digital)
      readpins(); // Call the readpins function
      break; // Break from the switch
    case 'w': // Write pin
      writepin(); // Call the writepin function
      break;
    case 'm': // Set pin mode
      setmode();
      break;
    }

  }

}

void readpins(){ // Read pins (analog or digital)

  switch (messageGetChar()) { // Gets the next word as a character

    case 'd': // READ digital pins

    messageSendChar('d');  // Echo what is being read
    for (char i=2;i<14;i++) {
      messageSendInt(digitalRead(i)); // Read pins 2 to 13
    }
    messageEnd(); // Terminate the message being sent
    break; // Break from the switch

  case 'a': // READ analog pins

    messageSendChar('a');  // Echo what is being read
    for (char i=0;i<6;i++) {
      messageSendInt(analogRead(i)); // Read pins 0 to 5
    }
    messageEnd(); // Terminate the message being sent

  }

}

void writepin() { // Write pin

  int pin;
  int state;

  switch (messageGetChar()) { // Gets the next word as a character

    case 'a' : // WRITE an analog pin

    pin = messageGetInt(); // Gets the next word as an integer
    state = messageGetInt(); // Gets the next word as an integer
    analogWrite(pin, state); //Sets the PWM of the pin 
    break;  // Break from the switch


    // WRITE a digital pin
  case 'd' : 
    
    pin = messageGetInt();  // Gets the next word as an integer
    state = messageGetInt();  // Gets the next word as an integer
    digitalWrite(pin,state);  //Sets the state of the pin HIGH (1) or LOW (0)
    

  }

}

void setmode() { // Set mode of pin

  int pin;

  pin = messageGetInt(); // Gets the next word as an integer
  
  switch (messageGetChar()) { // Gets the next word as a character

    case 'o' : // Set to be an output pin
    pinMode(pin, OUTPUT);
    break;
    
    case 'i' : // Set to be an input pin
    pinMode(pin, INPUT);
    break;

    case 'p' : // Set to be a pullup (input) pin
    pinMode(pin, INPUT_PULLUP);
    break;
    
    default:
    digitalWrite(13, HIGH);
    break;
  }

}


