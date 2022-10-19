/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_Emitter_aa809b1b_b839_4c53_895c_664674b0ab30_h
#define De_Vertexwahn_Core_Emitter_aa809b1b_b839_4c53_895c_664674b0ab30_h

#include "core/object.h"
#include "imaging/color.h"

#include <string>

DE_VERTEXWAHN_BEGIN_NAMESPACE

template <unsigned int Dimension, typename ScalarType>
class Emitter : public Object {
public:
    Emitter() {

    }
    virtual ~Emitter() {
    }

    virtual ColorType<3, ScalarType> evaluate() const = 0;
    
    [[nodiscard]]
    virtual std::string to_string() const override {
        return "Emitter";
    }
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define Okapi_Emitter_aa809b1b_b839_4c53_895c_664674b0ab30_h
