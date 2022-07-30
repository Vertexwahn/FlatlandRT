/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Core_Emitter_aa809b1b_b839_4c53_895c_664674b0ab30_h
#define Flatland_Core_Emitter_aa809b1b_b839_4c53_895c_664674b0ab30_h

#include "flatland/core/object.h"

#include <string>

FLATLAND_BEGIN_NAMESPACE

class Emitter : public Object {
public:
    Emitter() {
    }
    virtual ~Emitter() {
    }

    [[nodiscard]]
    virtual std::string to_string() const override {
        return "Emitter";
    }
};

FLATLAND_END_NAMESPACE

#endif // end define Okapi_Emitter_aa809b1b_b839_4c53_895c_664674b0ab30_h
