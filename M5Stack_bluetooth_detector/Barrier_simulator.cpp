#include "Barrier_simulator.h"


BarrierSimulator::BarrierSimulator() :
	m_state {CLOSED}, m_display {true}
{ };

BarrierSimulator& BarrierSimulator::getBarrierSimulator() {
	static BarrierSimulator barrierSimulator;
	return barrierSimulator;
}

void BarrierSimulator::setDisplay(bool p_display) {
	m_display = p_display;
}

BarrierSimulator::BarrierState BarrierSimulator::getState() const {
	return m_state;
}

void BarrierSimulator::open() {
	m_state = OPEN;
	if (m_display) {
		M5.Lcd.clear(GREEN);
		M5.Lcd.setTextColor(WHITE);
		M5.Lcd.setTextDatum(MC_DATUM);
		M5.Lcd.setTextSize(12);
		M5.Lcd.drawString("open", 160, 120);
	}
}

void BarrierSimulator::close() {
	m_state = CLOSED;
	if (m_display) {
		M5.Lcd.clear(RED);
		M5.Lcd.setTextColor(WHITE);
		M5.Lcd.setTextDatum(MC_DATUM);
		M5.Lcd.setTextSize(12);
		M5.Lcd.drawString("closed", 160, 120);
	}
}

void BarrierSimulator::handleOpCode(const char* opCode) {
	if(opCode == nullptr){return;}
	switch (opCode[0]) {
	case BARRIER_ACTION_OPEN:
		open();
		break;
	case  BARRIER_ACTION_CLOSE:
		close();
		break;
	default:
		M5.Lcd.print("unknown op code: ");
		M5.Lcd.println(opCode);
	}
}
