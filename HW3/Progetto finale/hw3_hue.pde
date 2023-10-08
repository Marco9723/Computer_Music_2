
import oscP5.*;
import netP5.*;
import java.io.File;
import controlP5.*;

//network variables
OscP5 oscP5;
NetAddress myRemoteLocation;

//chords array initialization
String[] chords = {"-69addmaj7", "-6addmaj7", "-6addmaj7", "-6addmaj7", "-69", "-maj7add9",

"-7add9", "-7add11", "-7add9add11", "-7add9", "-7add9", "-7add9",

"sus4maj7add9add13", "maj7add4add13", "7sus4add9add13", "7sus4add9", "-7add11", "-7add11add#5",

"7add9", "7add13", "7add13", "7add9", "7add913", "7add9",

"maj69", "maj69", "maj79", "maj769", "maj69", "maj79",

"sus67", "susmaj769", "sus69", "maj769", "maj79", "maj769",

"maj79#11", "maj69#11", "maj79#11", "maj79#11", "maj79#11", "maj69#11"
};
  
//image variables
PFont font;
ControlP5 cp5;
PImage img;
color c, textColor, backgroundColour = color(30,90,200);
float hue, sat, bri;
int winWidth = 1400, text_X = 910;
//new image choosen is used to print the image only once
//first click keeps track of the first time the user clicks the image, used to set ADSR parameters
//mouseclick keeps track if the mouse is pressed or not, used to trigger ADS or Release stages
boolean newImageChoosen = false, firstClick = true, mouseclick = false;

//music variables
int chord = 0, noteIndex = 0, prevNoteIndex = 0;
int startNote = 45, noteRange = 12, chordRange = 42, note = 40;
String[] stringNotes = {"A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab"};
int[] notes = {};
int index = 0;

//slider variables
Knob StartVal,attKnob,decKnob,susKnob,relKnob,freqCutKnob,slideKnob;
float peak, att, dec, sus, rel, freqCut, slideDur;
int ADSR_x = 910;
int ADSR_dist = 100;

void setup()
{
  size(1410, 600);
  background(backgroundColour);
  textSize(20);
  text("Choose an image:", text_X, 30);
  text("Root note:", text_X, 130);
  text("Current chord:", text_X, 230);
  //--------------------------------------------
  text("ADSR parameters:", text_X, 330);
  text("LPF", text_X, 480);
  text("Chord change slide", text_X+150, 480);
  
  frameRate(125);
  
  font = createFont("calibri", 20);
  //initialization of select image button
  cp5 = new ControlP5(this);
  cp5.addButton("Select")     //"Turn On" is the name of button
  .setPosition(910, 50) //x and y coordinates of upper left corner of button
  .setSize(120, 50)  //(width, height)
  .setFont(font)
  ;
  
  //adding knobs
  addKnob("peak", ADSR_x, 340, 1, 1);
  addKnob("att", ADSR_x+ADSR_dist, 340, 3, 0.5);
  addKnob("dec", ADSR_x+ADSR_dist*2, 340, 5, 0);
  addKnob("sus", ADSR_x+ADSR_dist*3, 340, 1, 1);
  addKnob("rel", ADSR_x+ADSR_dist*4, 340, 5, 0.5);
  addKnob("freqCut", ADSR_x, 490, 10000, 5000);
  addKnob("slideDur", ADSR_x+200, 490, 3, 0.3);
  
  //blue square containing current color
  fill(0,40,100); 
  rect(winWidth-80-50-10, 30-10, 100+20, 100+20);
  
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this,12000);
  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application
   */
  myRemoteLocation = new NetAddress("127.0.0.1",57120);
  
  //initialization of midi note values array from 45 to 56
  for (int i = startNote; i < (noteRange+startNote); i++) {
    notes = append(notes, i);
    index++;
  }
}

void draw()
{
  //drawing the choosen image
  if (img != null && newImageChoosen)
  {
    //resizing image in a way that does not compromise proportions
    //1.5 = 900/600, the proportion of space reserved for the image
    if ((float)img.width/img.height < 1.5){
      img.resize(0,600);
    } else {
      img.resize(900,0);
    }
    //rectangle that covers previously drawn images
    fill(backgroundColour);
    noStroke();
    rect(0, 0, 900, 600);
    
    //displaying the image choosen
    image(img, 0, 0);
    //boolean apt to call these funtions only once an image is choosen
    newImageChoosen = false;
  }
  
  OscMessage myMessage = new OscMessage("/color");
  myMessage.add(mouseclick);
  
  if(mousePressed && img != null && mouseX <= img.width && mouseY <= img.height ){
  mouseclick = true;
  
  // get the color of the image in position mouseX and mouseY
  c = img.get(mouseX,mouseY);
  hue = hue(c);
  sat = (1-saturation(c)/256)*0.5; //low sat = high reverb, high sat = clean sound
  bri = brightness(c)/256;
  
  chord = round((1 / (1 + exp(-5*(bri - 0.5)))) *(chordRange-1));
  
  if(hue <= 171){
    noteIndex = round(((hue+84)/256)*(noteRange-1));
  } else if (hue > 171) {
    noteIndex = round(((hue-171)/256)*(noteRange-1));
  }
  note = notes[noteIndex];
      
  myMessage.add(firstClick);
  myMessage.add(sat);
  myMessage.add(note);
  myMessage.add(chord);
  
  //sending ADSR parameters only once per mouse-click
  if(firstClick){
  //knob values
  myMessage.add(peak);
  myMessage.add(att);
  myMessage.add(dec);
  myMessage.add(sus);
  myMessage.add(rel);
  myMessage.add(freqCut);
  myMessage.add(slideDur);
  firstClick = false;
  }
  
  //text background
  fill(backgroundColour);
  noStroke();
  rect(905, 145, 100, 45);
  rect(905, 245, 500, 50);
  
  textSize(45);
  fill(255);
  //current note text
  text(stringNotes[noteIndex], 910, 185);
  //current chord text
  text(chords[chord], 910, 285);

  //colour selected
  fill(c);
  rect(winWidth-80-50, 30, 100, 100);
  }
  oscP5.send(myMessage, myRemoteLocation);
}

void Select(){
  selectInput( "Select an image", "fileSelected");
}

void fileSelected(File selection) {
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
  } else {
    println("User selected " + selection.getAbsolutePath());
    img = loadImage(selection.getAbsolutePath()); 
    newImageChoosen = true;
  }
}

void addKnob(String name, int x, int y, int maxRange, float startValue)
{
   slideKnob = cp5.addKnob(name)
   .setPosition(x, y)
   .setSize(20, 20)
   .setRange(0, maxRange)
   .setRadius(45)
   .setValue(startValue); 
}

void mouseReleased(){
  //restoring first click boolean
  firstClick = true;
  //mouse released
  mouseclick = false;
}
