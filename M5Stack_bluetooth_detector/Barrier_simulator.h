#ifndef BARRIER_SIMULATOR_H
#define BARRIER_SIMULATOR_H
#include "Barrier_orders.h"
#include <M5Stack.h>

enum BarrierType {IN, OUT};

struct BarrierInfo {
  unsigned long id;
  BarrierType type;

  // returns a string description of the type
  const char* getType() const {
    return type == IN ? "in" : "out";
  };
};

class BarrierSimulator {
public:
	enum BarrierState {
		CLOSED,
		OPEN
	};
private:
	BarrierState m_state;	// state of the barrier
	bool m_display;			// whether display to M5Stack screen.
  BarrierInfo m_barrierInfo; // contains information about this barrier
	

public:
  BarrierSimulator(const BarrierInfo& barrierInfo);
	void handleOpCode(const char* opCode);
	void setDisplay(bool p_display);
	BarrierState getState() const;
  const char* getBarrierType() const;
  // returns true if and only if idToCompare equals the id of this barrier
  bool checkBarrierId(unsigned long idToCompare) const; 
  unsigned long getBarrierId() const;
  void toogleBarrierState(); // toogle state between open/closed
  void showBarrierType(); // display the type of this barrier on the screen of the M5Stack

private:
	void open();
	void close();
};

#endif
