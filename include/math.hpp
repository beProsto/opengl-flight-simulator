#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <stddef.h>
#include <stdint.h>

// Linear Algebra n stuff for 3D graphicsx
namespace Math {

template <typename T, size_t N, size_t M> class Matrix {
public:
  Matrix(T val = 0) {
    for (uint32_t i = 0; i < N * M; i++) {
      m_data[i] = val;
    }
  }
  Matrix(T m[N * M]) { std::copy(m, (m + (N * M)), m_data); }
  Matrix(T m[N][M]) { std::copy(m, (m + (N * M)), m_data); }
  Matrix(std::initializer_list<T> l) { std::copy(l.begin(), l.end(), m_data); }
  Matrix(const Matrix<T, N, M> &other) { *this = other; }
  ~Matrix() {}

  Matrix<T, N, M> &operator=(const Matrix<T, N, M> &other) {
    std::copy(other.m_data, other.m_data + (N * M), this->m_data);
    return *this;
  }

  T &operator[](size_t i) { return m_data[i]; }
  const T &operator[](size_t i) const { return m_data[i]; }

  template <typename Y, size_t H, size_t W>
  Matrix<T, N, W> operator*(const Matrix<Y, H, W> &other) const {
    static_assert(H == M,
                  "Math::Matrix sizes should be compatible for multiplication "
                  "(assuming A[Y,X] * B[H,W], H should be equal to X)");
    Matrix<T, N, W> result;

    for (uint32_t i = 0; i < N * W; i++) {
      uint32_t y = i / W;
      uint32_t x = i % W;

      for (uint32_t j = 0; j < H; j++) {
        result[i] += m_data[y * M + j] * other[j * W + x];
      }
    }

    return result;
  }
  template <typename Y>
  Matrix<T, N, M> operator+(const Matrix<Y, N, M> &other) const {
    Matrix<T, N, M> result;

    for (uint32_t i = 0; i < N * M; i++) {
      result[i] = m_data[i] + other[i];
    }

    return result;
  }
  template <typename Y>
  Matrix<T, N, M> operator-(const Matrix<Y, N, M> &other) const {
    Matrix<T, N, M> result;

    for (uint32_t i = 0; i < N * M; i++) {
      result[i] = m_data[i] - other[i];
    }

    return result;
  }

  template <typename Y>
  Matrix<T, N, M> &operator*=(const Matrix<Y, N, M> &other) {
    static_assert(N == M, "Math::Matrix's operator*= works only if matrices' "
                          "both dimensions are equal ( A[X,X] )");
    *this = (*this) * other;
    return *this;
  }
  template <typename Y>
  Matrix<T, N, M> &operator+=(const Matrix<Y, N, M> &other) {
    *this = (*this) + other;
    return *this;
  }
  template <typename Y>
  Matrix<T, N, M> &operator-=(const Matrix<Y, N, M> &other) {
    *this = (*this) - other;
    return *this;
  }

  Matrix<T, M, N> Transpose() const {
    Matrix<T, M, N> transposed;
    for (uint32_t i = 0; i < N * M; i++) {
      uint32_t y = i / M;
      uint32_t x = i % M;
      transposed[x * N + y] = m_data[i];
    }
    return transposed;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const Matrix<T, N, M> &mat) {
    for (uint32_t y = 0; y < N; y++) {
      os << "[";
      for (uint32_t x = 0; x < M; x++) {
        os << mat.m_data[y * M + x];
        if (x != (M - 1)) {
          os << ", ";
        }
      }
      os << "]" << std::endl;
    }
    return os;
  }

protected:
  T m_data[N * M];
};

using Mat4f = Matrix<float, 4, 4>;
using Vec4f = Matrix<float, 4, 1>;
using Vec3f = Matrix<float, 3, 1>;
using Vec2f = Matrix<float, 2, 1>;

} // namespace Math