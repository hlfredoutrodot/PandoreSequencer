/*
 * .----------------.  .----------------.  .-----------------. .----------------.  .----------------.  .----------------.  .----------------.
 * | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |
 * | |   ______     | || |      __      | || | ____  _____  | || |  ________    | || |     ____     | || |  _______     | || |  _________   | |
 * | |  |_   __ \   | || |     /  \     | || ||_   \|_   _| | || | |_   ___ `.  | || |   .'    `.   | || | |_   __ \    | || | |_   ___  |  | |
 * | |    | |__) |  | || |    / /\ \    | || |  |   \ | |   | || |   | |   `. \ | || |  /  .--.  \  | || |   | |__) |   | || |   | |_  \_|  | |
 * | |    |  ___/   | || |   / ____ \   | || |  | |\ \| |   | || |   | |    | | | || |  | |    | |  | || |   |  __ /    | || |   |  _|  _   | |
 * | |   _| |_      | || | _/ /    \ \_ | || | _| |_\   |_  | || |  _| |___.' / | || |  \  `--'  /  | || |  _| |  \ \_  | || |  _| |___/ |  | |
 * | |  |_____|     | || ||____|  |____|| || ||_____|\____| | || | |________.'  | || |   `.____.'   | || | |____| |___| | || | |_________|  | |
 * | |              | || |              | || |              | || |              | || |              | || |              | || |              | |
 * | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |
 * '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'
 *
 * Version de développement 0.0.7
 *
 * Code source officiel du séquenceur d'un projet de boite à rythme open-source modulable.
 * Développé en France par Hélie Frédout Rodot.
 *
 */

//ajout des bibliothèques
#include <LiquidCrystal.h>
#include <Scheduler.h>
#include <MIDI.h>
#include <PushButton.h>
#include <DueFlashStorage.h>
#include <efc.h>
#include <flash_efc.h>

DueFlashStorage dueFlashStorage;



//initialisation de l'écran lcd
const int rs = 8, en = 9, d4 = 17, d5 = 16, d6 = 15, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//initialisation de la bibliothèque de gestion MIDI
#if defined(ARDUINO_SAM_DUE) || defined(SAMD_SERIES)
/* example not relevant for this hardware (SoftwareSerial not supported) */
MIDI_CREATE_DEFAULT_INSTANCE();
#else
#include <SoftwareSerial.h>
using Transport = MIDI_NAMESPACE::SerialMIDI<SoftwareSerial>;
int rxPin = 18;
int txPin = 19;
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);
Transport serialMIDI(mySerial);
MIDI_NAMESPACE::MidiInterface<Transport> MIDI((Transport&)serialMIDI);
#endif

//définition des caractères personnalisés

byte charPlay[] = { B00000, B01000, B01110, B01111, B01111, B01110, B01000, B00000 };
byte charPause[] = { B00000, B11011, B11011, B11011, B11011, B11011, B11011, B00000 };
byte charFkey[] = { B00000, B11000, B11101, B00100, B00101, B00100, B01000, B10000 };
byte charJoli[] = { B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000 };

byte charNoteEmpty[] = { B00000, B11111, B00000, B00000, B11111, B00000, B00000, B11111 };
byte charNoteHighest[] = { B01100, B11111, B01100, B00000, B11111, B00000, B00000, B11111 };
byte charNoteHigher[] = { B00000, B11111, B01100, B01100, B11111, B00000, B00000, B11111 };
byte charNoteHigh[] = { B00000, B11111, B00000, B01100, B11111, B01100, B00000, B11111 };
byte charNoteMid[] = { B00000, B11111, B00000, B00000, B11111, B01100, B01100, B11111 };
byte charNoteLow[] = { B00000, B11111, B00000, B00000, B11111, B00000, B01100, B11111 };
byte charNoteLower[] = { B01100, B11111, B00000, B00000, B11111, B00000, B00000, B11111 };

//définition des 4 rangs d'interrupteurs + broches
const int i11 = 37;
const int i12 = 35;
const int i13 = 33;
const int i14 = 31;
const int i15 = 29;
const int i16 = 27;
const int i17 = 25;
const int i18 = 23;

const int i21 = 53;
const int i22 = 51;
const int i23 = 49;
const int i24 = 47;
const int i25 = 45;
const int i26 = 43;
const int i27 = 41;
const int i28 = 39;

