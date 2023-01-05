/*
 *  SPDX-FileCopyrightText: Copyright 2022-2023 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef De_Vertexwahn_Core_Exception_6da089b7_f446_4ee4_ab38_24473b87d6ab_h
#define De_Vertexwahn_Core_Exception_6da089b7_f446_4ee4_ab38_24473b87d6ab_h

#include "core/namespace.h"

#include <exception>
#include <string>
#include <string_view>

DE_VERTEXWAHN_BEGIN_NAMESPACE

class Exception : public std::exception {
public:
    explicit Exception(std::string_view message);

    const char *what() const throw() override;

    virtual ~Exception();

protected:
    std::string error_message_;
};

DE_VERTEXWAHN_END_NAMESPACE

#endif // end define De_Vertexwahn_Core_Exception_6da089b7_f446_4ee4_ab38_24473b87d6ab_h
