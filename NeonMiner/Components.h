#pragma once

#include "ECS.h"

class PositionComponent : public Component {
public:
	int x() { return xpos; }
	int y() { return ypos; }
	void x(int x) { xpos = x; }
	void y(int y) { ypos = y; }
	
	void init() override {
		xpos = 0;
		ypos = 0;
	}
	void update() override {
		//xpos++,ypos++
	}
	void setPos(int x, int y) {
		xpos = x;
		ypos = y;
	}

private:
	int xpos = 0;
	int ypos = 0;
};
