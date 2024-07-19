#pragma once
#include <sstream>
struct IntelHex {				//�����-�� ������ � ����� 
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
	// ������� �� .hex � .bin
	void intelHexToBin(std::istream& intelBuf, std::ostream& binBuf);
	// ������� �� .bin � .hex
	void binToIntelHex(std::istream& binBuf, std::ostream& intelBuf);
private:
	// ����������� ������� ������ � HEX �����
	void decrCommand(std::istream& lineHex);
	// ������� � .bin
	std::string toBin(char* DATA);
	// ������� � .hex
	std::string toHex(std::stringstream& ss);
private:
	std::string _RECLEN;			// ���-�� ���� � hex ������
	std::string _LOAD_OFFSET_LV;	// �������� ������� ����
	std::string _LOAD_OFFSET_RV;	// �������� ������� ����
	int _counterOffset;				// ������� ��������
	std::string _DATA;				// ������
	std::string _RECTYP;            // ��� ������
	unsigned char _CHKSUM;          // ������������ �����
	IntelHex _intelHex;				// ��������� � �������
	std::istream* _stream;			// ����� ������
};

