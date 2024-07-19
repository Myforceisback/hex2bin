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
	_counterOffset = 15;
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
	int byte_str = 16;					 // ���-�� ���� � ����� ������ � ������� HEX �������

	intelBuf << ":020000040000FA";     // ��� Intel 32-bit
	
	while (_stream->get(hexVal, 2)) {
		_CHKSUM += hexVal[0];			 // ������� �� �����
		int lVal = hexVal[0] >> 4;		 // �����  ����� �������� �����
		int rVal = hexVal[0] & 0xf;		 // ������ ����� �������� �����
		ss << std::hex << lVal << rVal;  // ������� ����� � �������� �������

		byte_str--;						 

		if (byte_str == 0) {			 // ������������� ������ � HEX ������
			intelBuf << "\r" << toHex(ss);
			byte_str = 16;
			_DATA = "";
			ss.str("");
		}
	}

	intelBuf << "\r:00000001FF";         // ����� ���������
}
void Hex2bin::decrCommand(std::istream& lineHex) // �� ��������
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
	std::string res;									// ��������� ������ � ������� � 16-�� �������
	for (int i = 0; i < strlen(DATA) - 1; i += 2) {		
		std::stringstream tmp;							//
		std::string hex;								// ������� �� ����������� ������������� � 16-�� ������
		tmp << DATA[i] << DATA[i + 1];					//
		tmp >> hex;										//
		res.push_back(std::stoi(hex, 0, 16));			//
	}
	return res;
}
std::string Hex2bin::toHex(std::stringstream& ss)
{
	std::string resHexLine;															// ������� ������ ��� ������ � ����
	std::string lineDataHexFile;													// ������������������� ������ � �������
	std::stringstream ssLV, ssRV, ssCHKS;
	std::string chksum;																// ��������� ��������� �� �����

	lineDataHexFile = ss.str();
	for (int i = 0; i < lineDataHexFile.size(); ++i) {								// 
		if (!std::isdigit(lineDataHexFile[i]))										//
			_DATA += std::toupper(lineDataHexFile[i]);								// ������� ���� ���� � ������� �������
		else																		//
			_DATA += lineDataHexFile[i];											//
	}

	unsigned char lVal = std::stoi(_LOAD_OFFSET_LV, 0, 16);							//������� ������ � hex
	unsigned char rVal = std::stoi(_LOAD_OFFSET_RV, 0, 16);							//������� ������ � hex

	_CHKSUM += std::stoi(_RECLEN, 0, 16) + lVal + rVal + std::stoi(_RECTYP, 0, 16); // 
	_CHKSUM = 0x100 - _CHKSUM;														//
	ssCHKS << std::hex << (int)_CHKSUM;												//
	ssCHKS >> chksum;																// ������� �� ����� � 16-�� �������
	for (char& ch : chksum) {														// � ������� � ���������� ���
		if (!std::isdigit(ch))														//
			ch = std::toupper(ch);													//
	}																				//
	if (chksum.size() == 1) {														//
		chksum = "0" + chksum;														//
	}																				//

	resHexLine = ":" + _RECLEN + (_LOAD_OFFSET_LV + _LOAD_OFFSET_RV) + _RECTYP + _DATA + chksum;

	rVal += 0x10;																	//
	_counterOffset--;																//
	if (_counterOffset == 0) {														// ����� �������� LOAD_OFFSET � ������������ ������� ����� 0x10
		_LOAD_OFFSET_LV[0] += 0x10;													//
		_LOAD_OFFSET_RV = "00";														//
		_counterOffset = 15;														//
	}
	ssLV << std::hex << (int)lVal;													//
	ssLV >> _LOAD_OFFSET_LV; 														// ������������� ������� ����� LOAD_OFFSET
	ssRV << std::hex << (int)rVal;													//
	ssRV >> _LOAD_OFFSET_RV;														//
	
	if (_LOAD_OFFSET_LV.size() == 1)
		_LOAD_OFFSET_LV += "0";
	_CHKSUM = 0;

	return resHexLine;
}