const int i31 = 36;
const int i32 = 34;
const int i33 = 32;
const int i34 = 30;
const int i35 = 28;
const int i36 = 26;
const int i37 = 24;
const int i38 = 22;

const int i41 = 52;
const int i42 = 50;
const int i43 = 48;
const int i44 = 46;
const int i45 = 44;
const int i46 = 42;
const int i47 = 40;
const int i48 = 38;

int insyncpin = A7;  //pin recevant le signal de synchronisation
int tempoled = 7;    //pin de la led jaune de tempo
int minled = 10;
int modeled = 6;
int maxled = 5;
PushButton BTNmin(2);
PushButton BTNmode(3);
PushButton BTNmax(4);
PushButton BTNup(11);
PushButton BTNdown(12);

int stepn;   //stocke le numéro du pas en cours de lecture
int stepn1;  //stocke le nombre du pas en cours de lecture pour la rangée 1
int stepn2;  //stocke le nombre du pas en cours de lecture pour la rangée 2
int stepn3;  //stocke le nombre du pas en cours de lecture pour la rangée 3
int stepn4;  //stocke le nombre du pas en cours de lecture pour la rangée 4
bool hit1;   //vérifie si le la rangée 1 doit émettre un son sur le pas en cours
bool hit2;   //vérifie si le la rangée 2 doit émettre un son sur le pas en cours
bool hit3;   //vérifie si le la rangée 3 doit émettre un son sur le pas en cours
bool hit4;   //vérifie si le la rangée 4 doit émettre un son sur le pas en cours

int charCreated = 1;
bool doblink;
int tempcount = 0;
bool playing = false;
int timeduration;     //stocke la duréee d'un temps en fonction du tempo
int quarterduration;  //stocke la durée d'un quart de temps en fonction du tempo
int stepduration;
int timedivider = 4;
int stepnoten;
int previousstepnoten;
int pattern1lenght;

int previousmillis;
int CursorBlinkCount = 1;

//définition des variables nécessaires à la navigation en menus
int GMenu = 0;
int mmenu = 0;
int CursorCoord = 0;
bool showCursor = false;
int CursorSpeed = 250;

//définiton des variables de configuration (configurées automatiquement lors du premier démarrage du code, plus tard dans recover())
int tempo;     //stocke le tempo modifiable par cmd
bool syncuse;  //stocke la position de l'interrupteur commandant la synchronisation
int Out1CH;
int Out2CH;
int Out3CH;
int Out4CH;
int Out5CH;

int Track1List[16];

void setup() {

  //initialisation à 115200 bauds
  Serial.begin(115200);
  //démarrage de la connexion MIDI
  MIDI.begin(16);
  //initialisation de l'écran lcd
  lcd.begin(16, 2);
  lcd.createChar(0, charPlay);
  lcd.createChar(1, charPause);
  lcd.createChar(2, charFkey);
  lcd.createChar(3, charJoli);

  //définition des pin interrupteurs en tant que sorties
  pinMode(i11, INPUT_PULLUP);
  pinMode(i12, INPUT_PULLUP);
  pinMode(i13, INPUT_PULLUP);
  pinMode(i14, INPUT_PULLUP);
  pinMode(i15, INPUT_PULLUP);
  pinMode(i16, INPUT_PULLUP);
  pinMode(i17, INPUT_PULLUP);
  pinMode(i18, INPUT_PULLUP);

  pinMode(i21, INPUT_PULLUP);
  pinMode(i22, INPUT_PULLUP);
  pinMode(i23, INPUT_PULLUP);
  pinMode(i24, INPUT_PULLUP);
  pinMode(i25, INPUT_PULLUP);
  pinMode(i26, INPUT_PULLUP);
  pinMode(i27, INPUT_PULLUP);
  pinMode(i28, INPUT_PULLUP);

  pinMode(i31, INPUT_PULLUP);
  pinMode(i32, INPUT_PULLUP);
  pinMode(i33, INPUT_PULLUP);
  pinMode(i34, INPUT_PULLUP);
  pinMode(i35, INPUT_PULLUP);
  pinMode(i36, INPUT_PULLUP);
  pinMode(i37, INPUT_PULLUP);
  pinMode(i38, INPUT_PULLUP);

  pinMode(i41, INPUT_PULLUP);
  pinMode(i42, INPUT_PULLUP);
  pinMode(i43, INPUT_PULLUP);
  pinMode(i44, INPUT_PULLUP);
  pinMode(i45, INPUT_PULLUP);
  pinMode(i46, INPUT_PULLUP);
  pinMode(i47, INPUT_PULLUP);
  pinMode(i48, INPUT_PULLUP);

  //définition des autres pin

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);


  pinMode(tempoled, OUTPUT);
  pinMode(minled, OUTPUT);
  pinMode(modeled, OUTPUT);
  pinMode(maxled, OUTPUT);

  BTNmin.setActiveLogic(LOW);
  BTNmode.setActiveLogic(HIGH);
  BTNmax.setActiveLogic(HIGH);
  BTNup.setActiveLogic(LOW);
  BTNdown.setActiveLogic(LOW);

  BTNmin.disableDoubleClick();
  BTNmode.disableDoubleClick();
  BTNmax.disableDoubleClick();
  BTNup.disableDoubleClick();
  BTNdown.disableDoubleClick();

  BTNmin.setHoldTime(250);
  BTNmode.setHoldTime(10000);
  BTNmax.setHoldTime(250);
  BTNup.setHoldTime(250);
  BTNdown.setHoldTime(250);

  BTNmin.update();
  BTNmode.update();
  BTNmax.update();
  BTNup.update();
  BTNdown.update();

  //définition des autres pin
  Serial.setTimeout(10);

  //lecture et restauration de la mémoire
  recover();

  //écran d'accueil
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Bienvenue sur");
  lcd.setCursor(3, 1);
  delay(200);
  lcd.print("Pandore  ");
  lcd.write(byte(2));
  delay(3000);
  display();

  //démarrage des boucles
  Scheduler.startLoop(sequencer);  //démarre la boucle gérant la lecture des séquences
}

