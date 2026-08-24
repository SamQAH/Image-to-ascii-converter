#include"magick_wrapper.h"

//constconstexpr float Color::quantumScaleFactor = 256.0f / 65535.0f; // Magick::quantumRange needs Magick namespace

Color::Color(Magick::Color& color): qRed{ (int)color.quantumRed()}, qGreen{ (int)color.quantumGreen() }, qBlue{ (int)color.quantumBlue() }, Red{(int)(qRed * quantumScaleFactor)}, Green{(int)(qGreen * quantumScaleFactor)}, Blue{(int)(qBlue * quantumScaleFactor)}
{
}

Color::Color(const string& str)
{
	Magick::Color tempcol = Magick::Color(str);
	qRed = tempcol.quantumRed();
	qGreen = tempcol.quantumGreen();
	qBlue = tempcol.quantumBlue();
	Red = qRed * quantumScaleFactor;
	Green = qGreen * quantumScaleFactor;
	Blue = qBlue * quantumScaleFactor;
}

Color::Color(int r, int g, int b) :Red{ r > 256 ? 256 : r < 0 ? 0 : r }, Green{ g > 256 ? 256 : g < 0 ? 0 : g }, Blue{ b > 256 ? 256 : b < 0 ? 0 : b }, qRed{ 0 }, qGreen{ 0 }, qBlue{ 0 }
{
	qRed = Red / quantumScaleFactor;
	qGreen = Green / quantumScaleFactor;
	qBlue = Blue / quantumScaleFactor;
}

Magick::Color Color::quantumColor() const
{
	return Magick::Color(qRed, qGreen, qBlue);
}

int Color::quantumRed() const
{
	return qRed;
}

int Color::quantumGreen() const
{
	return qGreen;
}

int Color::quantumBlue() const
{
	return qBlue;
}

int Color::red() const
{
	return Red;
}

int Color::green() const
{
	return Green;
}

int Color::blue() const
{
	return Blue;
}

Image::Image(Magick::Image& img) :image{ img }, pixels{}, columns_{ img.columns() }, rows_{ img.rows() }
{
	pixels.resize(columns_);
	for (size_t i = 0; i < columns_; i++) {
		pixels.at(i).resize(rows_);
		for (size_t j = 0; j < rows_; j++) {
			pixels.at(i).at(j) = Color(image.pixelColor(i, j));
		}
	}
}

Image::Image(const Image& other) :image{ other.image }, pixels{}, columns_{ other.columns() }, rows_{ other.rows() } 
{
	pixels.resize(columns_);
	for (size_t i = 0; i < columns_; i++) {
		pixels.at(i).resize(rows_);
		for (size_t j = 0; j < rows_; j++) {
			pixels.at(i).at(j) = other.pixelColor(i, j);
		}
	}
}

Image& Image::operator=(const Image& other)
{
	image = other.image;
	columns_ = other.columns();
	rows_ = other.rows();
	pixels.resize(columns_);
	for (size_t i = 0; i < columns_; i++) {
		pixels.at(i).resize(rows_);
		for (size_t j = 0; j < rows_; j++) {
			pixels.at(i).at(j) = other.pixelColor(i, j);
		}
	}
	return *this;
}

Color& Image::pixelColor(size_t x, size_t y)
{
	return pixels.at(x).at(y);
}

Color Image::pixelColor(size_t x, size_t y) const
{
	return pixels.at(x).at(y);
}

void Image::pixelColor(size_t x, size_t y, const Color& color)
{
	pixels.at(x).at(y) = color;
}

size_t Image::columns() const
{
	return columns_;
}

size_t Image::rows() const
{
	return rows_;
}

void Image::write(const string& str)
{
	for (size_t i = 0; i < columns_; i++) {
		for (size_t j = 0; j < rows_; j++) {
			image.pixelColor(i, j, pixels.at(i).at(j).quantumColor());
		}
	}
	image.write(str);
}

Magick::Image& Image::operator*()
{
	return image;
}
