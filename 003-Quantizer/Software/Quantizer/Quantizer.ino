// #define  ENCODER_OPTIMIZE_INTERRUPTS //エンコーダノイズ対策
#include <Wire.h>
// #include <Encoder.h>

const byte triggerPin = 2;


//CV入出力用定数
float CV_IN = 512;
int CV_INh = 10;
int i = 0;

int scale = 0;

float oldPosition  = -999;
float newPosition = -999;

bool arm = true;

//--------------------scale list---------------------------------
//0=major
const static word DAC_LSB_maj[61] PROGMEM = {
 0,  137,  205,  341,  478,  546,  683,  819,  956,  1024, 1161, 1297, 1365, 1502, 1638, 1775, 1843, 1980, 2116, 2185, 2321, 2458, 2594, 2662, 2799, 2935, 3004, 3140, 3277, 3413, 3482, 3618, 3755, 3823, 3959, 4095
};

const static word CVIN_th_maj[62] PROGMEM = {
 0,  15, 44, 73, 102,  131,  160,  189,  218,  247,  276,  305,  334,  363,  392,  421,  450,  479,  508,  537,  566,  595,  624,  653,  682,  711,  740,  769,  798,  827,  856,  885,  914,  943,  972,  1001, 1024
};

//1=major_pentatonic
const static word DAC_LSB_mp[61] PROGMEM = {
 0,  205,  341,  478,  683,  819,  1024, 1161, 1297, 1502, 1638, 1843, 1980, 2116, 2321, 2458, 2662, 2799, 2935, 3140, 3277, 3482, 3618, 3755, 3959, 4095
};

const static word CVIN_th_mp[62] PROGMEM = {
 0, 20,  61, 102,  143,  184,  225,  266,  307,  348,  389,  430,  471,  512,  553,  594,  635,  676,  717,  758,  799,  840,  881,  922,  963,  1004, 1024
};

//2=major_pentatonic+7
const static word DAC_LSB_mp7[61] PROGMEM = {
 0,  137,  205,  341,  478,  683,  819,  956,  1024, 1161, 1297, 1502, 1638, 1775, 1843, 1980, 2116, 2321, 2458, 2594, 2662, 2799, 2935, 3140, 3277, 3413, 3482, 3618, 3755, 3959, 4095
};

const static word CVIN_th_mp7[62] PROGMEM = {
 0, 17,  51, 85, 119,  153,  187,  221,  255,  289,  323,  357,  391,  425,  459,  493,  527,  561,  595,  629,  663,  697,  731,  765,  799,  833,  867,  901,  935,  969,  1003, 1024
};

//3=harmonic_minor
const static word DAC_LSB_hm[61] PROGMEM = {
 68,  137,  205,  341,  410,  546,  683,  887,  956,  1024, 1161, 1229, 1365, 1502, 1707, 1775, 1843, 1980, 2048, 2185, 2321, 2526, 2594, 2662, 2799, 2867, 3004, 3140, 3345, 3413, 3482, 3618, 3686, 3823, 3959
};

const static word CVIN_th_hm[62] PROGMEM = {
 0, 15,  45, 75, 105,  135,  165,  195,  225,  255,  285,  315,  345,  375,  405,  435,  465,  495,  525,  555,  585,  615,  645,  675,  705,  735,  765,  795,  825,  855,  885,  915,  945,  975,  1005, 1024
};

//4=monor_pentatonic
const static word DAC_LSB_minp[61] PROGMEM = {
 205,  341,  410,  683,  751,  1024, 1161, 1229, 1502, 1570, 1843, 1980, 2048, 2321, 2389, 2662, 2799, 2867, 3140, 3209, 3482, 3618, 3686, 3959, 4028
};

const static word CVIN_th_minp[62] PROGMEM = {
 0, 21, 64, 107,  150,  193,  236,  279,  322,  365,  408,  451,  494,  537,  580,  623,  666,  709,  752,  795,  838,  881,  924,  967,  1010, 1024
};