void loop() {

  yield();
  //transformation du tempo en quarterduration (écarts de pas)
  quarterduration = 60000 / tempo;
  timeduration = 4 * quarterduration;
  stepduration = timeduration / 4;

  //mise à jour des boutons poussoirs de fonction
  BTNmin.update();
  BTNmode.update();
  BTNmax.update();
  BTNup.update();
  BTNdown.update();

  //exécution des commandes données par les boutons

  if (BTNmode.isClicked()) {  //Action du bouton mode
    if (GMenu == 0) {
      if (playing) {
        MIDI.sendStop();
        MIDI.sendNoteOff(60, 0, Out1CH);
        MIDI.sendNoteOff(60, 0, Out2CH);
        MIDI.sendNoteOff(60, 0, Out3CH);
        MIDI.sendNoteOff(60, 0, Out4CH);
        playing = false;
        stepn = 0;
        stepnoten = 0;
        tempcount = 0;
        analogWrite(tempoled, 0);
        displayimmediate();
      } else {
        MIDI.sendStart();
        playing = true;
        stepn = 0;
        stepnoten = 0;
        tempcount = 0;
        displayimmediate();
      }
    }
    if (GMenu != 0) {
      if (mmenu == 0) {
        mmenu = -1;
      } else {
        mmenu = 0;
      }
    }
    if (GMenu == -3) {
      display();
      save();
      delay(500);
      mmenu = 0;
      display();
    }
    display();
  }

  if (BTNmin.isClicked()) {  //Action du bouton min
    if (GMenu > 0) {
      if (mmenu != 0) {
        if (CursorCoord > 0) {
          CursorCoord = CursorCoord - 1;
        }
      }
    }
    if (GMenu == 0) {
      tempo = tempo - 1;
    }
    if (GMenu == -1) {
      if (mmenu == -1) {
        syncuse = false;
      }
    }

    if (GMenu == -2) {
      if (mmenu == -1) {
        if (Out1CH > 1) {
          Out1CH = Out1CH - 1;
        }
      }
      if (mmenu == -2) {
        if (Out2CH > 1) {
          Out2CH = Out2CH - 1;
        }
      }
      if (mmenu == -3) {
        if (Out3CH > 1) {
          Out3CH = Out3CH - 1;
        }
      }
      if (mmenu == -4) {
        if (Out4CH > 1) {
          Out4CH = Out4CH - 1;
        }
      }
      if (mmenu == -5) {
        if (Out5CH > 1) {
          Out5CH = Out5CH - 1;
        }
      }
    }

    display();
  }
  if (BTNmin.isHeld()) {  //Action du bouton min maintenu
    if (GMenu == 0) {
      tempo = tempo - 9;
    }
    display();
  }

  if (BTNmax.isClicked()) {  //Action du bouton max

    if (GMenu > 0) {
      if (mmenu != 0) {
        if (CursorCoord < 15) {
          CursorCoord = CursorCoord + 1;
        }
      }
    }
    if (GMenu == 0) {
      tempo = tempo + 1;
    }
    if (GMenu == -1) {
      if (mmenu == -1) {
        syncuse = true;
      }
    }
    if (GMenu == -2) {
      if (mmenu == -1) {
        if (Out1CH < 16) {
          Out1CH = Out1CH + 1;
        }
      }
      if (mmenu == -2) {
        if (Out2CH < 16) {
          Out2CH = Out2CH + 1;
        }
      }
      if (mmenu == -3) {
        if (Out3CH < 16) {
          Out3CH = Out3CH + 1;
        }
      }
      if (mmenu == -4) {
        if (Out4CH < 16) {
          Out4CH = Out4CH + 1;
        }
      }
      if (mmenu == -5) {
        if (Out5CH < 16) {
          Out5CH = Out5CH + 1;
        }
      }
    }
    display();
  }

  if (BTNmax.isHeld()) {  //Action du bouton max maintenu
    if (GMenu == 0) {
      tempo = tempo + 9;
    }
    display();
  }

  if (BTNup.isClicked()) {  //Action du bouton haut
    if (GMenu > 0) {
      if (Track1List[CursorCoord] < 11) {
        Track1List[CursorCoord] = Track1List[CursorCoord] + 1;
      }
    }
    if (mmenu == 0) {
      GMenu = GMenu + 1;
    } else {
      if (mmenu != -1) {
        mmenu = mmenu + 1;
      }
    }
    display();
  }

  if (BTNdown.isClicked()) {  //Action du bouton bas
    if (GMenu > 0) {
      if (Track1List[CursorCoord] > 0) {
        Track1List[CursorCoord] = Track1List[CursorCoord] - 1;
      }
      if (mmenu == 0) {
        GMenu = GMenu - 1;
      }
    } else if (GMenu != 1) {
      if (mmenu == 0) {
        GMenu = GMenu - 1;
      } else {
        mmenu = mmenu - 1;
      }
    }
    display();
  }

  yield();
}

