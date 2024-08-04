#include "hex2bin.hpp"
#include <fstream>
#include <locale>
#include <iostream>

int main() {
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::string hexFile;
    std::string binFile;
    Hex2bin converter;

    std::cout << "Âûáåðèòå äåéñòâèå:" << std::endl;
    std::cout << "1) HEX2BIN" << std::endl;
    std::cout << "2) BIN2HEX" << std::endl;
    std::cout << ">>";
    int sw = 0;
    std::cin >> sw;
    switch (sw)
    {
    case(1):{
        std::ifstream inFile("1.hex", std::ios::binary);
        if (!inFile) {
            std::cerr << "Hex ôàéë íå îòðûëñÿ!" << std::endl;
        }
        std::ofstream outFile("2.bin", std::ios::out | std::ios::binary);
        if (!outFile) {
            std::cerr << "Bin ôàéë íå îòðûëñÿ!" << std::endl;
        }
        converter.intelHexToBin(inFile, outFile);
        inFile.close();
        outFile.close();
        std::cout << "Success";
        break;
    }
    case(2): {
        std::ifstream inFile("2.bin", std::ios::binary);
        if (!inFile) {
            std::cerr << "Bin ôàéë íå îòðûëñÿ!" << std::endl;
        }
        std::ofstream outFile("3.hex", std::ios::out | std::ios::binary);
        if (!outFile) {
            std::cerr << "Hex ôàéë íå îòðûëñÿ!" << std::endl;
        }
        converter.binToIntelHex(inFile, outFile);
        inFile.close();
        outFile.close();
        std::cout << "Success";
        break;
    }
    }
    return 0;
}
