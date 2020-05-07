#include "Barrier_simulator.h"


BarrierSimulator::BarrierSimulator(const BarrierInfo& barrierInfo) :
	m_state {CLOSED}, m_display {true}
{
  m_barrierInfo = barrierInfo;
};



void BarrierSimulator::setDisplay(bool p_display) {
	m_display = p_display;
}

BarrierSimulator::BarrierState BarrierSimulator::getState() const {
	return m_state;
}

const char* BarrierSimulator::getBarrierType() const{
  return m_barrierInfo.getType();
}

bool BarrierSimulator::checkBarrierId(unsigned long idToCompare) const{
  return m_barrierInfo.id == idToCompare;
}

unsigned long BarrierSimulator::getBarrierId() const{
  return m_barrierInfo.id;
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

void BarrierSimulator::toogleBarrierState(){
  if(m_state == CLOSED){
    open();
  }else{
    close();
  }
}

void BarrierSimulator::showBarrierType(){
  if (m_display) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextSize(12);
    M5.Lcd.drawString(getBarrierType(), 160, 120);
  }
}