void display() {
  if (!playing) {
    displayimmediate();
  }
}

void displayimmediate() {

  yield();
  customChar();
  lcd.clear();

  if (GMenu > 1) {
    GMenu = 1;
  }

  if (GMenu < -3) {
    GMenu = -3;
  }

  if (GMenu == 1) {
    if (mmenu != 0) {
      showCursor = true;
      for (int t = 0; t < 17; t++) {
        if (Track1List[t] < 5) {
          lcd.setCursor(t, 0);
          lcd.write(byte(0));
          lcd.setCursor(t, 1);
          lcd.write(byte(Track1List[t] + 1));
        }
        if (Track1List[t] == 0) {
          lcd.setCursor(t, 0);
          lcd.write(byte(0));
          lcd.setCursor(t, 1);
          lcd.write(byte(0));
        }
        if (Track1List[t] == 5) {
          lcd.setCursor(t, 0);
          lcd.write(byte(0));
          lcd.setCursor(t, 1);
          lcd.write(byte(1));
        }

        if (Track1List[t] > 5) {
          lcd.setCursor(t, 0);
          lcd.write(byte(Track1List[t] - 4));
          lcd.setCursor(t, 1);
          lcd.write(byte(0));
        }
        if (Track1List[t] == 11) {
          lcd.setCursor(t, 0);
          lcd.write(byte(1));
          lcd.setCursor(t, 1);
          lcd.write(byte(0));
        }
      }
    }
    if (mmenu == 0) {
      showCursor = false;
      lcd.setCursor(2, 0);
      lcd.print("Sequence Note");
      lcd.setCursor(5, 1);
      lcd.print("Piste 1");
    }
  }

  if (GMenu == 0) {
    if (syncuse) {
      lcd.setCursor(7, 0);
      lcd.print("---");
    } else {
      if (tempo > 99) {
        lcd.setCursor(7, 0);
        lcd.print(tempo);
      } else {
        lcd.setCursor(7, 0);
        lcd.print(" ");
        lcd.setCursor(8, 0);
        lcd.print(tempo);
      }
    }

    if (playing) {
      lcd.setCursor(8, 1);
      lcd.write(byte(0));
    } else {
      lcd.setCursor(8, 1);
      lcd.write(byte(1));
    }

    lcd.setCursor(13, 1);
    if (syncuse) {
      lcd.print("ext");
      digitalWrite(modeled, HIGH);
    } else {
      lcd.print("int");
      digitalWrite(modeled, LOW);
    }

    if (mmenu != 0) {
      mmenu = 0;
    }
  }


  if (GMenu == -1) {

    if (mmenu < -5) {
      mmenu = -5;
    }

    if (mmenu == 0) {
      lcd.setCursor(5, 0);
      lcd.print("Config");
      lcd.setCursor(6, 1);
      lcd.print("MIDI");
    }

    if (mmenu != 0) {
      lcd.setCursor(0, 0);
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.print(" MIDI ");
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.write(byte(3));
    }

    if (mmenu == -1) {
      lcd.setCursor(0, 1);
      lcd.print("1.synchro");
      lcd.setCursor(11, 1);
      if (syncuse) {
        lcd.print("<ext>");
        digitalWrite(modeled, HIGH);
      } else {
        lcd.print("<int>");
        digitalWrite(modeled, LOW);
      }
    }
  }

  if (GMenu == -2) {

    if (mmenu < -5) {
      mmenu = -5;
    }

    if (mmenu == 0) {
      lcd.setCursor(5, 0);
      lcd.print("Config");
      lcd.setCursor(3, 1);
      lcd.print("Canaux MIDI");
    }

    if (mmenu != 0) {
      lcd.setCursor(0, 0);
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.print(" MIDI CH ");
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.write(byte(3));
    }

    if (mmenu == -1) {
      lcd.setCursor(0, 1);
      lcd.print("2.Nte1 CH");
      if (Out1CH < 10) {
        lcd.setCursor(12, 1);
        lcd.print("<0");
        lcd.print(Out1CH);
        lcd.print(">");
      } else {
        lcd.setCursor(12, 1);
        lcd.print("<");
        lcd.print(Out1CH);
        lcd.print(">");
      }
    }
    if (mmenu == -2) {
      lcd.setCursor(0, 1);
      lcd.print("3.Prc1 CH");
      if (Out1CH < 10) {
        lcd.setCursor(12, 1);
        lcd.print("<0");
        lcd.print(Out2CH);
        lcd.print(">");
      } else {
        lcd.setCursor(12, 1);
        lcd.print("<");
        lcd.print(Out2CH);
        lcd.print(">");
      }
    }
    if (mmenu == -3) {
      lcd.setCursor(0, 1);
      lcd.print("4.Prc2 CH");
      if (Out1CH < 10) {
        lcd.setCursor(12, 1);
        lcd.print("<0");
        lcd.print(Out3CH);
        lcd.print(">");
      } else {
        lcd.setCursor(12, 1);
        lcd.print("<");
        lcd.print(Out3CH);
        lcd.print(">");
      }
    }
    if (mmenu == -4) {
      lcd.setCursor(0, 1);
      lcd.print("5.Prc3 CH");
      if (Out1CH < 10) {
        lcd.setCursor(12, 1);
        lcd.print("<0");
        lcd.print(Out4CH);
        lcd.print(">");
      } else {
        lcd.setCursor(12, 1);
        lcd.print("<");
        lcd.print(Out4CH);
        lcd.print(">");
      }
    }
    if (mmenu == -5) {
      lcd.setCursor(0, 1);
      lcd.print("5.Prc4 CH");
      if (Out5CH < 10) {
        lcd.setCursor(12, 1);
        lcd.print("<0");
        lcd.print(Out5CH);
        lcd.print(">");
      } else {
        lcd.setCursor(12, 1);
        lcd.print("<");
        lcd.print(Out5CH);
        lcd.print(">");
      }
    }
  }

  if (GMenu == -3) {
    if (mmenu == 0) {
      lcd.setCursor(3, 0);
      lcd.print("Sauvegarder");
      lcd.setCursor(3, 1);
      lcd.print("la session");
    }
    if (mmenu != 0) {
      lcd.setCursor(0, 0);
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.print(" Sauvegarder ");
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.write(byte(3));
      lcd.print(" la session ");
      lcd.write(byte(3));
      lcd.write(byte(3));
    }
  }
}

