#define _CRT_SECURE_NO_WARNINGS
#include "hex2bin.hpp"
#include <iostream>
#include <bitset>
#include <cstdlib>
#include <vector>
Hex2bin::Hex2bin()
{
	_RECLEN = "10";
	_LOAD_OFFSET_LV = "00";
	_LOAD_OFFSET_RV = "00";
	_counterOffset = 16;
	_DATA = "";
	_RECTYP = "00";
	_CHKSUM = 0;
	_intelHex = { 0 };
	_stream = nullptr;
}
Hex2bin::~Hex2bin()
{

}
void Hex2bin::intelHexToBin(std::istream& intelBuf, std::ostream& binBuf)
{
	_stream = &intelBuf;
	std::string a;
	std::stringstream aF;
	while (!_stream->eof()) {
		getline(*_stream, a);
		aF << a;
		decrCommand(aF);
		aF.str("");
		if (!strcmp(_intelHex.RECTYP_byte, "00")) {
			if (!_intelHex.FIRSTOFFSET_FLAG) {
				_intelHex.FIRSTOFFSET_FLAG = 1;
				if (std::stoi(_intelHex.LOAD_OFFSET_byte, 0, 16) == 0x00){
					_intelHex.LAST_OFFSET = std::stoi(_intelHex.LOAD_OFFSET_byte, 0, 16);
					_intelHex.LAST_RECLEN = std::stoi(_intelHex.RECLEN_byte, 0, 16);
				}
			}
			std::string res = toBin(_intelHex.DATA_byte);
			binBuf << res;
		}
	}
}
void Hex2bin::binToIntelHex(std::istream& binBuf, std::ostream& intelBuf)
{
	_stream = &binBuf;
	std::stringstream ss;
	char hexVal[2];
	int byte_str = 16;							 // кол-во байт в одной строке с данными HEX формата
		
	intelBuf << ":020000040000FA";				 // для Intel 32-bit
	
	while (_stream->get(hexVal, 2)) {
		_CHKSUM += hexVal[0];					 // подсчет кт суммы
		int lVal = (hexVal[0] >> 4) & 0x0f;		 // левая  часть входного байта
		int rVal = hexVal[0] & 0x0f;			 // правая часть входного байта
		ss << std::hex << lVal << rVal;			 // перевод байта в печатные символы

		byte_str--;						 

		if (byte_str == 0) {					// форматировани строки в HEX формат
			intelBuf << "\r" << toHex(ss);
			byte_str = 16;
			_DATA = "";
			ss.str("");
		}
	}

	intelBuf << "\r:00000001FF";				// конец программы
}
void Hex2bin::decrCommand(std::istream& lineHex) 
{
	std::istream* hexStr = &lineHex;
	hexStr->get(_intelHex.RECORD_MASK, 2);
	hexStr->get(_intelHex.RECLEN_byte, 3);
	hexStr->get(_intelHex.LOAD_OFFSET_byte, 5);
	hexStr->get(_intelHex.RECTYP_byte, 3);
	if (strcmp(_intelHex.RECTYP_byte, "01")) {
		int sizeData = strtol(_intelHex.RECLEN_byte, NULL, 16) * 2 + 1;
		_intelHex.DATA_byte = new char[sizeData];
		hexStr->get(_intelHex.DATA_byte, sizeData);
		hexStr->get(_intelHex.CHKSUM_byte, 3);
	}
}
std::string Hex2bin::toBin(char* DATA)
{
	int lineDataSize = 32 - strlen(DATA);
	std::stringstream ss;
	std::string res;									// Финальная строка с данными в 16-ом формате
	unsigned int offset = std::stoi(_intelHex.LOAD_OFFSET_byte, 0, 16);
	int checkAddNullByte = offset - _intelHex.LAST_OFFSET - _intelHex.LAST_RECLEN;
	if (checkAddNullByte > 0x00) {
		for (unsigned char i = 0; i < checkAddNullByte; i += 0x01) {
			res.push_back(std::stoi("FF", 0, 16));
		}
	}

	for (int i = 0; i < strlen(DATA) - 1; i += 2) {
		std::stringstream tmp;							//
		std::string hex;								// Перевод из символьного представления в 16-ый формат
		tmp << DATA[i] << DATA[i + 1];					//
		tmp >> hex;										//
		res.push_back(std::stoi(hex, 0, 16));			//
	}

	_intelHex.LAST_OFFSET = offset;
	_intelHex.LAST_RECLEN = std::stoi(_intelHex.RECLEN_byte, 0, 16);
	return res;
}
std::string Hex2bin::toHex(std::stringstream& ss)
{
	std::string resHexLine;															// готовая строка для записи в файл
	std::string lineDataHexFile;													// неотформатированная строка с данными
	std::stringstream ssLV, ssRV, ssCHKS;
	std::string chksum;																// финальный результат кт суммы

	lineDataHexFile = ss.str();
	for (int i = 0; i < lineDataHexFile.size(); ++i) {								// 
		if (!std::isdigit(lineDataHexFile[i]))										//
			_DATA += std::toupper(lineDataHexFile[i]);								// перевод всех букв в верхний регистр
		else																		//
			_DATA += lineDataHexFile[i];											//
	}

	unsigned char lVal = std::stoi(_LOAD_OFFSET_LV, 0, 16);							//перевод строки в hex
	unsigned char rVal = std::stoi(_LOAD_OFFSET_RV, 0, 16);							//перевод строки в hex

	_CHKSUM += std::stoi(_RECLEN, 0, 16) + lVal + rVal + std::stoi(_RECTYP, 0, 16); // 
	_CHKSUM = 0x100 - _CHKSUM;														//
	ssCHKS << std::hex << (int)_CHKSUM;												//
	ssCHKS >> chksum;																// подсчет кт суммы в 16-ом формате
	for (char& ch : chksum) {														// и перевод в символьный вид
		if (!std::isdigit(ch))														//
			ch = std::toupper(ch);													//
	}																				//
	if (chksum.size() == 1) {														//
		chksum = "0" + chksum;														//
	}																				//
	for (char& ch : _LOAD_OFFSET_LV) {
		if (!std::isdigit(ch))														//
			ch = std::toupper(ch);
	}
	for (char& ch : _LOAD_OFFSET_RV) {
		if (!std::isdigit(ch))														//
			ch = std::toupper(ch);
	}

	resHexLine = ":" + _RECLEN + (_LOAD_OFFSET_LV + _LOAD_OFFSET_RV) + _RECTYP + _DATA + chksum;

	rVal += 0x10;																	//
	_counterOffset--;																//
	if (_counterOffset == 0) {														// сброс счетчика LOAD_OFFSET и инкементация старшей части 0x10
		lVal += 0x01;																//
		_LOAD_OFFSET_RV = "00";
		_counterOffset = 16;														//
		ssLV << std::hex << (int)lVal;												//
		ssLV >> _LOAD_OFFSET_LV;
	}
	else {
		ssLV << std::hex << (int)lVal;												//
		ssLV >> _LOAD_OFFSET_LV; 													// инкрементация младшей части LOAD_OFFSET
		ssRV << std::hex << (int)rVal;												//
		ssRV >> _LOAD_OFFSET_RV;													//
	}
	if (_LOAD_OFFSET_LV.size() == 1)
		_LOAD_OFFSET_LV = "0" + _LOAD_OFFSET_LV;
	_CHKSUM = 0;

	return resHexLine;
}

// привести код в порядок