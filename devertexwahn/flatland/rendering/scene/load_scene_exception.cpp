/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "flatland/rendering/scene/load_scene_exception.h"

FLATLAND_BEGIN_NAMESPACE

LoadSceneException::LoadSceneException(std::string_view message) : error_message_(message) {
}

const char *LoadSceneException::what() const throw() {
    return error_message_.c_str();
}

LoadSceneException::~LoadSceneException() {}

IntegratorMissing::IntegratorMissing() : LoadSceneException("Integrator missing") {
}

FLATLAND_END_NAMESPACE