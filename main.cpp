#include <cstdio>
#include <format>
#include <iostream>

struct Matrix {
	std::size_t width;
	std::size_t height;
	std::unique_ptr<double[]> data;

	Matrix(std::size_t width, std::size_t height) : width(width), height(height) {
		this->data = std::make_unique<double[]>(width * height);
		for (std::size_t i = 0; i < width * height; i++) {
			this->data[i] = 0.0;
		}
	}

	double get(std::size_t x, std::size_t y) {
		if (x > width || y > height) {
			throw std::format(
				"Matrix::get({}, {}) invalid on matrix with w={}, h={}",
				x, y, this->width, this->height
			);
		}
		return this->data[x + y*this->width];
	}

	void set(std::size_t x, std::size_t y, double val) {
		if (x > width || y > height) {
			throw std::format(
				"Matrix::get({}, {}) invalid on matrix with w={}, h={}",
				x, y, this->width, this->height
			);
		}
		this->data[x + y*this->width] = val;
	}

	void print(std::ostream& out) {
		out << this->width << " x " << this->height << std::endl;
		for (int y = 0; y < this->height; y++) {
			for (int x = 0; x < this->width; x++) {
				out << " " << this->get(x, y);
			}
			out << std::endl;
		}
	}
};

int main(int argv, char** args) {
	Matrix identity(2, 2);
	identity.set(0, 0, 1.0);
	identity.set(1, 1, 1.0);
	identity.print(std::cout);
	return 0;
}
