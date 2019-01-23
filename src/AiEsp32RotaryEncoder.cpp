// based on https://github.com/marcmerlin/IoTuz code - extracted and modified Encoder code
// 
// 

#include "AiEsp32RotaryEncoder.h"


AiEsp32RotaryEncoder::AiEsp32RotaryEncoder(uint8_t _encoder_APin, uint8_t _encoder_BPin, uint8_t _encoder_ButtonPin, uint8_t _encoder_VccPin)
{
	this->phase = 0;
	this->phasep = 0;
	
	this->encoderAPin = _encoder_APin;
	this->encoderBPin = _encoder_BPin;
	this->encoderButtonPin = _encoder_ButtonPin;
	this->encoderVccPin = _encoder_VccPin;
	//pinMode(this->encoderAPin, INPUT_PULLUP);
	//pinMode(this->encoderBPin, INPUT_PULLUP);
}

void AiEsp32RotaryEncoder::setBoundaries(int32_t minEncoderValue, int32_t maxEncoderValue, bool circleValues)
{
	this->_minEncoderValue = minEncoderValue * 2;
	this->_maxEncoderValue = maxEncoderValue * 2;
	this->_circleValues = circleValues;
}



int16_t AiEsp32RotaryEncoder::readEncoder()
{
	return (this->encoder0Pos / 2);
}

int16_t AiEsp32RotaryEncoder::encoderChanged() {
	int16_t _encoder0Pos = readEncoder();
	
	int16_t encoder0Diff = _encoder0Pos - this->lastReadEncoder0Pos;

	this->lastReadEncoder0Pos = _encoder0Pos;
	return encoder0Diff;
}

void AiEsp32RotaryEncoder::setup(void (*ISR_callback)(void))
{
	attachInterrupt(digitalPinToInterrupt(this->encoderAPin), ISR_callback, CHANGE);
	attachInterrupt(digitalPinToInterrupt(this->encoderBPin), ISR_callback, CHANGE);
}


void AiEsp32RotaryEncoder::begin()
{
	this->lastReadEncoder0Pos = 0;
	//Serial.begin(115200);
	if (this->encoderVccPin > 0) {
		pinMode(this->encoderVccPin, OUTPUT);	digitalWrite(this->encoderVccPin, 1);//Vcc for encoder 
	}
	//Serial.println("Enable rotary encoder ISR:");
	// Initialize rotary encoder reading and decoding
	this->previous_butt_state = 0;
	if (this->encoderButtonPin > 0) {
		pinMode(this->encoderButtonPin, INPUT_PULLUP);
	}
}

ButtonState AiEsp32RotaryEncoder::currentButtonState()
{
	if (!this->isEnabled) {
		return BUT_DISABLED;
	}

	uint8_t butt_state = !digitalRead(this->encoderButtonPin);


	if (butt_state && !this->previous_butt_state)
	{
		this->previous_butt_state = true;
		//Serial.println("Button Pushed");
		return BUT_PUSHED;
	}
	if (!butt_state && this->previous_butt_state)
	{
		this->previous_butt_state = false;
		//Serial.println("Button Released");
		return BUT_RELEASED; 
	}
	return (butt_state ? BUT_DOWN : BUT_UP);
}

void AiEsp32RotaryEncoder::reset(int32_t newValue_) {
	newValue_ = newValue_ * 2;
	this->encoder0Pos = newValue_;

	if (this->encoder0Pos > this->_maxEncoderValue) this->encoder0Pos = this->_circleValues ? this->_minEncoderValue : this->_maxEncoderValue;
	if (this->encoder0Pos < this->_minEncoderValue) this->encoder0Pos = this->_circleValues ? this->_maxEncoderValue : this->_minEncoderValue;

	Serial.printf("\nReseted encoder to value: %d\n", this->encoder0Pos);
}

void AiEsp32RotaryEncoder::enable() {
	this->isEnabled = true;
}
void AiEsp32RotaryEncoder::disable() {
	this->isEnabled = false;
}
