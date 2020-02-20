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

#include <SoftwareSerial.h>
#include "laballen.h"

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

class Minitel {
public:  
  void loop() {
//    swSer.perform_work();
    
    while (swSer.available() > 0) {
      Serial.write(swSer.read());
      yield();
    }
    while (Serial.available() > 0) {
      Serial1.write(Serial.read());
      yield();
    }
  
  }
  
  void reset() {
    Serial1.print( F(PRO1"\x7b") );
    Serial1.print( CUP(1,1) );
    Serial1.print( ED() );
  }

/**
 * Retourne une chaîne de caratères qui place le curseur en row, col.
 * @param row ligne entre 1 et 25.
 * @param col colonne entre 1 et 40 ou 80 selon le mode.
 * @return un pointeur vers une chaine de caractères.
 */
  char* CUP(const byte row, const byte col) const {
    static char s[10];
    sprintf_P(s, PSTR(CSI"%c;%cH"), '0'+row, '0'+col);
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
    sprintf_P(s, PSTR(CSI"%cJ"), '0'+ps);
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

  void setup() {
    Serial1.begin(1200, SERIAL_7E1); // TX only on GPIO2 = D4
    swSer.begin(1200, SWSERIAL_7E1, D5, false); // RX only on GPIO14 = D5
    while (!Serial1 || !swSer) yield();
    reset();
    Serial1.println( F("Copyright (c) 2020 par Marc SIBERT") );
    Serial1.println( );
    Serial1.println( );
    Serial1.println(F(" _         _     _  ___  _ _"));
    Serial1.println(F("| |       | |   ( )/ _ \\| | |"));
    Serial1.println(F("| |   __ _| |__ |// /_\\ \\ | | ___ _ __"));
    Serial1.println(F("| |  / _` | '_ \\  |  _  | | |/ _ \\ '_ \\"));
    Serial1.print  (F("| |_| (_| | |_) | | | | | | |  __/ | | |"));
    Serial1.print  (F("\\___/\\__,_|_.__/  |_| |_|_|_|\\___|_| |_|"));
    Serial1.println( );
    Serial1.println( );
    Serial1.println( );
    Serial1.println( );
    Serial1.println( );
    Serial1.println( );
    Serial1.println( );
    Serial1.println( );
    Serial1.println( );
    Serial1.println( );
    Serial1.println( );
    Serial1.println( );
    Serial1.println(F("      ... zzzZZ "));
    Serial1.println(F("     (- -)      "));
    Serial1.println(F("`ooO``(_)``Ooo``````````````````````````"));




                                                                                                                                  
                                                                                        
    
  }

private:
  SoftwareSerial swSer;
};

Minitel minitel;

void setup() {
	Serial.begin(115200);
  while (!Serial) yield();
  minitel.setup();

	Serial.println("\nSoftware serial test started");

}

void loop() {
  minitel.loop();
}
