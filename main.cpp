#include <cstdio>
#include <format>
#include <iostream>
#include <sstream>

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

	Matrix operator*(Matrix& other) {
		if (this->width != other.height) {
			std::ostringstream left;
			this->print(left);
			std::ostringstream right;
			other.print(right);
			throw std::format(
				"Matrix::operator* invoked with mismatching matrices:\n"
				"left: {}right: {}",
				left.str(), right.str()
			);
		}

		Matrix out(other.width, this->height);
		// TODO: Make this smarter.
		for (std::size_t y = 0; y < this->height; y++) {
			for (std::size_t x = 0; x < other.width; x++) {
				double val = 0.0;
				for (std::size_t z = 0; z < this->width; z++) {
					val += this->get(z, y) * other.get(x, z);
				}
				out.set(x, y, val);
			}
		}
		return std::move(out);
	}
};

int main(int argv, char** args) {
	Matrix identity(2, 2);
	identity.set(0, 0, 1.0);
	identity.set(1, 1, 1.0);
	std::cout << "identity" << std::endl;
	identity.print(std::cout);
	std::cout << std::endl; 

	Matrix scale(2, 2);
	scale.set(0, 0, 2.0);
	scale.set(1, 1, 2.0);
	std::cout << "scale" << std::endl;
	scale.print(std::cout);
	std::cout << std::endl; 

	Matrix product1 = identity * scale;
	std::cout << "identity * scale" << std::endl;
	product1.print(std::cout);
	std::cout << std::endl; 
	Matrix product2 = scale * identity;
	std::cout << "scale * identity" << std::endl;
	product2.print(std::cout);
	std::cout << std::endl; 
	return 0;
}
