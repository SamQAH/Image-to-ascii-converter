#include"magick_wrapper.h"

//constconstexpr float Color::quantumScaleFactor = 256.0f / 65535.0f; // Magick::quantumRange needs Magick namespace

Color::Color(Magick::Color& color): qRed{ (size_t)color.quantumRed()}, qGreen{ (size_t)color.quantumGreen() }, qBlue{ (size_t)color.quantumBlue() }, Red{(size_t)(qRed * quantumScaleFactor)}, Green{(size_t)(qGreen * quantumScaleFactor)}, Blue{(size_t)(qBlue * quantumScaleFactor)}
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

Color::Color(size_t r, size_t g, size_t b) :qRed{ r }, qGreen{ g }, qBlue{ b }, Red{ (size_t)(r * quantumScaleFactor) }, Green{ (size_t)(g * quantumScaleFactor) }, Blue{ (size_t)(b * quantumScaleFactor) }
{
}

Magick::Color Color::quantumColor() const
{
	return Magick::Color(qRed, qGreen, qBlue);
}

size_t Color::quantumRed() const
{
	return qRed;
}

size_t Color::quantumGreen() const
{
	return qGreen;
}

size_t Color::quantumBlue() const
{
	return qBlue;
}

size_t Color::red() const
{
	return Red;
}

size_t Color::green() const
{
	return Green;
}

size_t Color::blue() const
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
