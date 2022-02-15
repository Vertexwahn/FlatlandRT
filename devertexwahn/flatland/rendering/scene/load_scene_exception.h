/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#ifndef Flatland_Core_LoadSceneException_6da089b7_f446_4ee4_ab38_24473b87d6ab_h
#define Flatland_Core_LoadSceneException_6da089b7_f446_4ee4_ab38_24473b87d6ab_h

#include "flatland/core/namespace.h"

#include <string>

FLATLAND_BEGIN_NAMESPACE

class LoadSceneException : public std::exception {
public:
    explicit LoadSceneException(std::string_view message);

    const char *what() const throw() override;

    virtual ~LoadSceneException();

protected:
    std::string error_message_;
};

class IntegratorMissing : public LoadSceneException {
public:
    explicit IntegratorMissing();
};

FLATLAND_END_NAMESPACE

#endif // end define Okapi_LoadSceneException_6da089b7_f446_4ee4_ab38_24473b87d6ab_h
