/**
   Copyright 2020 Marc SIBERT

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#define D0 (16)
#define D5 (14)
#define D6 (12)
#define D7 (13)
#define D8 (15)

#define ESC "\x1B"
#define PRO1 ESC"\x39"
#define BEL "\x07"
#define BS "\x08"
#define HT "\x09"
#define LF "\x0A"
#define CR "\x0D"
#define CSI ESC"\x5B"
// Home
#define RS "\x1E"
// Clear Screen + Home
#define FF "\x0C"

#include <SoftwareSerial.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "font.h"

class Minitel {
public:  
  void setup() {
    Serial1.begin(1200, SERIAL_7E1); // TX only on GPIO2 = D4
    swSer.begin(1200, SWSERIAL_7E1, D5, false); // RX only on GPIO14 = D5
    while (!Serial1 || !swSer) yield();
    reset();
    Serial1.print( F("Copyright (c) 2020 par Marc SIBERT") );
    yield();

    ntpClient.begin();

    for (byte i = 0; i < 6; ++i) {
      Serial1.print(CUP(3+i, 1));
      Serial1.print(lab[i]);
    }
    yield();

    Serial1.print(CUP(22, 1));
    Serial1.println(F("      ... zzzZZ "));
    Serial1.println(F("     (- -)      "));
    Serial1.print  (F("`ooO``(_)``Ooo``````````````````````````"));
    yield();

  }

  void loop() {

    ntpClient.update();
    
    const byte hours = ntpClient.getHours();
    const byte minutes = ntpClient.getMinutes();

    const byte of = 3;
    digit(of+ 0, '0' + (hours / 10) );
    yield();
    digit(of+ 8, '0' + (hours % 10) );
    yield();
    digit(of+16, ':');
    yield();
    digit(of+21, '0' + (minutes / 10) );
    yield();
    digit(of+29, '0' + (minutes % 10) );
    delay(10000);

//    swSer.perform_work();

    /*
    while (swSer.available() > 0) {
      Serial.write(swSer.read());
      yield();
    }
    while (Serial.available() > 0) {
      Serial1.write(Serial.read());
      yield();
    }
*/  
  }
  
  void reset() {
    Serial1.print( F(PRO1"\x7b") );
    Serial1.print( FF );
//    Serial1.print( CUP(1,1) );
//    Serial1.print( ED() );
  }

/**
 * Retourne une chaîne de caratères qui place le curseur en row, col.
 * @param row ligne entre 1 et 25.
 * @param col colonne entre 1 et 40 ou 80 selon le mode.
 * @return un pointeur vers une chaine de caractères.
 */
  char* CUP(const byte row, const byte col) const {
    static char s[10];
    sprintf_P(s, PSTR(CSI"%i;%iH"), row, col);
    return s;
  }

