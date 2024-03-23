#ifndef CSVREADER_HPP
#define CSVREADER_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class CSVReader {
private:
    std::string fileName;
    char delimiter;

public:
    // Constructor with filename and delimiter (comma is the default delimiter)
    CSVReader(std::string fName, char delim = ',') : fileName(fName), delimiter(delim) {}

    // Function to fetch data from a CSV File
    std::vector<std::vector<double>> getData() {
        std::ifstream file(fileName);
        std::vector<std::vector<double>> dataList;
        std::string line = "";

        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << fileName << std::endl;
            return dataList; // Return empty list if file cannot be opened
        }

        // Skip the first line assuming it contains headers
        std::getline(file, line);

        // Iterate through each subsequent line and split the content using delimiter
        while (getline(file, line)) {
            if (line.empty()) continue; // Skip empty lines

            std::vector<double> vec;
            std::stringstream lineStream(line);
            std::string cell;

            while (getline(lineStream, cell, delimiter)) {
                try {
                    double value = stod(cell); // Convert string to double
                    vec.push_back(value);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: " << e.what() << " for cell: " << cell << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Out of range: " << e.what() << " for cell: " << cell << std::endl;
                }
            }

            dataList.push_back(vec);
        }

        file.close();
        return dataList;
    }

};

#endif