void save() {
  dueFlashStorage.write(0, tempo);
  dueFlashStorage.write(1, syncuse);
  dueFlashStorage.write(2, Out1CH);
  dueFlashStorage.write(3, Out2CH);
  dueFlashStorage.write(4, Out3CH);
  dueFlashStorage.write(5, Out4CH);
  dueFlashStorage.write(6, Out5CH);
}

void recover() {
  //Correction des valeurs erronées lors du premier démarrage après reprogrammation
  //tempo
  if (dueFlashStorage.read(0) == 255) {
    dueFlashStorage.write(0, 120);
  }

  //syncuse
  if (dueFlashStorage.read(1) == 255) {
    dueFlashStorage.write(1, 0);
  }

  //Out1CH
  if (dueFlashStorage.read(2) == 255) {
    dueFlashStorage.write(2, 9);
  }
  //Out2CH
  if (dueFlashStorage.read(3) == 255) {
    dueFlashStorage.write(3, 1);
  }
  //Out3CH
  if (dueFlashStorage.read(4) == 255) {
    dueFlashStorage.write(4, 2);
  }
  //Out4CH
  if (dueFlashStorage.read(5) == 255) {
    dueFlashStorage.write(5, 3);
  }
  //Out5CH
  if (dueFlashStorage.read(6) == 255) {
    dueFlashStorage.write(6, 4);
  }

  tempo = dueFlashStorage.read(0);
  syncuse = dueFlashStorage.read(1);
  Out1CH = dueFlashStorage.read(2);
  Out2CH = dueFlashStorage.read(3);
  Out3CH = dueFlashStorage.read(4);
  Out4CH = dueFlashStorage.read(5);
  Out5CH = dueFlashStorage.read(6);
}