//5=chromatic
const static word DAC_LSB_chr[61] PROGMEM = {
 0,  68, 137,  205,  273,  341,  410,  478,  546,  614,  683,  751,  819,  887,  956,  1024, 1092, 1161, 1229, 1297, 1365, 1434, 1502, 1570, 1638, 1707, 1775, 1843, 1911, 1980, 2048, 2116, 2185, 2253, 2321, 2389, 2458, 2526, 2594, 2662, 2731, 2799, 2867, 2935, 3004, 3072, 3140, 3209, 3277, 3345, 3413, 3482, 3550, 3618, 3686, 3755, 3823, 3891, 3959, 4028, 4095
};

const static word CVIN_th_chr[62] PROGMEM = {
 0, 9,  26, 43, 60, 77, 94, 111,  128,  145,  162,  179,  196,  213,  230,  247,  264,  281,  298,  315,  332,  349,  366,  383,  400,  417,  434,  451,  468,  485,  502,  519,  536,  553,  570,  587,  604,  621,  638,  655,  672,  689,  706,  723,  740,  757,  774,  791,  808,  825,  842,  859,  876,  893,  910,  927,  944,  961,  978,  995,  1012, 1024
};

//6=octave
const static word DAC_LSB_oct[61] PROGMEM = {
 0,  819,  1638, 2458, 3277, 4095
};

const static word CVIN_th_oct[62] PROGMEM = {
 0, 102, 307,  512,  717,  922,  1024
};

void setup() {
  pinMode(5, OUTPUT); //gate_out
  pinMode(triggerPin, INPUT);
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  scale = 1;

  if (digitalRead(triggerPin) == HIGH && arm==true) {
    arm = false;
    evaluateCV();
  }

  if (digitalRead(triggerPin == LOW)) arm = true;
}

void DAC(long CV_OUT) {
  digitalWrite(5, HIGH);

  Wire.beginTransmission(0x60);
  Wire.write((CV_OUT >> 8) & 0x0F);
  Wire.write(CV_OUT);
  Wire.endTransmission();
  delay(5);
  digitalWrite(5, LOW);
}

void evaluateCV(){
  CV_IN = analogRead(7);

  for ( i = 0; i <= 61 ; i++ ) {
    switch (scale) {
      case 0://0=major
        if ( CV_IN >= (pgm_read_word(&(CVIN_th_maj[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_maj[i + 1])))) {
          CV_INh = i;
          DAC(pgm_read_word(&(DAC_LSB_maj[CV_INh])));
          return;
        }
        break;

      case 1://1=major_pentatonic
        if ( CV_IN >= (pgm_read_word(&(CVIN_th_mp[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_mp[i + 1])))) {
          CV_INh = i;
          DAC(pgm_read_word(&(DAC_LSB_mp[CV_INh])));
          return;
        }
        break;

      case 2://2=major_pentatonic+7
        if ( CV_IN >= (pgm_read_word(&(CVIN_th_mp7[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_mp7[i + 1])))) {
          CV_INh = i;
          DAC(pgm_read_word(&(DAC_LSB_mp7[CV_INh])));
          return;
        }
        break;

      case 3://3=harmonic_minor
        if ( CV_IN >= (pgm_read_word(&(CVIN_th_hm[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_hm[i + 1])))) {
          CV_INh = i;
          DAC(pgm_read_word(&(DAC_LSB_hm[CV_INh])));
          return;
        }
        break;

      case 4://4=monor_pentatonic
        if ( CV_IN >= (pgm_read_word(&(CVIN_th_minp[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_minp[i + 1])))) {
          CV_INh = i;\
          DAC(pgm_read_word(&(DAC_LSB_minp[CV_INh])));
          return;
        }
        break;

      case 5://5=chromatic
        if ( CV_IN >= (pgm_read_word(&(CVIN_th_chr[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_chr[i + 1])))) {
          CV_INh = i;
          DAC(pgm_read_word(&(DAC_LSB_chr[CV_INh])));
          return;
        }
        break;

      case 6://6=octave
        if ( CV_IN >= (pgm_read_word(&(CVIN_th_oct[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_oct[i + 1])))) {
          CV_INh = i;
          DAC(pgm_read_word(&(DAC_LSB_oct[CV_INh])));
          return;
        }
        break;
    }
  }
}

