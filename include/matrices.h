#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using std::vector;

namespace Matrices {

    class Matrix {
        public:
            Matrix(int rows, int cols);

            const double& operator()(int i, int j) const {
                return m_vector.at(i).at(j);
            }

            double& operator()(int i, int j) {
                return m_vector.at(i).at(j);
            }

            int get_rows() const { return m_rows; }
            int get_cols() const { return m_cols; };

        protected:
            vector<vector<double>> m_vector;

        private:
            int m_rows, m_cols;
    };

    class RotationMatrix : public Matrix {
        public:
            RotationMatrix(double theta);
    };

    class ScalingMatrix : public Matrix {
        public:
            ScalingMatrix(double scalar);
    };

    class TranslationMatrix : public Matrix {
        public:
            TranslationMatrix(double x_shift, double y_shift, int cols);
    };

    Matrix operator+(const Matrix& a, const Matrix& b);
    Matrix operator*(const Matrix& a, const Matrix& b);
    bool operator==(const Matrix& a, const Matrix& b);
    bool operator!=(const Matrix& a, const Matrix& b);
    std::ostream& operator<<(std::ostream& out, const Matrix& a);
};
