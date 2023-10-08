
import oscP5.*;
import netP5.*;
import java.io.File;
import controlP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

PFont font;
ControlP5 cp5;
// PImage is a dataype for storing images; it contains field for the width and the height 
// also an array of pixels
PImage img;
color c;
float hue, sat, bri;
int mode, prevMode, noteIndex, prevNoteIndex;
char[] notes = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
String[] chords = {
"-69addmaj7",
"-6addmaj7",
"-6addmaj7",
"-6addmaj7",
"-69",
"-maj7add9",

"-7add9",
"-7add11",
"-7add9add11",
"-7add9",
"-7add9",
"-7add9",

"sus4maj7add9add13",
"maj7add4add13",
"7sus4add9add13",
"7sus4add9",
"-7add11",
"-7add11add#5",

"7add9",
"7add13",
"7add13",
"7add9",
"7add913",
"7add9",

"maj69",
"maj69",
"maj79",
"maj769",
"maj69",
"maj79",

"sus67",
"susmaj769",
"sus69",
"maj769",
"maj79",
"maj769",

"maj79#11",
"maj69#11",
"maj79#11",
"maj79#11",
"maj79#11",
"maj69#11"
  };
  
  Knob StartVal,attKnob,decKnob,susKnob,relKnob,freqCutKnob,slideKnob;
  float SVal, att,dec,sus,rel,freqCut,slideDur;
  int ADSR_x = 910;
  int ADSR_dist =60;
void setup()
{
  size(1200, 600);
  background(51);
  textSize(20);
  text("Choose another image:", 910, 30);
  text("Root note:", 910, 130);
  text("Current chord:", 910, 230);
  text("StartVal-ADSR:", 910, 330);
  text("Cutoff", 910, 480);
  text("Sliding Fac", 910+150, 480);
  
  // this function will make the user select an image in the object img
  selectInput( "Select an image", "imageChosen");
  frameRate(125);
  
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this,12000);
  
  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application
   */
  myRemoteLocation = new NetAddress("127.0.0.1",57120);
  
  font = createFont("calibri", 20);    // custom fonts for buttons and title
  
  cp5 = new ControlP5(this);
  cp5.addButton("Select")     //"Turn On" is the name of button
  .setPosition(910, 50) //x and y coordinates of upper left corner of button
  .setSize(120, 50)   //(width, height)
  .setFont(font);
  /*
  cp5.addSlider("StartVal")
  .setPosition(910, 250) //x and y upper left corner
  .setSize(10, 50) //(width, height)
  .setRange(0, 255) //slider range low,high
  .setValue(4) //start val
  .setColorBackground(color(0, 0, 255)) //top of slider color r,g,b
  .setColorForeground(color(0, 255, 0)) //botom of slider color r,g,b
  .setColorValue(color(255, 255, 255)) //vall color r,g,b
  .setColorActive(color(255, 0, 0)) //mouse over color
    ;
     Knob StartVal;
  Knob attKnob;
  Knob decKnob;
  Knob susKnob;
  Knob relKnob;
  Knob freqCutKnob;
  Knob slideKnob;
  float SVal, att,dec,sus,rel,freqCut;
 */
   StartVal = cp5.addKnob("SVal")
   .setPosition(ADSR_x, 370)
   .setSize(20, 20)
   .setRange(0, 1)
   .setRadius(20)
   .setValue(0.5);
   
   //ADSR
   attKnob = cp5.addKnob("att")
   .setPosition(ADSR_x+ADSR_dist, 370)
   .setSize(20, 20)
   .setRange(0, 3)
   .setRadius(20)
   .setValue(0.5);
 
   
   decKnob = cp5.addKnob("dec")
   .setPosition(ADSR_x+2*ADSR_dist, 370)
   .setSize(20, 20)
   .setRange(0, 5)
   .setRadius(20)
   .setValue(0.5);
   
   
   susKnob = cp5.addKnob("sus")
   .setPosition(ADSR_x+3*ADSR_dist, 370)
   .setSize(20, 20)
   .setRange(0, 5)
   .setRadius(20)
   .setValue(0.5);
   
   
   relKnob = cp5.addKnob("rel")
   .setPosition(ADSR_x+4*ADSR_dist, 370)
   .setSize(20, 20)
   .setRange(0, 5)
   .setRadius(20)
   .setValue(0.5);
   
   
   freqCutKnob =cp5.addKnob("freqCut")
   .setPosition(ADSR_x, 500)
   .setSize(20, 20)
   .setRange(0, 20000)
   .setRadius(20)
   .setValue(0.5);
   
   
   slideKnob =cp5.addKnob("slideDur")
   .setPosition(ADSR_x+200, 500)
   .setSize(20, 20)
   .setRange(0, 5)
   .setRadius(20)
   .setValue(0.5);
   
}

void draw()
{
  if (img != null )
  {
    // this function will draw an image to the display window, the last two arguments define
    // the position of the upper left corner
    img.resize(900,600);
    image( img, 0, 0 );
  }
  if(mousePressed){
  // get the color of the image in position mouseX and mouseY
  c = img.get(mouseX,mouseY);
  //println("hue: "+hue(c)+" - saturation: "+saturation(c)+" brightness :"+brightness(c));
  

  
  hue = hue(c)/256;
  sat = saturation(c)/256;
  bri = brightness(c)/256;
  /*
  mode = round((1 / (1 + exp(-5*(msg[3] - 0.5)))) *(chordRange-1));
  noteIndex = random(10);
  
    if(mode > ~prevMode)
    {
      if((~prevNoteIndex+noteIndex) > (range-1))
      {
        note = notes[~prevNoteIndex];
      }
      {
        note = notes[~prevNoteIndex+noteIndex];
      };
    }
    {
      if(mode == ~prevMode)
      {
        note = notes[~prevNoteIndex];
      }
      {
        if((~prevNoteIndex-noteIndex) < 2)
        {
          note = notes[~prevNoteIndex];
        }
        {
          note = notes[~prevNoteIndex-noteIndex];
        };
      };
    };
  */
  OscMessage myMessage = new OscMessage("/color");
  // SVal, att,dec,sus,rel,freqCut,slideDur;
  myMessage.add(hue);
  myMessage.add(sat);
  myMessage.add(bri);
  myMessage.add(SVal);
  myMessage.add(att);
  myMessage.add(dec);
  myMessage.add(sus);
  myMessage.add(rel);
  myMessage.add(freqCut);
  myMessage.add(slideDur);
  
  oscP5.send(myMessage, myRemoteLocation); 
  myMessage.print();
  

  

  //text("Choose another image:", 910, 30);
  }
}

void imageChosen( File f )
{
  if(f.exists())
  {
     img = loadImage(f.getAbsolutePath()); 
  }
}

void Select(){
  selectInput( "Select an image", "fileSelected");
}

void fileSelected(File selection) {
  if (selection == null) {
    println("Window was closed or the user hit cancel.");
  } else {
    println("User selected " + selection.getAbsolutePath());
  }
}
