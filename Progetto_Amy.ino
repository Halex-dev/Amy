#include <Counter.h>
#include <Servo.h>
#include <TMRpcm.h>
#include <LedControl.h>
#include <SoftwareSerial.h>
#include <math.h>
//------------------------ Altro -------------------------------
bool Segui = LOW;

/*//------------------------ COUNT -------------------------------
  Counter CountEyes;
  Counter Sbug;

  bool SbugFix = HIGH;
  bool Close = HIGH;*/
//------------------------ VOICE -------------------------------
 
TMRpcm Voice;
char mychar;

bool misu = LOW;
/*/------------------------ SD ----------------------------------

  53 = CS
  52 = SCK
  51 = MOSI
  50 = MISO*/


//--------------------- 8X8 MATRIX ---------------------------
LedControl lc = LedControl(41, 45, 43, 1); // Pins: DIN,CLK, CS, # display connessi

byte Happy[] = {
  B00000000,
  B00100110,
  B01000110,
  B01000000,
  B01000000,
  B01000110,
  B00100110,
  B00000000
};

byte HappyClosedEyes[] = {
  B00000000,
  B00100100,
  B01000100,
  B01000000,
  B01000000,
  B01000100,
  B00100100,
  B00000000
};

byte HappyLeft[] = {
  B00000000,
  B00100110,
  B01000000,
  B01000000,
  B01000000,
  B01000110,
  B00100000,
  B00000000
};

byte HappyRight[] = {
  B00000000,
  B00100000,
  B01000110,
  B01000000,
  B01000000,
  B01000000,
  B00100110,
  B00000000
};

/*//---------------------- JOYSTICK -----------------------------
  const int joystickX = A8;         // joystick X axis
  const int joystickY = A9;         // joystick Y axis
  const int joystickMS = 11;         // joystick MS

  int VarX = 0;
  int VarY = 0;
  int VarMS = 0;

  bool premi = HIGH;*/

//---------------------- BLUETOOTH ----------------------------
String voice;

//------------------ BRACCIO MECCANICO 1 DESTRA ----------------------
Servo Spalla;
Servo Braccio;
Servo Mano;

void MuoviSpallaDestra(int valore);
void MuoviBraccioDestra(int valore);
void MuoviManoDestra(int valore);

int spalla1 = 80;
int braccio1 = 80;
int mano1 = 80;

//------------------ BRACCIO MECCANICO 2 SINISTRA ----------------------
Servo Spalla2;
Servo Braccio2;
Servo Mano2;

void MuoviSpallaSinistra(int valore);
void MuoviBraccioSinistra(int valore);
void MuoviManoSinistra(int valore);

int spalla2 = 80;
int braccio2 = 80;
int mano2 = 80;

//------------------------ TESTA ------------------------------

Servo ServoBase; //26
Servo ServoTesta; //24

const int Sopra = A6;
const int DxGiu = A5;
const int SxGiu = A7;

int valoreTesta = 50;
int valoreBase = 80;

bool Testa = HIGH;
bool BassoDx = HIGH;
bool BassoSx = HIGH;

//------------------------- SD ----------------------------------
bool SDFAIL = true;

