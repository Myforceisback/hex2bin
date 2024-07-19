#pragma once
#include <sstream>
struct IntelHex {				//зачем-то сделал и забыл 
	char RECORD_MASK[2];
	char RECLEN_byte[3];
	char LOAD_OFFSET_byte[5];
	char RECTYP_byte[3];
	char* DATA_byte;
	char CHKSUM_byte[3];

	~IntelHex() {
		delete[] DATA_byte;
	}
};

class Hex2bin
{
public:
	Hex2bin(); ~Hex2bin();
	// Перевод из .hex в .bin
	void intelHexToBin(std::istream& intelBuf, std::ostream& binBuf);
	// Перевод из .bin в .hex
	void binToIntelHex(std::istream& binBuf, std::ostream& intelBuf);
private:
	// Расшифровка входных данный с HEX файла
	void decrCommand(std::istream& lineHex);
	// Перевод в .bin
	std::string toBin(char* DATA);
	// Перевод в .hex
	std::string toHex(std::stringstream& ss);
private:
	std::string _RECLEN;			// кол-во байт в hex записи
	std::string _LOAD_OFFSET_LV;	// смещение старший байт
	std::string _LOAD_OFFSET_RV;	// смещение младший байт
	int _counterOffset;				// счетчик смещения
	std::string _DATA;				// данные
	std::string _RECTYP;            // код записи
	unsigned char _CHKSUM;          // контрольтная сумма
	IntelHex _intelHex;				// структура с данными
	std::istream* _stream;			// поток данных
};

