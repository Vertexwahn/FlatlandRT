#pragma once

// copy https://github.com/mitsuba-renderer/drjit/blob/77d2873a38d50a8d195bdcb3f2292f12987b0040/include/drjit/tensor.h#L187
// https://www.tensorflow.org/guide/tensor
// https://git.aapelivuorinen.com/aapeli/bazel_rules/-/blob/develop/xtensor/example.cc
// https://www.youtube.com/watch?v=TvxmkZmBa-k
// https://xtensor.readthedocs.io/en/latest/numpy.html
// From numpy to xtensor: https://xtensor.readthedocs.io/en/latest/numpy.html
// https://www.math.tugraz.at/~ganster/lv_vektoranalysis_ss_10/17_tensor_definition.pdf

#include "core/namespace.h"

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

#include <iostream>

DE_VERTEXWAHN_BEGIN_NAMESPACE

/*
template <typename ScalarType>
class TensorX {
public:
    using Scalar = ScalarType;

    TensorX() {}

    /// Creates a scalar/rank-0 tensor.
    TensorX(Scalar value) {
        values_ = new Scalar[1];
    }

    virtual ~TensorX() {
        delete[] values_;
    }

    ScalarType* values() {
        return values_;
    }

    int shape(int index) const {
        return 0;
    }

    int ndim() const {
        return 0;
    }

private:
    ScalarType* values_ = nullptr;
};

template <typename  ScalarType>
std::ostream &operator<<(std::ostream &os, const TensorX<ScalarType> &t) {
    os << "TensorX"
       //<< internal::convert_type_to_string<ScalarType>::value
       << "[" << std::endl
       << "  value = " << t.values()[0] << std::endl
       << "]" << std::endl;

    return os;
}

typedef TensorX<float> TensorXf;
typedef TensorX<double> TensorXd;
*/

typedef xt::xarray<float> TensorXf;
typedef xt::xarray<double> TensorXd;

DE_VERTEXWAHN_END_NAMESPACE