void setup()
{
  //--------------- Altro -------------------------
  Serial.begin(9600);

  //---------------------- VOICE ------------------------------
  Voice.speakerPin = 11;
  pinMode(11, OUTPUT);

  while(SDFAIL)
  {
    if (!SD.begin(53))
    {
      Serial.println("SD fail");
      SDFAIL = true;
    }
    else
    {
      SDFAIL = false;
    }
    delay(2000);
  }
  Serial.println("SD ready");

  //--------------- 8x8 pt. 1 --------------------------
  lc.shutdown(0, true);
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);

  LedOn(HappyClosedEyes);

  //---------- Braccio destra --------------------
    Spalla.attach(28);
    Braccio.attach(30);
    Mano.attach(32);

    MuoviSpallaDestra(145);
    MuoviBraccioDestra(90);
    MuoviManoDestra(180);

    Serial.print("Braccio Destro startato \n");
    delay(3000);

    //---------- Braccio Sinistra --------------------
    Spalla2.attach(34);
    Braccio2.attach(36);
    Mano2.attach(38);

    MuoviSpallaSinistra(0);
    MuoviBraccioSinistra(70);
    MuoviManoSinistra(165);

    Serial.print("Braccio Sinistro startato \n");
    delay(3000);

    //--------------- Testa ------------------------
    ServoBase.attach(26);
    ServoTesta.attach(24);

    pinMode(Sopra,INPUT);
    pinMode(DxGiu,INPUT);
    pinMode(SxGiu,INPUT);

    ServoTesta.write(valoreTesta);
    ServoBase.write(valoreBase);

    Serial.print("Testa startata \n");
    delay(1000);

   /* //--------------- COUNTER -----------------------
    //CountEyes.Start();
    //Sbug.Start();

    //--------------------- joystick ----------------------------
    /*pinMode(joystickX,INPUT);
    pinMode(joystickY,INPUT);
    pinMode(joystickMS,INPUT);

    digitalWrite(joystickMS, HIGH);

    //----------------------- 8X8 -------------------------------*/

  LedOn(Happy);
  delay(5000);
  Voice.play("papu.wav");
  delay(1000);
  //MuoviSpallaSinistra(0);
  //MuoviSpallaDestra(180);
}