/**
 * Retourne une chaîne de caratère qui efface l'écran .
 * @param ps : 
 *    0 : Effacement depuis la position jusqu'à la fin de la page (par défaut) ;
 *    1 : Effacement depuis le début de la page jusqu'à la position du curseur incluse ;
 *    2 : Effacement complet de la page sans modification de la position du curseur.
 * @return une référence sur la chaine de caractères.
 */
  char* ED(const byte ps = 0) const {
    static char s[5];
    if (ps) sprintf_P(s, PSTR(CSI"%cJ"), '0'+ps);
    else strcpy_P(s, PSTR(CSI"J"));
    return s;
  }

  String& imagePBM(const unsigned& aSize, const char* aFile) const {
    static String s;
    const char *p = aFile;
    if (!strncmp(p, "P5", 2)) {
      return s = String( F("Fichier inattendu !") );
    }
    p += '\x1A';
            
  }

  void digit(const byte col, const char val) {
    switch(val) {
      case ':' :
        Serial1.print(CUP(12, col));  Serial1.print(F(" ## "));
        Serial1.print(CUP(13, col));  Serial1.print(F("####"));
        Serial1.print(CUP(14, col));  Serial1.print(F(" ## "));
        Serial1.print(CUP(15, col));  Serial1.print(F("    "));
        Serial1.print(CUP(16, col));  Serial1.print(F(" ## "));
        Serial1.print(CUP(17, col));  Serial1.print(F("####"));
        Serial1.print(CUP(18, col));  Serial1.print(F(" ## "));
        break;
      case '0' :
        Serial1.print(CUP(12, col));  Serial1.print(F(" ##### "));
        for (byte i = 0; i < 5; ++i) {
          Serial1.print(CUP(13+i, col)); Serial1.print(F("##   ##"));
        }
        Serial1.print(CUP(18, col));  Serial1.print(F(" ##### "));
        break;
      case '1' :
        Serial1.print(CUP(12, col));  Serial1.print(F("   ##  "));
        Serial1.print(CUP(13, col));  Serial1.print(F("  ###  "));
        Serial1.print(CUP(14, col));  Serial1.print(F(" ####  "));
        for (byte i = 0; i < 4; ++i) {
          Serial1.print(CUP(15+i, col));  Serial1.print(F("   ##  "));
        }
        break;
      case '2' :
        Serial1.print(CUP(12, col));  Serial1.print(F(" ##### "));
        Serial1.print(CUP(13, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(14, col));  Serial1.print(F("     ##"));
        Serial1.print(CUP(15, col));  Serial1.print(F("    ## "));
        Serial1.print(CUP(16, col));  Serial1.print(F("  ###  "));
        Serial1.print(CUP(17, col));  Serial1.print(F(" ##    "));
        Serial1.print(CUP(18, col));  Serial1.print(F("#######"));
        break;
      case '3' :
        Serial1.print(CUP(12, col));  Serial1.print(F(" ##### "));
        Serial1.print(CUP(13, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(14, col));  Serial1.print(F("     ##"));
        Serial1.print(CUP(15, col));  Serial1.print(F("    ## "));
        Serial1.print(CUP(16, col));  Serial1.print(F("     ##"));
        Serial1.print(CUP(17, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(18, col));  Serial1.print(F(" ##### "));
        break;
      case '4' :
        Serial1.print(CUP(12, col));  Serial1.print(F(" ##    "));
        Serial1.print(CUP(13, col));  Serial1.print(F(" ##    "));
        Serial1.print(CUP(14, col));  Serial1.print(F("##  ## "));
        Serial1.print(CUP(15, col));  Serial1.print(F("##  ## "));
        Serial1.print(CUP(16, col));  Serial1.print(F("#######"));
        Serial1.print(CUP(17, col));  Serial1.print(F("    ## "));
        Serial1.print(CUP(18, col));  Serial1.print(F("    ## "));
        break;
      case '5' :
        Serial1.print(CUP(12, col));  Serial1.print(F("#######"));
        Serial1.print(CUP(13, col));  Serial1.print(F("##     "));
        Serial1.print(CUP(14, col));  Serial1.print(F("#####  "));
        Serial1.print(CUP(15, col));  Serial1.print(F("    ## "));
        Serial1.print(CUP(16, col));  Serial1.print(F("     ##"));
        Serial1.print(CUP(17, col));  Serial1.print(F("##  ## "));
        Serial1.print(CUP(18, col));  Serial1.print(F(" ####  "));
        break;
      case '6' :
        Serial1.print(CUP(12, col));  Serial1.print(F("   ### "));
        Serial1.print(CUP(13, col));  Serial1.print(F(" ##    "));
        Serial1.print(CUP(14, col));  Serial1.print(F("##     "));
        Serial1.print(CUP(15, col));  Serial1.print(F("###### "));
        Serial1.print(CUP(16, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(17, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(18, col));  Serial1.print(F(" ##### "));
        break;
      case '7' :
        Serial1.print(CUP(12, col));  Serial1.print(F("#######"));
        Serial1.print(CUP(13, col));  Serial1.print(F("     ##"));
        Serial1.print(CUP(14, col));  Serial1.print(F("    ## "));
        Serial1.print(CUP(15, col));  Serial1.print(F(" ######"));
        Serial1.print(CUP(16, col));  Serial1.print(F("   ##  "));
        Serial1.print(CUP(17, col));  Serial1.print(F("  ##   "));
        Serial1.print(CUP(18, col));  Serial1.print(F("  ##   "));
        break;
      case '8' :
        Serial1.print(CUP(12, col));  Serial1.print(F(" ##### "));
        Serial1.print(CUP(13, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(14, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(15, col));  Serial1.print(F(" ##### "));
        Serial1.print(CUP(16, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(17, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(18, col));  Serial1.print(F(" ##### "));
        break;
      case '9' :
        Serial1.print(CUP(12, col));  Serial1.print(F(" ##### "));
        Serial1.print(CUP(13, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(14, col));  Serial1.print(F("##   ##"));
        Serial1.print(CUP(15, col));  Serial1.print(F(" ##### "));
        Serial1.print(CUP(16, col));  Serial1.print(F("    ## "));
        Serial1.print(CUP(17, col));  Serial1.print(F("   ##  "));
        Serial1.print(CUP(18, col));  Serial1.print(F("  ##   "));
        break;
      case ' ' :
        for (byte i = 0; i < 7; ++i) {
          Serial1.print(CUP(12+i, col));  Serial1.print(F("       "));
        }
        break;
    }
  }

private:
  SoftwareSerial swSer;
  static const char* lab[] PROGMEM;
  static WiFiUDP ntpUDP;
  static NTPClient ntpClient; 
};

const char* Minitel::lab[] PROGMEM = {
  "__        _     _  ___  _ _",
  "{}       | |   ( )/ _ \\| | |",
  "{}   __ _| |__ |// /_\\ \\ | | ___ _ __",
  "{}  / _` | '_ \\  |  _  | | |/ _ \\ '_ \\",
  "{}__{(_{ | |_) | | | | | | |  __/ | | |",
  "\\__/\\__,_|_.__/  |_| |_|_|_|\\___|_| |_|"
};

WiFiUDP Minitel::ntpUDP;

NTPClient Minitel::ntpClient = NTPClient(Minitel::ntpUDP, "europe.pool.ntp.org", 3600);
