#ifndef BARRIER_SIMULATOR_H
#define BARRIER_SIMULATOR_H
#include "Barrier_orders.h"
#include <M5Stack.h>

class BarrierSimulator {
public:
	enum BarrierState {
		CLOSED,
		OPEN
	};
private:
	BarrierState m_state;	// state of the barrier
	bool m_display;			// whether display to M5Stack screen.
	

public:
	static BarrierSimulator& getBarrierSimulator();
	void handleOpCode(const char* opCode);
	void setDisplay(bool p_display);
	BarrierSimulator::BarrierState getState() const;

private:
	BarrierSimulator();
	void open();
	void close();
};

#endif