void loop()
{

  //----------- BLUETOOTH --------------------

  while (Serial.available())
  {
    delay(10);
    char c = Serial.read();
    if (c == '#') {
      break;
    }
    voice += c;
  }
  if (voice.length() > 0)
  {
    Serial.println(voice);

    if (voice == "*saluta" || voice == "*ciao")
    {
      Voice.play("Ciao.wav");
    }
    else if (voice == "*come ti chiami?")
    {
      Voice.play("IlMioNome.wav");
    }
    else if (voice == "*cosa sai fare?")
    {
      Voice.play("Masioni.wav");
    }
    else if (voice == "*chi è tuo padre?")
    {
      Voice.play("Padre.wav");
    }
    else if (voice == "*sei maschio o femmina?")
    {
      Voice.play("Asessuata.wav");
    }
    else if (voice == "*Ti amo")
    {
      Voice.play("TiAmo.wav");
    }
    else if (voice == "*cantami una canzone")
    {
      int rando = random(0, 2);

      if (rando == 0)
      {
        Voice.play("Bamba.wav");
      }
      else if (rando == 1)
      {
        Voice.play("Cucu.wav");
      }
      else if (rando == 2)
      {
        Voice.play("Italia.wav");
      }

    }
    else if (voice == "*raccontami una barzelletta")
    {
      int rando = random(0, 2);

      if (rando == 0)
      {
        Voice.play("Giraffa.wav");
      }
      else if (rando == 1)
      {
        Voice.play("Lattuga.wav");
      }
      else if (rando == 2)
      {
        Voice.play("Maio.wav");
      }
    }
    else if (voice == "*dimmi qualcosa")
    {
      int rando = random(0, 2);

      if (rando == 0)
      {
        Voice.play("C1.wav");
      }
      else if (rando == 1)
      {
        Voice.play("C2.wav");
      }

    }
    else if (voice == "*rileva distanza")
    {

    }
    else if (voice == "*sposta l'oggetto alla tua destra")
    {
      MuoviSpallaDestra(145);//145
      MuoviManoDestra(120);//180
      MuoviBraccioDestra(20);//90

      MuoviSpallaDestra(145);//145
      MuoviManoDestra(120);//180
      MuoviBraccioDestra(20);//90
    }
    else if (voice == "*sposta l'oggetto alla tua sinistra")
    {

      MuoviManoDestra(100);//0
      delay(2000);
      MuoviManoDestra(180);//0
      delay(2000);
      MuoviManoDestra(50);//0
      delay(2000);
      MuoviManoDestra(0);//0

      MuoviManoSinistra(100);//0
      MuoviBraccioSinistra(140); //50
      delay(2000);

      MuoviManoSinistra(0);//0
      MuoviBraccioSinistra(50); //50
      MuoviSpallaSinistra(170); //0
      delay(2000);

      MuoviBraccioSinistra(100); //50
      MuoviManoSinistra(50);//0
      MuoviBraccioSinistra(50); //50
      delay(2000);

      MuoviSpallaSinistra(0);
      MuoviBraccioSinistra(50);
      MuoviManoSinistra(0);
      delay(2000);

    }
    else if (voice == "*seguimi")
    {
      Segui = HIGH;
    }
    else if (voice == "*fermo")
    {
      Segui = LOW;
    }
    //---------------EASTER EGG------------------
    else if (voice == "*che hai?")
    {
      Voice.play("EasterGirl.wav");
    }
    else if (voice == "*papu")
    {
      Voice.play("papu.wav");

      MuoviSpallaSinistra(0);
      MuoviSpallaDestra(180);

      MuoviBraccioSinistra(140); //50
      delay(2000);

      MuoviManoSinistra(0);//0
      MuoviBraccioSinistra(50); //50
      MuoviSpallaSinistra(170); //0
      delay(2000);

      MuoviBraccioSinistra(100); //50
      MuoviManoSinistra(50);//0
      MuoviBraccioSinistra(50); //50
      delay(2000);

      MuoviSpallaSinistra(0);
      MuoviBraccioSinistra(50);
      MuoviManoSinistra(0);
      delay(2000);
    }

    else
    {
      if (misu == LOW)
      {
        Voice.play("NonHoCapitoBene.wav");
        misu = HIGH;
      }
      else
      {
        int rando = random(0, 1);

        if (rando == 0)
        {
          Voice.play("NonProgrammata.wav");
        }
        else if (rando == 1)
        {
          Voice.play("NonSoRispondere.wav");
        }

        misu = LOW;
      }
    }

    voice = "";
  }
  //------------------------ ANALOGIC ----------------

  /*VarX = analogRead(joystickX);
    VarY = analogRead(joystickY);
    VarMS = digitalRead(joystickMS);

    Serial.print("   SPALLA=");
    Serial.print(spalla1);
    Serial.print("   BRACCIO=");
    Serial.print(braccio1);
    Serial.print("   MANO=");
    Serial.print(mano1);

    VarX = map(VarX, 0, 1023, 0, 180);
    VarY = map(VarY, 0, 1023, 0, 180);

    MuoviSpalla(VarX);
    MuoviBraccio(VarY);

    Serial.print("  X=");
    Serial.print(VarX);

    Serial.print("   Y=");
    Serial.print(VarY);

    Serial.print("   MS=");
    Serial.print(VarMS);
    Serial.print("\n");

    if(VarMS == LOW)
    {
    if(premi == HIGH)
    {
      premi = LOW;
      MuoviMano(0);
    }
    else
    {
      premi = HIGH;
      MuoviMano(30);
    }
    }
  */

  //-------------------- TESTA -----------------------
  if (Segui == HIGH)
  {
    Testa = digitalRead(Sopra);
    delay(5);
    BassoDx = digitalRead(DxGiu);
    delay(5);
    BassoSx = digitalRead(SxGiu);

    if (BassoSx == LOW && Testa == LOW && BassoDx == LOW) //Fermo
    {
      valoreTesta += 0;
      valoreBase += 0;
      ServoTesta.write(valoreTesta);
      ServoBase.write(valoreBase);
    }
    else if (BassoSx == LOW && Testa == LOW && BassoDx == HIGH) //Sinistra
    {
      valoreBase -= 1;
      ServoBase.write(valoreBase);
    }
    else if (BassoDx == LOW && Testa == LOW && BassoSx == HIGH) //Destra
    {
      valoreBase += 1;
      ServoBase.write(valoreBase);
    }
    else if (BassoDx == LOW && BassoSx == LOW && Testa == HIGH) //Giu
    {
      valoreTesta += 1;
      ServoTesta.write(valoreTesta);
    }
    else if (Testa == LOW) // Sopra
    {
      valoreTesta -= 1;
      ServoTesta.write(valoreTesta);
    }
    else if (BassoDx == LOW) //basso destra
    {
      valoreTesta += 1;
      valoreBase += 1;
      ServoTesta.write(valoreTesta);
      ServoBase.write(valoreBase);

    }
    else if (BassoSx == LOW) //basso a sinistra
    {
      valoreTesta += 1;
      valoreBase -= 1;
      ServoTesta.write(valoreTesta);
      ServoBase.write(valoreBase);
    }

    if (valoreTesta > 180 && valoreTesta < 0)
    {
      if (valoreTesta > 180)
      {
        valoreTesta = 180;
      }
      else
      {
        valoreTesta = 0;
      }
    }

    if (valoreBase > 180 && valoreBase < 0)
    {
      if (valoreBase > 180)
      {
        valoreBase = 180;
      }
      else
      {
        valoreBase = 0;
      }
    }
  }
}

