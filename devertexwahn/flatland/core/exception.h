/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Core_FlatlandException_6da089b7_f446_4ee4_ab38_24473b87d6ab_h
#define Flatland_Core_FlatlandException_6da089b7_f446_4ee4_ab38_24473b87d6ab_h

#include "flatland/core/namespace.h"

#include <string>

FLATLAND_BEGIN_NAMESPACE

class FlatlandException : public std::exception {
public:
    explicit FlatlandException(std::string_view message);

    const char *what() const throw() override;

    virtual ~FlatlandException();

protected:
    std::string error_message_;
};

FLATLAND_END_NAMESPACE

#endif // end define Okapi_LoadSceneException_6da089b7_f446_4ee4_ab38_24473b87d6ab_h
