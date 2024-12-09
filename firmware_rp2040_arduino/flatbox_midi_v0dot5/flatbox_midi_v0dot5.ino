//FLATBOX MIDI CC NO LED
//for rp2040
//dpx i@dubpixel.tv

#include <Wire.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

// Create a new instance of the Arduino MIDI Library,
// and attach usb_midi as the transport.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, usbMIDI);


//MIDI CC 
const int CHANNEL= 1; //MIDI channel 
#define N_OUT   18
#define ON_VAL 127

unsigned pinsIn[N_OUT]= {6,5,4,3,2,1,9,10,11,16,18,25,27,29,19,24,26,28};
int pinVal[N_OUT] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int pinPVal[N_OUT]= {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const int ccVAL[N_OUT] = {31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48};


void setup() {

  usbMIDI.begin();
  Serial.begin(115200);
  for(unsigned i=0; i < N_OUT; i++){
    pinMode (pinsIn[i], INPUT_PULLUP);
  }

  // Wait until device is enumerated properly before sending MIDI message
  while( !TinyUSBDevice.mounted() ) delay(1);
}

//main loop
void loop() {

//read buttons
    for(unsigned i=0; i < N_OUT; i++){
      pinVal[i] = digitalRead(pinsIn[i]);
      if (pinVal[i] != pinPVal[i]){ //debounce
        if(pinVal[i]==1){ //if 1 send 0
          usbMIDI.sendControlChange(ccVAL[i], 0, CHANNEL);
        }
        else{ //otherwise send 1
          usbMIDI.sendControlChange(ccVAL[i], ON_VAL, CHANNEL);
        }
      pinPVal[i] = pinVal[i]; //set old button val
      }//if debounce
    }//end for loop/ read buttons and send midi
    //ignore midi incoming
  while (usbMIDI.read()) {
  }
  delay(1);
}