void LedOn(byte led[])
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, led[i]);
    delay(1);
  }
}

void MuoviSpallaDestra(int valore)
{
  Segui = LOW;
  if (spalla1 != valore)
  {

    if (spalla1 < valore)
    {
      for (spalla1; spalla1 <= valore; spalla1++)
      {
        Spalla.write(spalla1);
        delay(10);
      }
    }
    else
    {
      for (spalla1; spalla1 >= valore; spalla1--)
      {
        Spalla.write(spalla1);
        delay(10);
      }
    }
    spalla1 = valore;
  }
  Segui = HIGH;
}

void MuoviBraccioDestra(int valore)
{
  Segui = LOW;
  if (braccio1 != valore)
  {
    if (braccio1 < valore)
    {
      for (braccio1; braccio1 <= valore; braccio1++)
      {
        Braccio.write(braccio1);
        delay(10);
      }
    }
    else
    {
      for (braccio1; braccio1 >= valore; braccio1--)
      {
        Braccio.write(braccio1);
        delay(10);
      }
    }
    braccio1 = valore;
  }
  Segui = HIGH;
}

void MuoviManoDestra(int valore)
{
  Segui = LOW;
  if (mano1 != valore)
  {
    if (mano1 < valore)
    {
      for (mano1; mano1 <= valore; mano1++)
      {
        Mano.write(mano1);
        delay(10);
      }
    }
    else
    {
      for (mano1; mano1 >= valore; mano1--)
      {
        Mano.write(mano1);
        delay(10);
      }
    }
    mano1 = valore;
  }
  Segui = HIGH;
}

void MuoviSpallaSinistra(int valore)
{
  Segui = LOW;
  if (spalla2 != valore)
  {
    if (spalla2 < valore)
    {
      for (spalla2; spalla2 <= valore; spalla2++)
      {
        Spalla2.write(spalla2);
        delay(10);
      }
    }
    else
    {
      for (spalla2; spalla2 >= valore; spalla2--)
      {
        Spalla2.write(spalla2);
        delay(10);
      }
    }
    spalla2 = valore;
  }
  Segui = HIGH;
}

void MuoviBraccioSinistra(int valore)
{
  Segui = LOW;
  if (braccio2 != valore)
  {
    if (braccio2 < valore)
    {
      for (braccio2; braccio2 <= valore; braccio2++)
      {
        Braccio2.write(braccio2);
        delay(10);
      }
    }
    else
    {
      for (braccio2; braccio2 >= valore; braccio2--)
      {
        Braccio2.write(braccio2);
        delay(10);
      }
    }
    braccio2 = valore;
  }
  Segui = HIGH;
}

void MuoviManoSinistra(int valore)
{
  Segui = LOW;
  if (mano2 != valore)
  {
    if (mano2 < valore)
    {
      for (mano2; mano2 <= valore; mano2++)
      {
        Mano2.write(mano2);
        delay(10);
      }
    }
    else
    {
      for (mano2; mano2 >= valore; mano2--)
      {
        Mano2.write(mano2);
        delay(10);
      }
    }
    mano2 = valore;
  }
  Segui = HIGH;
}


