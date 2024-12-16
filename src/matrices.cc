#include "../include/matrices.h"

namespace Matrices {

    Matrix::Matrix(int rows, int cols) {
        m_rows = rows;
        m_cols = cols;

        m_vector.resize(rows);
        for (int i = 0; i < rows; i++) {
            m_vector.at(i).resize(cols);
        }
    }

    RotationMatrix::RotationMatrix(double theta): Matrix(2, 2) {
        (*this)(0, 0) = std::cos(theta);
        (*this)(0, 1) = -std::sin(theta);
        (*this)(1, 0) = std::sin(theta);
        (*this)(1, 1) = std::cos(theta);
    }

    ScalingMatrix::ScalingMatrix(double scalar): Matrix(2, 2) {
        (*this)(0, 0) = scalar;
        (*this)(0, 1) = 0;
        (*this)(1, 0) = 0;
        (*this)(1, 1) = scalar;
    }

    TranslationMatrix::TranslationMatrix(double x_shift, double y_shift, int cols)
    : Matrix(2, cols) {
        for (int i = 0; i < cols; i++) {
            (*this)(0, i) = x_shift;
            (*this)(1, i) = y_shift;
        }
    }

    Matrix operator+(const Matrix& a, const Matrix& b) {
        if ((a.get_rows() != b.get_rows()) || (a.get_cols() != b.get_cols())) {
            throw std::runtime_error("Error: matrix dimensions must agree");
        }

        Matrix result(a.get_rows(), a.get_cols());

        for (int i = 0; i < a.get_rows(); i++) {
            for (int j = 0; j < a.get_cols(); j++) {
                result(i, j) = a(i, j) + b(i, j);
            }
        }

        return result;
    }

    Matrix operator*(const Matrix& a, const Matrix& b) {
        if (a.get_cols() != b.get_rows()) {
            throw std::runtime_error("Error: dimensions must agree");
        }

        Matrix result(a.get_rows(), b.get_cols());

        for (int k = 0; k < b.get_cols(); k++) {
            for (int i = 0; i < a.get_rows(); i++) {
                for (int j = 0; j < a.get_cols(); j++) {
                    result(i, k) += a(i, j) * b(j, k);
                }
            }
        }

        return result;
    }

    bool operator==(const Matrix&a, const Matrix& b) {
        if ((a.get_cols() != b.get_cols()) || (a.get_rows() != b.get_rows())) {
            return false;
        }

        for (int i = 0; i < a.get_rows(); i++) {
            for (int j = 0; j < a.get_cols(); j++) {
                if ( std::abs( a(i, j) - b(i, j) ) > 0.001) {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(const Matrix&a, const Matrix& b) {
        return !(a == b);
    }

    std::ostream& operator<<(std::ostream& out, const Matrix& a) {
        for (int i = 0; i < a.get_rows(); i++) {
            for (int j = 0; j < a.get_cols(); j++) {
                out << std::setw(11) << a(i, j) << " ";
            }
            out << std::endl;
        }

        return out;
    }
}
