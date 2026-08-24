#ifndef MAGICK_WRAPPER_H
#define MAGICK_WRAPPER_H

#include<Magick++.h>
#include<vector>
#include<string>

using namespace std;

constexpr float quantumRange = 65535.0f; // Magick::quantumRange needs Magick namespace

class Color {
public:
	static constexpr float quantumScaleFactor = 256.0f / quantumRange; 
	size_t qRed, qGreen, qBlue;
	size_t Red, Green, Blue;
	Color(Magick::Color& color);
	Color(const string& str);
	Color(size_t r=0, size_t g=0, size_t b=0);
	Magick::Color quantumColor() const;
	size_t quantumRed() const;
	size_t quantumGreen() const;
	size_t quantumBlue() const;
	size_t red() const;
	size_t green() const;
	size_t blue() const;
};

class Image {
private:
	Magick::Image& image;
	vector<vector<Color>> pixels;
	size_t columns_, rows_;
public:
	Image(Magick::Image& img);
	Image(const Image& other);
	Image& operator=(const Image& other);
	Color& pixelColor(size_t x, size_t y);
	Color pixelColor(size_t x, size_t y) const;
	void pixelColor(size_t x, size_t y, const Color& color);
	size_t columns() const;
	size_t rows() const;
	void write(const string& str);
	Magick::Image& operator*();
};

#endif