
import ddf.minim.*;

import processing.serial.*;
import spacebrew.*;
//import processing.sound.*;

Minim minim;
AudioPlayer groove;

// de declaraties voor de kleuren van de schermweergave na het uitvoeren
color color_on = color(255, 255, 50);
color color_off = color(255, 255, 255);
int currentColor = color_off;


Serial myPort;                       // The serial port
int[] serialInArray = new int[1];    // Where we'll put what we receive
int serialCount = 0;                 // A count of how many bytes we receive             // Starting position of the ball
boolean firstContact = false;        // Whether we've heard from the microcontroller
boolean is_playing = false;

// verbinding met spacebrew
String server="54.93.57.201";
String name="Sander";
String description ="Client that sends and receives boolean messages. Background turns yellow when message received.";
// SoundFile file;
Spacebrew sb;

void setup() {
  size(500, 400);
  noStroke();      // No border on the next thing drawn

  // Print a list of the serial ports for debugging purposes
  // if using Processing 2.1 or later, use Serial.printArray()
  sb = new Spacebrew( this );
  println(Serial.list());

  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
  sb.addPublish( "button_pressed", "boolean", false );  
  sb.addSubscribe( "change_led", "boolean" );


  // sb.addSubscribe( "change_LED", "boolean" );
  // connect to spacebre<
  sb.connect(server, name, description );
  // file = new SoundFile(this, "C:\\Users\\Sander\\Documents\\++ School\\Public Play\\pvcsoundgame\\pvcsoundgame_v1\\testloop.wav");
  minim = new Minim(this);
  groove = minim.loadFile("C:\\Users\\Sander\\Documents\\++ School\\Public Play\\pvcsoundgame\\pvcsoundgame_v1\\gelach2.mp3", 2048);
}

void draw() {
  background(currentColor);
  fill(255, 0, 0);
  stroke(200, 0, 0);
  rectMode(CENTER);
  ellipse(width/2, height/2, 250, 250);
  fill(150);
  textAlign(CENTER);
  textSize(24);
}



void serialEvent(Serial myPort) {
  int inByte = myPort.read();
  myPort.write('A'); // maak een verbinding met arduino en stuur een letter A. Als arduino de letter A ontvangt controleert hij deze en is er een verbinding tot stand gekomen
  if (inByte == 255 ) {
    // myPort.write('G');
    sb.send( "button_pressed", true );
    println("sb.send: button pressed is true");
    //text("Click Me", width/2, height/2 + 12);
  } else {
    // myPort.write('R');
    sb.send( "button_pressed", false );
    println("sd.send: button pressed is false");
    //text("That Feels Good", width/2, height/2 + 12);
  }
}


void onBooleanMessage( String name, boolean value ) {
// als het liedje aan het spelen is, wacht met de if uit te voeren, pas vanaf het moment dat het liedje klaar is met spelen maar de if opnieuw uitgevoerd worden en zal het liedjes opnieuw afgespeeld worden. Hetzelfde voor de ledview.
  println("got bool message " + name + " : " + value); 
  if (name.equals("change_led") && is_playing == false) {
    println("got bool message " + name + " : " + value); 
    // update background color
    if (value == true) {

      if (!groove.isPlaying()) {
        currentColor = color_on; 
        myPort.write('P');
        groove.rewind();
        groove.play();
 
      }
    } else {
      currentColor = color_off;
      myPort.write('R');
    }
  }
}
