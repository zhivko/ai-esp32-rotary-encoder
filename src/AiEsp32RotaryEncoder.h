// AiEsp32RotaryEncoder.h
// based on https://github.com/marcmerlin/IoTuz code - extracted and modified Encoder code

#ifndef _AIESP32ROTARYENCODER_h
#define _AIESP32ROTARYENCODER_h

#include "Arduino.h"

// Rotary Encocer
#define AIESP32ROTARYENCODER_DEFAULT_A_PIN 25
#define AIESP32ROTARYENCODER_DEFAULT_B_PIN 26
#define AIESP32ROTARYENCODER_DEFAULT_BUT_PIN 15
#define AIESP32ROTARYENCODER_DEFAULT_VCC_PIN -1

typedef enum {
	BUT_DOWN = 0,
	BUT_PUSHED = 1,
	BUT_UP = 2,
	BUT_RELEASED = 3,
	BUT_DISABLED = 99,
} ButtonState;

class AiEsp32RotaryEncoder {
	
private:
	int8_t ENC_PORT;
	bool _circleValues = false;
	bool isEnabled = true;

	int32_t _minEncoderValue = -1 << 15;
	int32_t _maxEncoderValue = 1 << 15;

	int16_t lastReadEncoder0Pos;
	bool previous_butt_state;

public:

	volatile int32_t encoder0Pos = 0;

	uint8_t encoderAPin      = AIESP32ROTARYENCODER_DEFAULT_A_PIN;
	uint8_t encoderBPin      = AIESP32ROTARYENCODER_DEFAULT_B_PIN;
	uint8_t encoderButtonPin = AIESP32ROTARYENCODER_DEFAULT_BUT_PIN;
	uint8_t encoderVccPin    = AIESP32ROTARYENCODER_DEFAULT_VCC_PIN;
	uint8_t phase;
	uint8_t phasep;

	AiEsp32RotaryEncoder(
		uint8_t _encoderAPin,
		uint8_t _encoderBPin,
		uint8_t _encoderButtonPin,
		uint8_t _encoderVccPin
	);
	void setBoundaries(int32_t minValue = -100, int32_t maxValue = 100, bool circleValues = false);
	portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
	void IRAM_ATTR readEncoder_ISR();
	
	void setup(void (*ISR_callback)(void));
	void begin();
	void reset(int32_t newValue);
	void enable();
	void disable();
	int16_t readEncoder();
	int16_t encoderChanged();
	ButtonState currentButtonState();
};
#endif

