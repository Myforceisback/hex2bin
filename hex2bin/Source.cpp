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
        std::cout << "Ââåäèòå ïóòü ê *.hex* ôàéëó (path/name.hex):";
        std::cin >> hexFile;
        std::cout << "Ââåäèòå ïóòü äëÿ ñîõðàíåíèÿ *.bin* ôàéëà (path/name.bin):";
        std::cin >> binFile;
        std::ifstream inFile(hexFile, std::ios::binary);
        if (!inFile) {
            std::cerr << "Hex ôàéë íå îòðûëñÿ!" << std::endl;
        }
        std::ofstream outFile(binFile, std::ios::out | std::ios::binary);
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
        std::cout << "Ââåäèòå ïóòü ê *.bin* ôàéëó (path/name.bin):";
        std::cin >> binFile;
        std::cout << "Ââåäèòå ïóòü äëÿ ñîõðàíåíèÿ *.hex* ôàéëà (path/name.hex):";
        std::cin >> hexFile;
        std::ifstream inFile(binFile, std::ios::binary);
        if (!inFile) {
            std::cerr << "Bin ôàéë íå îòðûëñÿ!" << std::endl;
        }
        std::ofstream outFile(hexFile, std::ios::out | std::ios::binary);
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
