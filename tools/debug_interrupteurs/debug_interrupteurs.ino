//définition des 4 rangs d'interrupteurs
int i11 = 37;
int i12 = 35;
int i13 = 33;
int i14 = 31;
int i15 = 29;
int i16 = 27;
int i17 = 25;
int i18 = 23;

int i21 = 53;
int i22 = 51;
int i23 = 49;
int i24 = 47;
int i25 = 45;
int i26 = 43;
int i27 = 41;
int i28 = 39;

int i31 = 36;
int i32 = 34;
int i33 = 32;
int i34 = 30;
int i35 = 28;
int i36 = 26;
int i37 = 24;
int i38 = 22;

int i41 = 52;
int i42 = 50;
int i43 = 48;
int i44 = 46;
int i45 = 44;
int i46 = 42;
int i47 = 40;
int i48 = 38;



void setup() {

  //initialisation à 9600 bauds
  Serial.begin(9600);

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
}

void loop() {

  Serial.print(!digitalRead(i11));
  Serial.print(!digitalRead(i12));
  Serial.print(!digitalRead(i13));
  Serial.print(!digitalRead(i14));
  Serial.print(!digitalRead(i15));
  Serial.print(!digitalRead(i16));
  Serial.print(!digitalRead(i17));
  Serial.print(!digitalRead(i18));


  Serial.println();

  /*
Serial.print(!digitalRead(i21));
Serial.print(!digitalRead(i22));
Serial.print(!digitalRead(i23));
Serial.print(!digitalRead(i24));
Serial.print(!digitalRead(i25));
Serial.print(!digitalRead(i26));
Serial.print(!digitalRead(i27));
Serial.print(!digitalRead(i28));

Serial.println();

Serial.print(!digitalRead(i31));
Serial.print(!digitalRead(i32));
Serial.print(!digitalRead(i33));
Serial.print(!digitalRead(i34));
Serial.print(!digitalRead(i35));
Serial.print(!digitalRead(i36));
Serial.print(!digitalRead(i37));
Serial.print(!digitalRead(i38));

Serial.println();

Serial.print(!digitalRead(i41));
Serial.print(!digitalRead(i42));
Serial.print(!digitalRead(i43));
Serial.print(!digitalRead(i44));
Serial.print(!digitalRead(i45));
Serial.print(!digitalRead(i46));
Serial.print(!digitalRead(i47));
Serial.print(!digitalRead(i48));

Serial.println();
*/
}
