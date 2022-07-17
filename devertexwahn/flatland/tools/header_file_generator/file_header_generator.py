"""
    SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

import uuid
import sys
import argparse

TEXT_HEADER = """/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
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
	str_uuid = str_uuid.replace("-","_")
	
	print(TEXT_HEADER)
	
	print("#pragma once")
	print("#ifndef Flatland_Core_" + class_name + "_" + str_uuid + "_h")
	print("#define Flatland_Core_" + class_name + "_" + str_uuid + "_h")
	print("")
	print("#include \"flatland/core/namespace.h\"")
	print("")
	print("FLATLAND_BEGIN_NAMESPACE")
	print("")
	
	print("class " + class_name + " {");
	print("public:")
	
	print(class_name + "() {" )
	print("}")
	
	print("virtual ~" + class_name + "() {" )
	print("}")	
	
	print("};")
	print("")
	
	print("FLATLAND_END_NAMESPACE")
	print("")
	
	print("#endif // end define Okapi_" + class_name + "_" + str_uuid + "_h")

if __name__ == "__main__":
   main(sys.argv[1:])