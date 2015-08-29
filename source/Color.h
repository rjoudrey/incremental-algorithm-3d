#ifndef COLOR_H
#define COLOR_H

class Color { 
public:
	Color() : r(values[0]), g(values[1]), b(values[2]), a(values[3]) {
	}
	float values[4]; float &r, &g, &b, &a;
	void SetColor(float red, float green, float blue, float alpha)  { 
		r = red; g = green; b = blue; a = alpha; 
	}
};

#endif