void customChar() {
  if (GMenu != 1) {
    if (charCreated != 1) {
      lcd.createChar(0, charPlay);
      lcd.createChar(1, charPause);
      lcd.createChar(2, charFkey);
      lcd.createChar(3, charJoli);
      charCreated = 1;
    }
  }

  if (GMenu == 1) {
    if (charCreated != 2) {
      lcd.createChar(6, charNoteHighest);
      lcd.createChar(5, charNoteHigher);
      lcd.createChar(4, charNoteHigh);
      lcd.createChar(3, charNoteMid);
      lcd.createChar(2, charNoteLow);
      lcd.createChar(1, charNoteLower);
      lcd.createChar(0, charNoteEmpty);
      charCreated = 2;
    }
  }
}

void step() {

  if ((stepn % 2) == 0) {
    if (playing) {
      analogWrite(tempoled, 1);
    } else {
      analogWrite(tempoled, 0);
    }
  } else {
    analogWrite(tempoled, 0);
  }

  //partie contrôlant les pas de l'interrupteur, à voir si je laisse
  //comptage du pas en cours
  if (stepn < 9) {
    stepn++;
  }

  if (stepn == 9) {
    stepn = 1;
  }

  //correspondance du pas en cours avec les références de lecture des interrupteurs
  stepn1 = stepn + 10;
  stepn2 = stepn + 20;
  stepn3 = stepn + 30;
  stepn4 = stepn + 40;

  //vérifie l'état de l'interrupteur concerné par le pas
  switch (stepn1) {
    case 11:
      hit1 = !digitalRead(i11);
      break;
    case 12:
      hit1 = !digitalRead(i12);
      break;
    case 13:
      hit1 = !digitalRead(i13);
      break;
    case 14:
      hit1 = !digitalRead(i14);
      break;
    case 15:
      hit1 = !digitalRead(i15);
      break;
    case 16:
      hit1 = !digitalRead(i16);
      break;
    case 17:
      hit1 = !digitalRead(i17);
      break;
    case 18:
      hit1 = !digitalRead(i18);
      break;
  }

  switch (stepn2) {
    case 21:
      hit2 = !digitalRead(i21);
      break;
    case 22:
      hit2 = !digitalRead(i22);
      break;
    case 23:
      hit2 = !digitalRead(i23);
      break;
    case 24:
      hit2 = !digitalRead(i24);
      break;
    case 25:
      hit2 = !digitalRead(i25);
      break;
    case 26:
      hit2 = !digitalRead(i26);
      break;
    case 27:
      hit2 = !digitalRead(i27);
      break;
    case 28:
      hit2 = !digitalRead(i28);
      break;
  }

  switch (stepn3) {
    case 31:
      hit3 = !digitalRead(i31);
      break;
    case 32:
      hit3 = !digitalRead(i32);
      break;
    case 33:
      hit3 = !digitalRead(i33);
      break;
    case 34:
      hit3 = !digitalRead(i34);
      break;
    case 35:
      hit3 = !digitalRead(i35);
      break;
    case 36:
      hit3 = !digitalRead(i36);
      break;
    case 37:
      hit3 = !digitalRead(i37);
      break;
    case 38:
      hit3 = !digitalRead(i38);
      break;
  }

  switch (stepn4) {
    case 41:
      hit4 = !digitalRead(i41);
      break;
    case 42:
      hit4 = !digitalRead(i42);
      break;
    case 43:
      hit4 = !digitalRead(i43);
      break;
    case 44:
      hit4 = !digitalRead(i44);
      break;
    case 45:
      hit4 = !digitalRead(i45);
      break;
    case 46:
      hit4 = !digitalRead(i46);
      break;
    case 47:
      hit4 = !digitalRead(i47);
      break;
    case 48:
      hit4 = !digitalRead(i48);
      break;
  }




  //partie concernant le séquenceur de note
  if (stepnoten <= pattern1lenght) {
    stepnoten++;
    previousstepnoten = stepnoten - 1;
  }

  if (stepnoten == pattern1lenght + 1) {
    stepnoten = 1;
    previousstepnoten = pattern1lenght;
  }

  midisend();

  CursorBlinkCount++;
  if (CursorBlinkCount == 1) {
    displayimmediate();
  }
  if (CursorBlinkCount == 3) {
    cursor();
  }
  if (CursorBlinkCount > 4) {
    CursorBlinkCount = 0;
  }
}

