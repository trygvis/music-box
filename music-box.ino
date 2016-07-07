#include <Bounce2.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

SoftwareSerial s(10, 11); // RX, TX

enum buttons {
  a = 3,
  b = 4,
  c = 5,
  d = 6,
  busy = 2 // only interrupt port
};

// HIGH means not playing, LOW means playing, should read this in setup?
volatile byte playing = HIGH;
volatile int current_song = 1;
volatile int volume = 15;
Bounce buttonA, buttonB, buttonC, buttonD;

void setup() {
  Serial.begin(9600);

  pinMode(buttons::a, INPUT_PULLUP);

  buttonA.attach(buttons::a);
  buttonA.interval(5);

  pinMode(buttons::b, INPUT_PULLUP);
  buttonB.attach(buttons::b);
  buttonB.interval(5);

  pinMode(buttons::c, INPUT_PULLUP);
  buttonC.attach(buttons::c);
  buttonC.interval(5);

  pinMode(buttons::d, INPUT_PULLUP);
  buttonD.attach(buttons::d);
  buttonD.interval(5);

  pinMode(buttons::busy, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttons::busy), playStateChange, CHANGE);

  s.begin(9600);
  mp3_set_serial(s);
  delay(1);                     // delay 1ms to set volume
  mp3_set_volume (volume);

  playing = digitalRead(buttons::busy);
  Serial.print("Booted  ");
  if (playing == HIGH) {
    Serial.print("HIGH");
  } else {
    Serial.print("LOW");
  }
  Serial.println();


  mp3_play(1);

  playing = digitalRead(buttons::busy);
  Serial.print("Booted  ");
  if (playing == HIGH) {
    Serial.print("HIGH");
  } else {
    Serial.print("LOW");
  }
  Serial.println();

}

void playStateChange() {

  Serial.println("playStateChanged()");
  playing = !playing;
}


void button_log() {
  if (buttonA.fell()) {
    Serial.println("buttonA.fell()");
  }
  if (buttonA.rose()) {
    Serial.println("buttonA.rose()");
  }
  if (buttonB.fell()) {
    Serial.println("buttonB.fell()");
  }
  if (buttonB.rose()) {
    Serial.println("buttonB.rose()");
  }
  if (buttonC.fell()) {
    Serial.println("buttonC.fell()");
  }
  if (buttonC.rose()) {
    Serial.println("buttonC.rose()");
  }
  if (buttonD.fell()) {
    Serial.println("buttonD.fell()");
  }
  if (buttonD.rose()) {
    Serial.println("buttonD.rose()");
  }
}

void loop() {
  if (playing == HIGH) {
    //Serial.println("No longer playing, start next song?");
  }
  buttonA.update();
  buttonB.update();
  buttonC.update();
  buttonD.update();

  button_log();

  if (buttonA.fell()) {
    if (playing == LOW) {
      Serial.println("mp3_next");
      mp3_next();
      current_song++;
    } else {
      Serial.println("mp3_play");
      mp3_play(current_song);
    }
  } else if (buttonB.fell()) {
    Serial.println("mp3_stop");
    mp3_stop();
  } else if (buttonC.fell()) {
    volume++;
    mp3_set_volume(volume);
  } else if (buttonD.fell()) {
    volume--;
    mp3_set_volume(volume);
  }
}
