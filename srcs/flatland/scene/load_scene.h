/*
    This file is part of Flatland, a simple 2D ray tracer

    Copyright (c) 2020, Julian Amann (dev@vertexwahn.de), All rights reserved.
    Distributed under the 3-Clause BSD License software license, see the accompanying
    file LICENSE.md or https://opensource.org/licenses/BSD-3-Clause.
*/

#pragma once
#ifndef Flatland_loadScene_48a28c98_4cfb_4ceb_85e3_118d91191028_h
#define Flatland_loadScene_48a28c98_4cfb_4ceb_85e3_118d91191028_h

#include "flatland/core/namespace.h"
#include "flatland/core/reference_counted.h"
#include "flatland/scene/scene.h"

#include <string_view>

FLATLAND_BEGIN_NAMESPACE

class LoadSceneException : public std::exception {
public:
    explicit LoadSceneException(std::string_view message);

    const char *what() const throw() override;

    virtual ~LoadSceneException();

protected:
    std::string errorMessage_;
};

class IntegratorMissing : public LoadSceneException {
public:
    explicit IntegratorMissing();
};

ReferenceCounted<Scene2f> loadScene(std::string_view filename);

FLATLAND_END_NAMESPACE

#endif // end define Flatland_loadScene_48a28c98_4cfb_4ceb_85e3_118d91191028_h