void sequencer() {

  //synchronise la lecture à l'entrée sync ou à l'horloge interne selon la variable syncuse
  if (syncuse) {
    if (analogRead(insyncpin) > 750) {
      tempcount++;
      switch (tempcount) {
        case 1:
          if (playing) { step(); }
          break;
        case 3:
          if (timedivider == 8) {
            if (playing) { step(); }
          }
          break;
        case 6:
          if (timedivider > 2) { tempcount = 0; }
          break;
        case 12:
          tempcount = 0;
          break;
      }
      MIDI.sendClock();
      delay(10);
    }
  } else {

    if (millis() >= previousmillis) {
      previousmillis = millis() + stepduration / 24;
      tempcount++;
      switch (tempcount) {
        case 1:
          if (playing) { step(); }
          break;
        case 3:
          if (timedivider == 8) {
            if (playing) { step(); }
          }
          break;
        case 6:
          if (timedivider > 2) { tempcount = 0; }
          break;
        case 12:
          tempcount = 0;
          break;
      }
      MIDI.sendClock();
    }
  }
  yield();
}

void cursor() {
  yield();
  if (showCursor) {
    lcd.setCursor(CursorCoord, 0);
    lcd.print(" ");
    lcd.setCursor(CursorCoord, 1);
    lcd.print(" ");
  }
}

void midisend() {
  //joue les pas
  if (hit1) {
    MIDI.sendNoteOff(60, 0, Out2CH);
    MIDI.sendNoteOn(60, 127, Out2CH);
  } else {
    MIDI.sendNoteOff(60, 0, Out2CH);
  }

  if (hit2) {
    MIDI.sendNoteOff(60, 0, Out3CH);
    MIDI.sendNoteOn(60, 127, Out3CH);
  } else {
    MIDI.sendNoteOff(60, 0, Out3CH);
  }

  if (hit3) {
    MIDI.sendNoteOff(60, 0, Out4CH);
    MIDI.sendNoteOn(60, 127, Out4CH);
  } else {
    MIDI.sendNoteOff(60, 0, Out4CH);
  }

  if (hit4) {
    MIDI.sendNoteOff(60, 0, Out5CH);
    MIDI.sendNoteOn(60, 127, Out5CH);
  } else {
    MIDI.sendNoteOff(60, 0, Out5CH);
  }

  if (Track1List[stepnoten] != 0) {
    if (Track1List[previousstepnoten] != Track1List[stepnoten]) {
      MIDI.sendNoteOff(Track1List[previousstepnoten] + 59, 0, Out1CH);
    }
    MIDI.sendNoteOff(Track1List[stepnoten] + 59, 0, Out1CH);
    MIDI.sendNoteOn(Track1List[stepnoten] + 59, 127, Out1CH);
  } else {
    MIDI.sendNoteOff(Track1List[previousstepnoten] + 59, 0, Out1CH);
    MIDI.sendNoteOff(Track1List[stepnoten] + 59, 0, Out1CH);
  }
}
