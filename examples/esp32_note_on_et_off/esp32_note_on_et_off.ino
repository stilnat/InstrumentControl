#include <InstrumentControl.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include <ESPmDNS.h>  
#include <MPU6050_tockn.h>
#include <Wire.h>


MPU6050 mpu6050(Wire);
InstrumentControl instrumentControl;
char volume;

float angleX = 0;
float angleY = 0;

#define SerialMon Serial
#define APPLEMIDI_DEBUG SerialMon
#include <AppleMIDI.h>

char ssid[] = "Tenda_4CA640"; //  your network SSID (name)
char pass[] = "danslemur";    // your network password (use for WPA, or use as key for WEP)

unsigned long t0 = millis();
int8_t isConnected = 0;

APPLEMIDI_CREATE_DEFAULTSESSION_INSTANCE();

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  

  DBG_SETUP(115200);
  DBG("Booting");

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DBG("connection au Wifi..");
  }
  DBG("Connection faite !");

  DBG(F("active une session RTPMidi"));
  DBG(F("Ajoute un participant avec host"), WiFi.localIP(), "Port", AppleMIDI.getPort(), "(Nom", AppleMIDI.getName(), ")");
  DBG(F("Selectionne et appuie sur connect"));
  DBG(F("Tout est bon ! Ouvre un logiciel capable d'écouter du midi (Ableton ou autre)"));

  MIDI.begin();

  AppleMIDI.setHandleConnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc, const char* name) {
    isConnected++;
    DBG(F("Connection a la session"), ssrc, name);
  });
  AppleMIDI.setHandleDisconnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc) {
    isConnected--;
    DBG(F("deconnection"), ssrc);
  });
  
  MIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
    DBG(F("NoteOn"), note);
  });
  MIDI.setHandleNoteOff([](byte channel, byte note, byte velocity) {
    DBG(F("NoteOff"), note);
  });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  MIDI.read();
  mpu6050.update(); 
  angleX = (mpu6050.getAngleX());
  angleY = (mpu6050.getAngleY());

  if ((isConnected > 0) && (millis() - t0) > 500)
  {
    byte note = instrumentControl.AngleToMidi(angleX);
    t0 = millis();
    DBG(F("AngleX is "), angleX);
    DBG( F("note is "), note);
    MIDI.sendNoteOn(note, 120, 1);
    MIDI.sendNoteOff(note, 120, 1);
  }
}

