#include "CallBackFunctions.h"

int CallBackFunctions::countRows(void* pParam, int argc, char** argv, char** azColName)
{
	int* pCount = (int*)pParam;
	(*pCount)++;
	return 0;
}

int CallBackFunctions::getStrField(void* pParam, int argc, char** argv, char** azColName)
{
	StrField* pField = (StrField*)pParam;
	for (int i = 0; i < argc; i++)  // Go through all the columns
	{
		if (std::string(azColName[i]) == pField->field)
		{
			pField->value = argv[i];
		}
	}
	return 0;
}

int CallBackFunctions::getIntValue(void* pValue, int argc, char** argv, char** azColName)
{
	(*((int*)pValue)) = std::stoi(argv[0]);  // We get inly 1 value that happens only once
	return 0;
}

//This function adds the question to the vector of questions
int CallBackFunctions::getQuestion(void* pVectorOfQuestion, int argc, char** argv, char** azColName)
{
	QuestionS curr;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "question_id")
		{
			curr.questionId = std::stoi(argv[i]);
		}
		else if (std::string(azColName[i]) == "question")
		{
			curr.question = argv[i];
		}
		else if (std::string(azColName[i]) == "correct_ans")
		{
			curr.correctAns = argv[i];
		}
		else if (std::string(azColName[i]) == "ans2")
		{
			curr.ans2 = argv[i];
		}
		else if (std::string(azColName[i]) == "ans3")
		{
			curr.ans3 = argv[i];
		}
		else if (std::string(azColName[i]) == "ans4")
		{
			curr.ans4 = argv[i];
		}
	}

	((std::vector<QuestionS>*)pVectorOfQuestion)->push_back(curr);
	return 0;
}

int CallBackFunctions::getDataForAvg(void* pForAvg, int argc, char** argv, char** azColName)
{
	//Increase the conut and the sum
	for (int i = 0; i < argc; i++)  // Go through all the columns
	{
		if (std::string(azColName[i]) == "avg_ans_time")
		{
			((ForAvg*)pForAvg)->count++;
			((ForAvg*)pForAvg)->total += std::stof(argv[i]);
		}
	}
	return 0;
}

int CallBackFunctions::getDataForSum(void* pForSum, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)  // Go through all the columns
	{
		if (std::string(azColName[i]) == ((ForSum*)pForSum)->field)
		{
			((ForSum*)pForSum)->count++;
			((ForSum*)pForSum)->total += std::stoi(argv[i]);
		}
	}
	return 0;
}

int CallBackFunctions::getStrVector(void* pForStrs, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)  // Go through all the columns
	{
		((std::vector<std::string>*)pForStrs)->push_back(argv[i]);
	}
	return 0;
}

int CallBackFunctions::getIntVector(void* pForInts, int argc, char** argv, char** azColName)
{
	((std::vector<int>*)pForInts)->push_back(std::stoi(argv[0]));
	return 0;
}

int CallBackFunctions::getStatsVector(void* pForStats, int argc, char** argv, char** azColName)
{
	StatisticData sData;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "game_id")
		{
			sData.gameId = std::stoi(argv[i]);
		}
		else if (std::string(azColName[i]) == "username")
		{
			sData.username = argv[i];
		}
		else if (std::string(azColName[i]) == "num_of_questions")
		{
			sData.numOfQuestions = std::stoi(argv[i]);
		}
		else if (std::string(azColName[i]) == "correct_answers")
		{
			sData.numOfCorrectAns = std::stoi(argv[i]);
		}
		else if (std::string(azColName[i]) == "avg_ans_time")
		{
			sData.avgAnsTime = std::stoi(argv[i]);
		}
	}

	((std::vector<StatisticData>*)pForStats)->push_back(sData);
	return 0;
}
