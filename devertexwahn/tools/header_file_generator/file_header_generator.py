"""
    SPDX-FileCopyrightText: Copyright 2022-2024 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

import argparse
import sys
import uuid

TEXT_HEADER = """/*
 *  SPDX-FileCopyrightText: Copyright 2024 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */
"""


def parseArguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("class_name")
    return parser.parse_args()


def main(argv):
    args = parseArguments()

    class_name = args.class_name

    str_uuid = str(uuid.uuid4())
    str_uuid = str_uuid.replace("-", "_")

    print(TEXT_HEADER)

    print("#pragma once")
    print("#ifndef De_Vertexwahn_Core_" + class_name + "_" + str_uuid + "_h")
    print("#define De_Vertexwahn_Core_" + class_name + "_" + str_uuid + "_h")
    print("")
    print("#include \"core/namespace.h\"")
    print("")
    print("DE_VERTEXWAHN_BEGIN_NAMESPACE")
    print("")

    print("class " + class_name + " {");
    print("public:")

    print(class_name + "() {")
    print("}")

    print("virtual ~" + class_name + "() {")
    print("}")

    print("};")
    print("")

    print("DE_VERTEXWAHN_END_NAMESPACE")
    print("")

    print("#endif // end define De_Vertexwahn_Core_" + class_name + "_" + str_uuid + "_h")


if __name__ == "__main__":
    main(sys.argv[1:])
