
#ifndef IR_remote_h
#define IR_remote_h

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

#ifdef ME_PORT_DEFINED
#endif // ME_PORT_DEFINED
#ifndef __AVR_ATmega32U4__
#define MARK  0
#define SPACE 1
#define NEC_BITS 32
#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 80 // Length of raw duration buffer

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define NEC_HDR_MARK	9000
#define NEC_HDR_SPACE	4500
#define NEC_BIT_MARK	560
#define NEC_ONE_SPACE	1600
#define NEC_ZERO_SPACE	560
#define NEC_RPT_SPACE	2250
#define NEC_RPT_PERIOD	110000


#define _GAP 5000 // Minimum map between transmissions

// receiver states
#define STATE_IDLE     2
#define STATE_MARK     3
#define STATE_SPACE    4
#define STATE_STOP     5


// Values for decode_type
#define NEC 1
#define SONY 2
#define RC5 3
#define RC6 4
#define DISH 5
#define SHARP 6
#define PANASONIC 7
#define JVC 8
#define SANYO 9
#define MITSUBISHI 10
#define SAMSUNG 11
#define LG 12
#define UNKNOWN -1

#define TOPBIT 0x80000000


#ifdef F_CPU
#define SYSCLOCK F_CPU     // main Arduino clock
#else
#define SYSCLOCK 16000000  // main Arduino clock
#endif


#define _GAP 5000 // Minimum map between transmissions
#define GAP_TICKS (_GAP/USECPERTICK)


#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
#define TIMER_ENABLE_PWM     (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_PWM    (TCCR2A &= ~(_BV(COM2B1)))
#define TIMER_ENABLE_INTR    (TIMSK2 = _BV(OCIE2A))
#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
#define TIMER_INTR_NAME      TIMER2_COMPA_vect
#define TIMER_CONFIG_KHZ(val) ({ \
  const uint8_t pwmval = F_CPU / 2000 / (val); \
  TCCR2A = _BV(WGM20); \
  TCCR2B = _BV(WGM22) | _BV(CS20); \
  OCR2A = pwmval; \
  OCR2B = pwmval / 3; \
})

#define TIMER_COUNT_TOP      (SYSCLOCK * USECPERTICK / 1000000)
#if (TIMER_COUNT_TOP < 256)
#define TIMER_CONFIG_NORMAL() ({ \
  TCCR2A = _BV(WGM21); \
  TCCR2B = _BV(CS20); \
  OCR2A = TIMER_COUNT_TOP; \
  TCNT2 = 0; \
})
#else
#define TIMER_CONFIG_NORMAL() ({ \
  TCCR2A = _BV(WGM21); \
  TCCR2B = _BV(CS21); \
  OCR2A = TIMER_COUNT_TOP / 8; \
  TCNT2 = 0; \
})
#endif

// information for the interrupt handler
typedef struct {
  uint8_t recvpin;           // pin for IR data from detector
  volatile uint8_t rcvstate;          // state machine
  volatile uint32_t lastTime;
  unsigned int timer;     // 
  volatile uint8_t rawbuf[RAWBUF]; // raw data
  volatile uint8_t rawlen;         // counter of entries in rawbuf
} irparams_t;


class IRremote
{
public:

  IRremote(int pin);
  ErrorStatus decode();
  void begin();
  void end();
  void loop();
  boolean keyPressed(unsigned char r);
  // void resume();
   
  int8_t decode_type; // NEC, SONY, RC5, UNKNOWN
  unsigned long value; // Decoded value
  uint8_t bits; // Number of bits in decoded value
  volatile uint8_t *rawbuf; // Raw intervals in .5 us ticks
  int rawlen; // Number of records in rawbuf.
  String getString();
  unsigned char getCode();
  String getKeyMap( byte keycode);
  byte getIrKey(byte keycode);
  void sendString(String s);
  void sendString(float v);
  void sendNEC(unsigned long data, int nbits);
  void sendRaw(unsigned int buf[], int len, uint8_t hz);
  void enableIROut(uint8_t khz);
  void enableIRIn();
  void mark(uint16_t us);
  void space(uint16_t us);
private:
  ErrorStatus decodeNEC();
  int16_t irIndex;	
  char irRead;
  char floatString[5];
  boolean irReady;
  boolean irPressed;
  String irBuffer;
  String Pre_Str;
  double irDelayTime;
};
#endif // !__AVR_ATmega32U4__
#endif

