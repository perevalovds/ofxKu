#pragma once

//Integer rectangle

class int_rect {
public:
    int x, y;
	int w, h;
	
	int_rect() { x = y = w = h = 0; }
	int_rect(int x0, int y0, int w0, int h0) {
		x = x0;
		y = y0;
		w = w0;
		h = h0;
	}
};


