import xml.etree.ElementTree as ET
import os
import argparse

def extract_device_names(file_paths):
    device_names = []

    for file_path in file_paths:
        if not os.path.exists(file_path):
            print(f"Warning: File not found - {file_path}")
            continue

        if file_path.endswith('.xml'):
            tree = ET.parse(file_path)
            root = tree.getroot()
            for device in root.findall('device'):
                name = device.get('name')
                # Prepend 'RS' if name starts with an integer
                #if name[0].isdigit():
                #    name = 'RS' + name
                name = name.replace("-", "_")
                if name not in device_names:
                    device_names.append(name)
        elif file_path.endswith('.txt'):
            with open(file_path, 'r') as file:
                for line in file:
                    name = line.strip()
                    name = name.replace("-", "_")
                    if name and name not in device_names:
                        device_names.append(name)
    
    return device_names

def create_cpp_header(device_names, header_file_path):
    if os.path.exists(header_file_path):
        os.remove(header_file_path)
    with open(header_file_path, 'w') as file:
        file.write("#pragma once\n\n")
        file.write("#include <iostream>\n")
        file.write("#include \"License_manager.hpp\"\n")
        file.write("using namespace std;\n\n")
        file.write("\n\n")
        file.write("const unordered_map<int, string> stringByEventType = {\n")
        file.write("\t{LICENSE_OK, \"OK \"},\n")
        file.write("\t{LICENSE_FILE_NOT_FOUND, \"license file not found \"},\n")
        file.write("\t{LICENSE_SERVER_NOT_FOUND, \"license server can't be contacted \"},\n")
        file.write("\t{ENVIRONMENT_VARIABLE_NOT_DEFINED, \"environment variable not defined \"},\n")
        file.write("\t{FILE_FORMAT_NOT_RECOGNIZED, \"license file has invalid format (not .ini file) \"},\n")
        file.write("\t{LICENSE_MALFORMED, \"some mandatory field are missing, or data can't be fully read. \"},\n")
        file.write("\t{PRODUCT_NOT_LICENSED, \"this product is not licensed \"},\n")
        file.write("\t{PRODUCT_EXPIRED, \"license expired \"},\n")
        file.write("\t{LICENSE_CORRUPTED, \"license signature didn't match with current license \"},\n")
        file.write("\t{IDENTIFIERS_MISMATCH, \"Calculated identifier and the one provided in license didn't match\"}\n")
        file.write("};\n")
        file.write("\n\n")
        file.write("    bool License_Manager::licenseCheckout(const string &productName) {\n")
        file.write("        LicenseLocation licLocation = {" + 'LICENSE_PATH' + "};\n")
        file.write("        LicenseInfo licenseInfo;\n")
        file.write("        size_t pc_id_sz = LCC_API_PC_IDENTIFIER_SIZE + 1;\n")
        file.write("        char pc_identifier[LCC_API_PC_IDENTIFIER_SIZE + 1];\n")
        file.write("\n")
        file.write(f"       LCC_EVENT_TYPE result = acquire_license(nullptr, &licLocation, &licenseInfo);\n")
        file.write("        if (result == LICENSE_OK) {\n")
        file.write("            cout << \"License for Raptor software OK\" << endl;\n")
        file.write("        \n")
        file.write("        CallerInformations callerInfo;\n")
        file.write("        if (productName == \"MPW1\") { \n")
        file.write("            callerInfo = {\"\\0\", \"MPW1\"};\n")
        for i, name in enumerate(device_names):
            if name != "MPW1":
                file.write(f"           }} else if (productName == \"{name}\") {{\n")
                file.write(f"                            callerInfo = {{\"\\0\", \"{name}\"}};\n")
        file.write("            } else { \n")
        file.write("                  cout << \" No matching device\" << endl; \n")        
        file.write("        }\n")
        file.write("        result = acquire_license(&callerInfo, &licLocation, &licenseInfo);\n")
        file.write("        if (result == LICENSE_OK) {\n")
        file.write("            cout << productName.c_str() << \"  is licensed\" << endl;\n")
        file.write("            if (!licenseInfo.linked_to_pc) {\n")
        file.write("                cout << \"No hardware signature in license file. This is a 'demo' license that works on every PC.\" << endl;\n")
        file.write("            }\n")
        file.write("            return true;\n")
        file.write("        } else {\n")
        file.write("            cout << productName.c_str() << \"  is NOT licensed because \" << stringByEventType.at(result) << endl;\n")
        file.write("            return false;\n")
        file.write("        }\n")
        file.write("        } else {\n")
        file.write("            cout << \"License ERROR:\" << endl;\n")
        file.write("            cout << \"    \" << stringByEventType.at(result) << endl;\n")
        file.write("            if (identify_pc(STRATEGY_DEFAULT, pc_identifier, &pc_id_sz, nullptr)) {\n")
        file.write("                cout << \"PC signature is:\" << endl;\n")
        file.write("                cout << \"    \" << pc_identifier << endl;\n")
        file.write("            } else {\n")
        file.write("                cout << \"Errors in identify_pc\" << endl;\n")
        file.write("            }\n")
        file.write("            return false;\n")
        file.write("        }\n")
        file.write("        }\n")

def main ():
    parser = argparse.ArgumentParser()
    parser.add_argument('--xml_filepath',
                        type=str,default='',metavar='',required=True,help='Path for the device.xml which will be used', dest="XML_filepath")
    parser.add_argument('--feature_txt_path',
                        type=str,default='1',metavar='',help='Path of txt file having other feature list', dest="features_txt")
    args = parser.parse_args()
    print("XML file that will be update:\n\t\t",args.XML_filepath)
    print("features.txt file path:\n\t\t",args.features_txt)
    file_paths = [args.XML_filepath, args.features_txt]  # Replace with actual paths
    header_file_path = 'License_devices.cpp'  # Path to the header file
    device_names = extract_device_names(file_paths)
    create_cpp_header(device_names, header_file_path)
    print(f"Header file created at {header_file_path}")

if __name__ == "__main__":
    main()




