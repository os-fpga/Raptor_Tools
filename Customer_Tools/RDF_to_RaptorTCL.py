"""
  Copyright (C) 2022-2023 Rapid Silicon Inc.
  Authorized use only
"""

import xml.etree.ElementTree as ET
import argparse
import os


def parse_xml(xml_file):
    tree = ET.parse(xml_file)
    root = tree.getroot()

    newline = '\n'
    tcl_line = '\\' + newline
    with open(xml_file + ".tcl", "w") as tcl_file:
        # Create design
        tcl_file.write('create_design' + ' ' + root.attrib['title'] + newline)

        # Target device
        tcl_file.write('target_device "1GE100-ES1"' + newline)

        for option in root.iter('Option'):
            # print(option.attrib['name'], "  ", option.attrib['value'])
            if option.attrib['name'] == 'include path':
                # Include path
                tcl_file.write('add_include_path' + ' ' + option.attrib['value'] + newline)

        for options in root.iter('Options'):
            if 'top_module' in options.attrib:
                # Top module
                tcl_file.write("set_top_module" + " " + options.attrib['top_module'] + newline)

        constraint_files = []
        design_files = []
        for sources in root.iter('Source'):
            # Design and constraint files
            if sources.attrib['name'].endswith('.ldc'):
                print('Skipping *.ldc files')
            elif sources.attrib['name'].endswith('.fdc'):
                constraint_files.append(sources.attrib['name'])
            else:
                design_files.append(sources.attrib['name'])

        once = True
        for design_file in design_files:
            while once:
                tcl_file.write('add_design_file' + ' ')
                once = False
                break
            tcl_file.write(design_file + ' ' + tcl_line)

        for constraint_file in constraint_files:
            tcl_file.write('add_constraint_file' + ' ' + constraint_file + newline)

        # Synthesis step
        tcl_file.write('synthesize delay' + newline)
        tcl_file.write('packing' + newline)
        tcl_file.write('place' + newline)
        tcl_file.write('route' + newline)
        tcl_file.write('sta' + newline)
        tcl_file.write('bitstream' + newline)

        print('Finished conversion to TCL: ', tcl_file.name)


def convert(file_path):
    parse_xml(file_path)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--path', '-p', help="Path to single RDF project file. Default is current directory.")
    args = parser.parse_args()
    target_loc = args.path

    if not os.path.exists(target_loc):
        print('File not found')
        raise SystemExit(1)
    print('Found file: ', os.path.abspath(target_loc))
    convert(os.path.abspath(target_loc))


if __name__ == '__main__':
    print("""
**********************************************************************
RDF to TCL for Raptor from RapidSilicon Inc., Copyright (C) 2022-2023
Authorized use only
**********************************************************************
    """)
    main()


