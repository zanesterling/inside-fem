#include <cstdio>
#include <format>
#include <iostream>
#include <sstream>

template<typename T>
struct Matrix {
	std::size_t width;
	std::size_t height;
	std::unique_ptr<T[]> data;

	Matrix(std::size_t width, std::size_t height) : width(width), height(height) {
		this->data = std::make_unique<T[]>(width * height);
		for (std::size_t i = 0; i < width * height; i++) {
			this->data[i] = 0;
		}
	}

	T get(std::size_t x, std::size_t y) {
		if (x >= this->width || y >= this->height) {
			throw std::format(
				"Matrix::get({}, {}) invalid on matrix with w={}, h={}",
				x, y, this->width, this->height
			);
		}
		return this->data[x + y*this->width];
	}

	void set(std::size_t x, std::size_t y, T val) {
		if (x >= this->width || y >= this->height) {
			throw std::format(
				"Matrix::set({}, {}) invalid on matrix with w={}, h={}",
				x, y, this->width, this->height
			);
		}
		this->data[x + y*this->width] = val;
	}

	void print(std::ostream& out) {
		out << this->width << " x " << this->height << std::endl;
		for (int y = 0; y < this->height; y++) {
			for (int x = 0; x < this->width; x++) {
				out << std::format(" {:3}", this->get(x, y));
			}
			out << std::endl;
		}
	}

	Matrix<T> operator*(Matrix<T>& other) {
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

		Matrix<T> out(other.width, this->height);
		// TODO: Make this smarter.
		for (std::size_t y = 0; y < this->height; y++) {
			for (std::size_t x = 0; x < other.width; x++) {
				T val = 0;
				for (std::size_t z = 0; z < this->width; z++) {
					val += this->get(z, y) * other.get(x, z);
				}
				out.set(x, y, val);
			}
		}
		return std::move(out);
	}
};

struct Grid {
	// A matrix of size num_verts x 2.
	// Each column represents one vertex.
	// Each value in that column is one of the world-coordinates of that vertex.
	Matrix<double> vertices;

	// A matrix of size num_triangles x 3.
	// Each column represents one triangle.
	// Each value in that column represents a vertex of the triangle as an index
	// into the the array of points.
	Matrix<std::size_t> triangles;

	Grid(Matrix<double>&& vertices, Matrix<std::size_t>&& triangles)
		: vertices(std::move(vertices)), triangles(std::move(triangles))
	{
		// Validate.
		for (int tri = 0; tri < this->triangles.width; tri++) {
			for (int vert_index = 0; vert_index < 3; vert_index++) {
				// Make sure that each vert-index in triangles is in vertices.
				this->vertices.get(this->triangles.get(tri, vert_index), 0);
			}
		}
	}

	// Makes a square grid covering [0, 1] x [0, 1],
	// divided into n x n squares,
	// each square covered by two triangles.
	// The divider of each square is parallel to y=x.
	static Grid make_square(std::size_t n) {
		std::size_t points_per_row = n+1;

		Matrix<double> vertices((n+1) * (n+1), 2);
		for (std::size_t j = 0; j <= n; j++) {
			for (std::size_t i = 0; i <= n; i++) {
				double x_i = i * 1.0 / n;
				double y_j = j * 1.0 / n;
				vertices.set(i + j * points_per_row, 0, x_i);
				vertices.set(i + j * points_per_row, 1, y_j);
			}
		}

		Matrix<std::size_t> triangles(2*n*n, 3);
		for (std::size_t j = 0; j < n; j++) {
			for (std::size_t i = 0; i < n; i++) {
				// i is the small horizontal index of the cell.
				// j is the small vertical index of the cell.
				triangles.set(2 * (i + j*n),   0, j * points_per_row + i);
				triangles.set(2 * (i + j*n),   1, (j+1) * points_per_row + i);
				triangles.set(2 * (i + j*n),   2, (j+1) * points_per_row + (i+1));
				triangles.set(2 * (i + j*n)+1, 0, j * points_per_row + i);
				triangles.set(2 * (i + j*n)+1, 1, j * points_per_row + (i+1));
				triangles.set(2 * (i + j*n)+1, 2, (j+1) * points_per_row + (i+1));
			}
		}

		Grid g(std::move(vertices), std::move(triangles));
		return std::move(g);
	}

	void print(std::ostream& out) {
		out << "vertices: ";
		this->vertices.print(out);
		out << "triangles: ";
		this->triangles.print(out);
	}
};

int main(int argv, char** args) {
	Matrix<double> identity(2, 2);
	identity.set(0, 0, 1.0);
	identity.set(1, 1, 1.0);
	std::cout << "identity" << std::endl;
	identity.print(std::cout);
	std::cout << std::endl; 

	Matrix<double> scale(2, 2);
	scale.set(0, 0, 2.0);
	scale.set(1, 1, 2.0);
	std::cout << "scale" << std::endl;
	scale.print(std::cout);
	std::cout << std::endl; 

	Matrix<double> product1 = identity * scale;
	std::cout << "identity * scale" << std::endl;
	product1.print(std::cout);
	std::cout << std::endl; 
	Matrix<double> product2 = scale * identity;
	std::cout << "scale * identity" << std::endl;
	product2.print(std::cout);
	std::cout << std::endl; 

	Grid g = Grid::make_square(5);
	std::cout << "square grid 5x5" << std::endl;
	g.print(std::cout);
	std::cout << std::endl;

	return 0;
}
