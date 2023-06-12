# TODO license file, clean up, add argument to take rdf file as input

import xml.etree.ElementTree as ET


def parseXML(xmlfile):
    tree = ET.parse(xmlfile)
    root = tree.getroot()
    # print(root.tag)
    # print(root.attrib['title'])

    newline = '\n'
    tclline = '\\' + newline
    with open(xmlfile+".tcl", "w") as tclfile:
        # Create design
        tclfile.write('create_design' + ' ' + root.attrib['title'] + newline)

        # Target device
        tclfile.write('target_device "1GE100-ES1"' + newline)

        for option in root.iter('Option'):
            # print(option.attrib['name'], "  ", option.attrib['value'])
            if option.attrib['name'] == 'include path':
                # Include path
                tclfile.write('add_include_path' + ' ' + option.attrib['value'] + newline)

        for options in root.iter('Options'):
            if 'top_module' in options.attrib:
                # Top module
                tclfile.write("set_top_module" + " " + options.attrib['top_module'] + newline)

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
                tclfile.write('add_design_file' + ' ')
                once = False
                break
            tclfile.write(design_file + ' ' + tclline)

        for constraint_file in constraint_files:
            tclfile.write('add_constraint_file' + ' ' + constraint_file + newline)

        # Synthesis step
        tclfile.write('synthesize delay' + newline)
        tclfile.write('packing' + newline)
        tclfile.write('place' + newline)
        tclfile.write('route' + newline)
        tclfile.write('sta' + newline)
        tclfile.write('bitstream' + newline)


def convert(name):
    print(f'Hi, {name}')
#    parseXML('63_oc_aquarius_6_pw.rdf')
#    parseXML('3001_oc_or1k_x4.rdf')
#    parseXML('3002_oc_8051_x6.rdf')
#    parseXML('3003_oc_des_x5p.rdf')

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    convert('RDF to TCL for Raptor from RapidSilicon Inc.')


