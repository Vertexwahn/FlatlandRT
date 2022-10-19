/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_FlatlandException_6da089b7_f446_4ee4_ab38_24473b87d6ab_h
#define De_Vertexwahn_Core_FlatlandException_6da089b7_f446_4ee4_ab38_24473b87d6ab_h

#include "core/namespace.h"

#include <exception>
#include <string>
#include <string_view>

DE_VERTEXWAHN_BEGIN_NAMESPACE

class FlatlandException : public std::exception {
public:
    explicit FlatlandException(std::string_view message);

    const char *what() const throw() override;

    virtual ~FlatlandException();

protected:
    std::string error_message_;
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define Okapi_LoadSceneException_6da089b7_f446_4ee4_ab38_24473b87d6ab_h
