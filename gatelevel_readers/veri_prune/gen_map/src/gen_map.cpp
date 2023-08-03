#include "gen_map.h"
#define RYML_SINGLE_HDR_DEFINE_NOW
#include "read_yaml.h"

std::string getDirectionName(int direction) {
    static const std::unordered_map<int, std::string> directionNames = {
        {IN_DIR, "IN_DIR"},
        {OUT_DIR, "OUT_DIR"},
        {INOUT_DIR, "INOUT_DIR"},
        {OUT_CLK, "OUT_CLK"},
        {IN_CLK, "IN_CLK"},
        {IN_RESET, "IN_RESET"}
    };

    auto it = directionNames.find(direction);
    return (it != directionNames.end()) ? it->second : "UNKNOWN";
}

int write_map(gb_map &gb) {
    // ... (unchanged)

    // Create a new file to write the gb_mods data
    std::ofstream outputFile(OUT_PATH);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file for writing." << std::endl;
        return 1;
    }

    outputFile << "#ifndef GB_MODS_DATA_H" << std::endl;
    outputFile << "#define GB_MODS_DATA_H" << std::endl << std::endl;

    outputFile << "struct gb_constructs_data {" << std::endl;
    outputFile << "    std::vector<std::pair<std::string, std::map<std::string, int>>> gb_mods = {" << std::endl;

    for (const auto& module_info : gb.gb_mods) {
        outputFile << "        { \"" << module_info.first << "\", {" << std::endl;
        for (const auto& port_info : module_info.second) {
            outputFile << "            { \"" << port_info.first << "\", " << getDirectionName(port_info.second) << " }," << std::endl;
        }
        outputFile << "        } }," << std::endl;
    }

    outputFile << "    };" << std::endl;
    outputFile << "};" << std::endl << std::endl;

    outputFile << "#endif // GB_MODS_DATA_H" << std::endl;

    outputFile.close();

    // ... (unchanged)

    return 0;
}

std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return std::vector<char>(); // Return an empty vector on error
    }

    // Get the length of the file
    file.seekg(0, std::ios::end);
    unsigned fileSize = static_cast<unsigned>(file.tellg());
    file.seekg(0, std::ios::beg);

    // Handle the case where the file size is 0
    if (fileSize == 0) {
        std::cerr << "Empty file: " << filename << std::endl;
        return std::vector<char>();
    }

    // Read the file content into a vector
    std::vector<char> content(fileSize);
    file.read(content.data(), fileSize);
    file.close();

    return content;
}

int get_gb_data(gb_map &gb) {
    const std::string folderPath = PRIMITIVES_PATH;

    DIR* directory = opendir(folderPath.c_str());
    if (!directory) {
        std::cerr << "Error opening directory: " << folderPath << std::endl;
        return 1;
    }

    // Read files from the directory
    struct dirent* entry;
    while ((entry = readdir(directory)) != nullptr) {
        if (entry->d_type == DT_REG) {
            std::string filename = folderPath + "/" + entry->d_name;
            std::cout << "FILE : " << filename << std::endl;
            std::vector<char> fileContent = readFile(filename);
            if (!fileContent.empty()) {
                gb.contentArray = std::make_unique<char[]>(fileContent.size() + 1);
                std::copy(fileContent.begin(), fileContent.end(), gb.contentArray.get());
                ryml::Tree tree = ryml::parse_in_place(gb.contentArray.get());
                ryml::ConstNodeRef root = tree.rootref();
                if(root.is_map()) {
                    c4::csubstr  category_(tree["category"].val());
                    std::string category(category_.data(), category_.size());
                    std::string periphery = "periphery";
                    if (category == periphery) {
                        std::map<std::string, int> port_info;
                        std::pair<std::string, std::map<std::string, int>> module_info;
                        c4::csubstr  name_(tree["name"].val());
                        std::string name(name_.data(), name_.size());
                        if (tree["ports"].is_map()) {
                            for(const auto &child : tree["ports"].children()) {
                                if(child.is_seq()) {
                                    std::cout << "Child is a sequence " << std::endl;
                                } else if(child.is_map()) {
                                    int direction;
                                    c4::csubstr  port_name_(child.key());
                                    std::string port_name(port_name_.data(), port_name_.size());
                                    c4::csubstr  dir_(child[0].val());
                                    std::string dir(dir_.data(), dir_.size());
                                    c4::csubstr  desc_(child[1].val());
                                    std::string desc(desc_.data(), desc_.size());
                                    if (dir == "input") {
                                        direction = IN_DIR;
                                        if (desc == "Clock input") direction = IN_CLK;
                                        if (desc.find("reset") != std::string::npos) direction = IN_RESET;
                                    } else if (dir == "output") {
                                        direction = OUT_DIR;
                                        if (desc == "Clock output") direction = OUT_CLK;
                                    } else if (dir == "inout") {
                                        direction = INOUT_DIR;
                                    } else {
                                        throw std::runtime_error("Port direction is incorrect.");
                                    }
                                    port_info.insert(std::make_pair(port_name, direction));
                                }
                            }
                        }
                        module_info.first = name;
                        module_info.second = port_info;
                        port_info.clear();
                        gb.gb_mods.push_back(module_info);
                    } else {

                    }
                }
                // Process the parsed data as needed
                //delete[] contentArray; // Remember to deallocate the memory
            }
        }
    }

    closedir(directory);
    write_map(gb);
    return 0;
}

int main(int argc, char **argv)
{
    gb_map gb;
    get_gb_data(gb);
    return 0;
}