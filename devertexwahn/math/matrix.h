/*
 *  SPDX-FileCopyrightText: Copyright 2023 Technical University of Munich, Chair of Computational Modeling and Simulation
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Math_Matrix_3b0aa566_7e67_433d_8fb6_075c84a4a70e_h
#define De_Vertexwahn_Math_Matrix_3b0aa566_7e67_433d_8fb6_075c84a4a70e_h

#include "core/namespace.h"

#include "Eigen/Core"
#include "Eigen/Dense"
#include "Eigen/Geometry"

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <typename ScalarType, int RowCount, int ColumnCount>
using Matrix = Eigen::Matrix<ScalarType, RowCount, ColumnCount>;
template <typename ScalarType>
using Matrix22 = Eigen::Matrix<ScalarType, 2, 2>;
template <typename ScalarType>
using Matrix33 = Eigen::Matrix<ScalarType, 3, 3>;
template <typename ScalarType>
using Matrix44 = Eigen::Matrix<ScalarType, 4, 4>;

using Matrix22i = Matrix22<int>;
using Matrix22f = Matrix22<float>;
using Matrix22d = Matrix22<double>;
using Matrix33i = Matrix33<int>;
using Matrix33f = Matrix33<float>;
using Matrix33d = Matrix33<double>;
using Matrix44i = Matrix44<int>;
using Matrix44f = Matrix44<float>;
using Matrix44d = Matrix44<double>;

template <typename ScalarType>
ScalarType det(Matrix<ScalarType, 2, 2> const& m) {
    return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
}

template <typename ScalarType>
Matrix<ScalarType, 2, 2> rotation_matrix22(const ScalarType &angle) {
    ScalarType s = sin(angle);
    ScalarType c = cos(angle);

    Matrix<ScalarType, 2, 2> m;
    m.row(0) = Eigen::Matrix<ScalarType, 2, 1>(c, -s);
    m.row(1) = Eigen::Matrix<ScalarType, 2, 1>(s, c);

    return m;
}

// Creates efficiently a 2D rotation matrix, that is defined by rotating by the angle enclosed
// between (1,0) and targetDirection or in other words by rotating (1,0) until targetDirection.
template <typename ScalarType>
Matrix<ScalarType, 2, 2> rotation_matrix22(Matrix<ScalarType, 2, 1> const& targetDirection) {
    ScalarType const cosAlpha = Matrix<ScalarType, 2, 1>(1, 0).dot(targetDirection.normalized());
    ScalarType const sinAlpha = sqrt(1 - cosAlpha*cosAlpha);
    Matrix<ScalarType, 2, 2> rotation;
    auto& row0 = rotation.row(0), row1 = rotation.row(1);
    row0[0] = row1[1] = cosAlpha;
    row0[1] = -sinAlpha;
    row1[0] = sinAlpha;
    return rotation;
}

template <typename ScalarType>
Matrix<ScalarType, 3, 3> rotation_matrix3_x(const ScalarType &angle) {
    ScalarType s = sin(angle);
    ScalarType c = cos(angle);

    Matrix<ScalarType, 3, 3> m;
    m.row(0) = Eigen::Matrix<ScalarType, 3, 1>(1, 0, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 3, 1>(0, c, -s);
    m.row(2) = Eigen::Matrix<ScalarType, 3, 1>(0, s, c);

    return m;
}

template <typename ScalarType>
Matrix<ScalarType, 3, 3> rotation_matrix3_y(const ScalarType &angle) {
    ScalarType s = sin(angle);
    ScalarType c = cos(angle);

    Matrix<ScalarType, 3, 3> m;
    m.row(0) = Eigen::Matrix<ScalarType, 3, 1>(c, 0, s);
    m.row(1) = Eigen::Matrix<ScalarType, 3, 1>(0, 1, -0);
    m.row(2) = Eigen::Matrix<ScalarType, 3, 1>(-s, 0, c);

    return m;
}

template <typename ScalarType>
Matrix<ScalarType, 3, 3> rotation_matrix_3_z(const ScalarType &angle) {
    ScalarType s = sin(angle);
    ScalarType c = cos(angle);

    Matrix<ScalarType, 3, 3> m;
    m.row(0) = Eigen::Matrix<ScalarType, 3, 1>(c, -s, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 3, 1>(s, c, 0);
    m.row(2) = Eigen::Matrix<ScalarType, 3, 1>(0, 0, 1);

    return m;
}

template <typename ScalarType>
Matrix<ScalarType, 4, 4> translation_matrix(const ScalarType x, const ScalarType y, const ScalarType z) {
    Eigen::Transform<ScalarType,3,Eigen::Affine> transform{Eigen::Translation<ScalarType, 3>(x ,y ,z)};
    Eigen::Matrix4f matrix{transform.matrix()};
    return matrix;
}

template <typename ScalarType>
Matrix<ScalarType, 4, 4> translation_matrix(const Matrix<ScalarType, 3, 1> &translation) {
    Eigen::Transform<ScalarType,3,Eigen::Affine> transform{Eigen::Translation<ScalarType, 3>(translation)};
    Eigen::Matrix4f matrix{transform.matrix()};
    return matrix;
}

template <typename T>
Matrix<T, 4, 4> scale_matrix(const T x, const T y, const T z) {
    Matrix<T, 4, 4> m = Matrix<T, 4, 4>::Identity();
    m(0, 0) = x;
    m(1, 1) = y;
    m(2, 2) = z;
    m(3, 3) = 1.0;

    return m;
}

template <typename ScalarType>
Matrix<ScalarType, 4, 4> scale_matrix(const Matrix<ScalarType, 3, 1> &scale) {
    Matrix<ScalarType, 4, 4> m = Matrix<ScalarType, 4, 4>::Identity();
    m(0, 0) = scale[0];
    m(1, 1) = scale[1];
    m(2, 2) = scale[2];
    m(3, 3) = 1.0;

    return m;
}

template <typename ScalarType>
Matrix<ScalarType, 4, 4> inverse_matrix(const Matrix<ScalarType, 4, 4> &m) {
    return m.inverse();
}

template <typename ScalarType>
Matrix<ScalarType, 4, 4> rotation_matrix_x(const ScalarType &angle) {
    ScalarType s = sin(angle);
    ScalarType c = cos(angle);

    Matrix<ScalarType, 4, 4> m;
    m.row(0) = Eigen::Matrix<ScalarType, 4, 1>(1, 0, 0, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 4, 1>(0, c, -s, 0);
    m.row(2) = Eigen::Matrix<ScalarType, 4, 1>(0, s, c, 0);
    m.row(3) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 0, 1);

    return m;
}

template <typename ScalarType>
Matrix<ScalarType, 4, 4> rotation_matrix_y(const ScalarType &angle) {
    ScalarType s = sin(angle);
    ScalarType c = cos(angle);

    Matrix<ScalarType, 4, 4> m;
    m.row(0) = Eigen::Matrix<ScalarType, 4, 1>(c, 0, s, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 4, 1>(0, 1, -0, 0);
    m.row(2) = Eigen::Matrix<ScalarType, 4, 1>(-s, 0, c, 0);
    m.row(3) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 0, 1);

    return m;
}

template <typename ScalarType>
Matrix<ScalarType, 4, 4> rotation_matrix_z(const ScalarType &angle) {
    ScalarType s = sin(angle);
    ScalarType c = cos(angle);

    Matrix<ScalarType, 4, 4> m;
    m.row(0) = Eigen::Matrix<ScalarType, 4, 1>(c, -s, 0, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 4, 1>(s, c, 0, 0);
    m.row(2) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 1, 0);
    m.row(3) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 0, 1);

    return m;
}

template <typename ScalarType>
Matrix<ScalarType, 4, 4> perspective_projection_matrix_RH(
        ScalarType field_of_viewY,
        ScalarType aspect_ratio,
        ScalarType near_plane,
        ScalarType far_plane) {
    ScalarType y = 1 / tan(field_of_viewY / 2);
    ScalarType x = y / aspect_ratio;

    ScalarType d1 = far_plane / (far_plane - near_plane);
    ScalarType d2 = -near_plane * far_plane / (far_plane - near_plane);

    Matrix<ScalarType, 4, 4> m;
    m.row(0) = Eigen::Matrix<ScalarType, 4, 1>(x, 0, 0, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 4, 1>(0, y, 0, 0);
    m.row(2) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, d1, d2);
    m.row(3) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 1, 0);

    return m;
}

template<typename ScalarType>
Matrix<ScalarType, 4, 4> orthographic_projection_matrix_RH(
        ScalarType width,
        ScalarType height,
        ScalarType near_plane,
        ScalarType far_plane) {
    ScalarType x = 2 / width;
    ScalarType y = 2 / height;

    ScalarType d1 = 1 / (far_plane - near_plane);
    ScalarType d2 = -near_plane / (far_plane - near_plane);

    Matrix<ScalarType, 4, 4> m;
    m.row(0) = Eigen::Matrix<ScalarType, 4, 1>(x, 0, 0, 0);
    m.row(1) = Eigen::Matrix<ScalarType, 4, 1>(0, y, 0, 0);
    m.row(2) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, d1, d2);
    m.row(3) = Eigen::Matrix<ScalarType, 4, 1>(0, 0, 0, 1);

    return m;
}

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Math_Matrix_3b0aa566_7e67_433d_8fb6_075c84a4a70e_h
