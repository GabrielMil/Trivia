#include "Helper.h"


void Helper::createBuffer(Buffer& buffer, json& jsonObj, BYTE code)
{
	std::string jsonStr = "";
	std::string binaryLen = "";
	jsonStr = jsonObj.dump();  // create the json as string
	buffer.push_back(code);  // Fill the code
	binaryLen = makeBinary32(jsonStr.length());
	//Start filling the length
	buffer.push_back(binToDec8(binaryLen.substr(0, 8)));  // first byte
	buffer.push_back(binToDec8(binaryLen.substr(8, 8)));  // second byte
	buffer.push_back(binToDec8(binaryLen.substr(16, 8)));  // third byte
	buffer.push_back(binToDec8(binaryLen.substr(24, 8)));  // fourth byte
	fillBufferField(buffer, jsonStr);  // Fill the data
}

void Helper::fillBufferField(Buffer& bf, std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		bf.push_back(str[i]);
	}
}

std::string Helper::lenPaddZero(int len)
{
	std::string strLen = std::to_string(len);  // convert to str
	strLen.insert(strLen.begin(), 4 - strLen.length(), '0');  // Padd with zeros (it takes 4 bytes so 4 zeros)
	return strLen;
}

void Helper::arrToVector(Buffer& bf, char* toCopy)
{
	std::string str(toCopy);
	int length = atoi(str.substr(1, 5).c_str());
	int i = 0;
	for (i = 0; i < length; i++)
	{
		bf.push_back(str[i + 1]);
	}
}

std::string Helper::bufferToJson(Buffer& buffer)
{
	std::string jsonStr = "";
	size_t len = lenFromBuffer(buffer);
	for (int i = 0; i < len; i++)
	{
		jsonStr += buffer[DATA_START + i];
	}
	return jsonStr;
}

size_t Helper::lenFromBuffer(Buffer& buffer)
{
	//Get the bytes
	std::string first = makeBinary8(buffer[1]);
	std::string second = makeBinary8(buffer[2]);
	std::string third = makeBinary8(buffer[3]);
	std::string fourth = makeBinary8(buffer[4]);
	return binToDec32(first + second + third + fourth);
}

std::string Helper::makeBinary32(size_t num)
{
	BYTE arr[BITS_ARRAY] = { 0 };
	std::string binary = "";
	for (int i = 0; num > 0; i++)
	{
		arr[i] = num % 2;
		num /= 2;
	}
	for (int i = 0; i < BITS_ARRAY; i++)
	{
		binary += std::to_string(arr[i]);
	}
	std::reverse(binary.begin(), binary.end());
	return binary;
}

//This function converts the number from BYTE to bytearray(8 bits)
std::string Helper::makeBinary8(BYTE num)
{
	BYTE arr[BYTE_LEN] = { 0 };
	std::string binary = "";
	for (int i = 0; num > 0; i++)
	{
		arr[i] = num % 2;
		num /= 2;
	}
	for (int i = 0; i < BYTE_LEN; i++)
	{
		binary += std::to_string(arr[i]);
	}
	std::reverse(binary.begin(), binary.end());
	return binary;
}
/*
Function : The function checks if the given code is logical
Input : The code to check
Output : wether it's valid or not.
*/
bool Helper::isCodeInEnum(int code)
{

	return code == Error || code == Signup || code == Login || (code <= GetPersonalStatus && code >= Logout);
}




//This function converts binary string to BYTE value(8 bits)
BYTE Helper::binToDec8(std::string byte)
{
	BYTE toReturn = 0;
	std::reverse(byte.begin(), byte.end());
	for (int i = 0; i < BYTE_LEN; i++)
	{
		if (byte[i] == '1')  // Check if the bit is on
		{
			toReturn += std::pow(2, i);
		}
	}
	return toReturn;
}

//This function converts binary string to BYTE value(32 bits)
size_t Helper::binToDec32(std::string byte)
{
	size_t toReturn = 0;
	std::reverse(byte.begin(), byte.end());
	for (int i = 0; i < BITS_ARRAY; i++)
	{
		if (byte[i] == '1')  // Check if the bit is on
		{
			toReturn += std::pow(2, i);
		}
	}
	return toReturn;
}