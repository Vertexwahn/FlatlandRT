/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_Exception_6da089b7_f446_4ee4_ab38_24473b87d6ab_h
#define De_Vertexwahn_Core_Exception_6da089b7_f446_4ee4_ab38_24473b87d6ab_h

#include "core/namespace.h"

#include "boost/predef.h"

#include <exception>
#include <string>
#include <string_view>

DE_VERTEXWAHN_BEGIN_NAMESPACE

class Exception : public std::exception {
public:
    explicit Exception(std::string_view message);

    #if BOOST_COMP_GNUC < BOOST_VERSION_NUMBER(10,0,0) // GCC9 does not support nodiscard with
    [[nodiscard]]
    const char *what() const noexcept override;
    #else
    [[nodiscard("Returns the reason for the exception")]]
    const char *what() const noexcept override;
    #endif

    ~Exception() override;

protected:
    std::string error_message_;
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_Exception_6da089b7_f446_4ee4_ab38_24473b87d6ab_h
