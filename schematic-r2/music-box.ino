#include <Bounce2.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

SoftwareSerial s(10, 11); // RX, TX

enum buttons {
  a = 6,
  b = 7,
  c = 8,
  d = 9,
  busy = 3 // only interrupt port
};

volatile bool playing = false;
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
  delay(1000);                     // delay 1ms to set volume
  mp3_set_volume (volume);

  playing = digitalRead(buttons::busy) == LOW;
  Serial.print("BUSY  ");
  Serial.println(playing ? "yes" : "no");

  delay(1000);
  mp3_play(1);

  playing = digitalRead(buttons::busy) == LOW;
  Serial.print("BUSY  ");
  Serial.println(playing ? "yes" : "no");
}

void playStateChange() {
  Serial.println("playStateChanged()");
  playing = !digitalRead(buttons::busy);
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
  playing = digitalRead(buttons::busy) == LOW;

  if (!playing) {
    Serial.println("No longer playing, start next song?");
  }
  buttonA.update();
  buttonB.update();
  buttonC.update();
  buttonD.update();

  button_log();

  if (buttonA.fell()) {
    if (!playing) {
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
