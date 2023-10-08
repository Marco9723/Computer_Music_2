
import oscP5.*;
import netP5.*;
import java.io.File;
import controlP5.*;

//network variables
OscP5 oscP5;
NetAddress myRemoteLocation;

//chords array initialization
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
  
//image variables
PFont font;
ControlP5 cp5;
PImage img;
color c, textColor;
float hue, sat, bri;
int winWidth = 1400;
boolean newImageChoosen = false;

//music variables
int mode = 0, prevMode = 0, noteIndex = 0, prevNoteIndex = 0;
int startNote = 45, noteRange = 15, chordRange = 42, note = 40;
String[] stringNotes = {"A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab"};
int[] notes = {};
int index = 0;

//slider variables
Knob StartVal,attKnob,decKnob,susKnob,relKnob,freqCutKnob,slideKnob;
float peak, att,dec,sus,rel,freqCut,slideDur;
int ADSR_x = 870;
int ADSR_dist =100;

void setup()
{
  size(1400, 600);
  background(30,90,200);
  textSize(20);
  text("Choose an image:", 870, 30);
  text("Root note:", 870, 130);
  text("Current chord:", 870, 230);
  //------------------------
  text("ADSR parameters:", 870, 330);
  text("LPF", 870, 480);
  text("Chord change slide", 870+150, 480);
  
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
  
  //initialization of select image button
  cp5 = new ControlP5(this);
  cp5.addButton("Select")     //"Turn On" is the name of button
  .setPosition(910, 50) //x and y coordinates of upper left corner of button
  .setSize(120, 50)   //(width, height)
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
  
  //initialization of midi note values array from 45 to 60
  for (int i = startNote; i < (noteRange+startNote); i++) {
    notes = append(notes, i);
    index++;
  }
}

void draw()
{
  if (img != null && newImageChoosen)
  {
    if ((float)img.width/img.height < 1.5){
      img.resize(0,600);
    } else {
      img.resize(900,0);
    }
    // this function will draw an image to the display window, the last two arguments define
    // the position of the upper left 
    fill(51);
    //noStroke();    //rect(0, 0, 900, 500);
    image(img, 0, 0);
    newImageChoosen = false;
  }
  
  if(mousePressed && img!=null && mouseX<=img.width && mouseY<=img.height){
  // get the color of the image in position mouseX and mouseY
  c = img.get(mouseX,mouseY);
  //println("hue: "+hue(c)+" - saturation: "+saturation(c)+" brightness :"+brightness(c));
  
  hue = hue(c)/256;
  sat = (1-saturation(c)/256)*0.5; //low sat = high reverb, high sat = clean sound
  bri = brightness(c)/256;
  
  prevMode = mode;
  mode = round((1 / (1 + exp(-5*(bri - 0.5)))) *(chordRange-1));
  noteIndex = round(random(8));
  note = randomNotePicker();
  prevNoteIndex = note-startNote;
      
  OscMessage myMessage = new OscMessage("/color");
  myMessage.add(sat);
  myMessage.add(note);
  myMessage.add(mode);
  //knob values
  myMessage.add(peak);
  myMessage.add(att);
  myMessage.add(dec);
  myMessage.add(sus);
  myMessage.add(rel);
  myMessage.add(freqCut);
  myMessage.add(slideDur);
  
  oscP5.send(myMessage, myRemoteLocation); 
  
  //text background
  fill(30,90,200);  
  noStroke();
  rect(905, 145, 100, 45);
  rect(905, 245, 500, 50);
  
  textSize(45);
  fill(255);       
  //current note
  text(stringNotes[(note-startNote)%12], 910, 185);
  //current chord text
  text(chords[mode], 910, 285);
  
  //colour selected
  fill(0,40,100); 
  rect(winWidth-80-50-10, 30-10, 100+20, 100+20);
  fill(c); 
  rect(winWidth-80-50, 30, 100, 100);
  
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

int randomNotePicker(){
    if(mode > prevMode)
    {
      if((prevNoteIndex+noteIndex) > (noteRange-1))
      {
        return notes[prevNoteIndex];
      }
        else
      {
        return notes[prevNoteIndex+noteIndex];
      }
    }
    else
    {
      if(mode == prevMode)
      {
        return notes[prevNoteIndex];
      }
        else
      {
        if((prevNoteIndex-noteIndex) < 2)
        {
          return notes[prevNoteIndex];
        }
          else
        {
          return notes[prevNoteIndex-noteIndex];
        }
      }
    }
}
