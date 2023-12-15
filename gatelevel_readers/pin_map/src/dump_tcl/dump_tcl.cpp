#include "dump_tcl.h"
#include <regex>
int dump_tcl(std::string& user_sdc, std::string& output_tcl)
{
    // Open the file for reading
    std::ifstream file(user_sdc);

    if (!file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    std::string text;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue; // Skip empty lines and lines starting with '#'
        }
        text += line + "\n";
    }

    // Define a regular expression pattern to parse pin location commands
    std::regex pattern("set_property PIN_LOC (\\w+) \\[get_ports (\\w+)(\\[(\\d+)\\])?\\]");

    // Create a regex iterator
    std::sregex_iterator iter(text.begin(), text.end(), pattern);
    std::sregex_iterator end;

    // Open the output TCL file for appending
    std::ofstream tclFile(output_tcl);

    if (!tclFile.is_open()) {
        std::cerr << "Error opening TCL file." << std::endl;
        return 1;
    }

    tclFile << "set outfile [open \"pin_map.json\" w+] " << std::endl;
    tclFile << "puts $outfile \"{\"\nputs $outfile \"    \\\"locations\\\" : {\" " << std::endl;

    // Iterate through the matches and append the SDC lines to the file
    while (iter != end) {
        std::smatch match = *iter;
        std::cout << "First part: " << match[1] << std::endl;
        std::cout << "Second part: " << match[2] << std::endl;
        tclFile << "puts $outfile \"        \\\"" << match[1] <<"\\\" : {\"" << std::endl;
        tclFile << "puts $outfile \"            \\\"name\\\" : \\\"" << match[2] << "\\\"";
        if (match.size() > 3) {
            if (!match[3].str().empty()) {
                std::string extractedNumber = match[3].str(); // Extract the matched string
                extractedNumber = extractedNumber.substr(1, extractedNumber.length() - 2); // Remove the brackets []
                std::cout << "Extracted number: " << extractedNumber << std::endl;
                tclFile << ",\"" << std::endl;
                tclFile << "puts $outfile \"            \\\"index\\\" : " << extractedNumber;
            }
        }
        tclFile << "\"" << std::endl;
        if (std::next(iter) != end) {
            tclFile << "puts $outfile \"        },\"" << std::endl;
        } else {
            tclFile << "puts $outfile \"        }\"" << std::endl;
        }
        iter++; // Move to the next match
    }
    tclFile << "puts $outfile \"    }\"" << std::endl;
    tclFile << "puts $outfile \"}\"" << std::endl;
    return 0;
}