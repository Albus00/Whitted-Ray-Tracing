#pragma once
class colorRGB
{
public:
	colorRGB() {
		r = g = b = 0;
	}

	colorRGB(double c1, double c2, double c3) : r(c1), g(c2), b(c3) {}

	double r;
	double g;
	double b;

	// Overloaded * operator
	colorRGB operator*(double scalar) const {
		return colorRGB(r * scalar, g * scalar, b * scalar);
	}

	// Overloaded + operator
	colorRGB operator+(colorRGB c) const {
		return colorRGB(r + c.r, g + c.g, b + c.b);
	}

};
	// Global function to handle double * colorRGB
inline colorRGB operator*(double scalar, const colorRGB& color) {
		return color * scalar;
	}