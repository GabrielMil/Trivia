#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "IDatabase.h"
//int name(void* fourthParam, int argc, char** argv, char** azColName)

typedef struct StrField {
	std::string field;
	std::string value;
}StrField;

typedef struct IntField {
	int value;
	std::string field;
}IntField;

typedef struct ForAvg
{
	float total;
	size_t count;
}ForAvg;

typedef struct ForSum
{
	size_t total;
	size_t count;
	std::string field;
}ForSum;

class CallBackFunctions
{
public:
	static int countRows(void* pCount, int argc, char** argv, char** azColName);
	static int getStrField(void* pField, int argc, char** argv, char** azColName);
	static int getIntValue(void* pValue, int argc, char** argv, char** azColName);

	static int getQuestion(void* pVectorOfQuestion, int argc, char** argv, char** azColName);
	static int getDataForAvg(void* pForAvg, int argc, char** argv, char** azColName);
	static int getDataForSum(void* pForSum, int argc, char** argv, char** azColName);
	static int getStrVector(void* pForStrs, int argc, char** argv, char** azColName);
	static int getIntVector(void* pForInts, int argc, char** argv, char** azColName);

	static int getStatsVector(void* pForStats, int argc, char** argv, char** azColName);
};