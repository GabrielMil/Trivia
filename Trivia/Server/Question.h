#pragma once
#include <iostream>
#include <vector>

class Question 
{
public:
	Question(std::string question, std::vector<std::string> answers);
	~Question();
	void setQuestion(std::string);
	void setAnswers(std::vector<std::string>&);
	void setTime(int t);
	int getTime();
	std::string getQuestion();
	std::vector<std::string> getPossibleAnswers();
	std::string getCorrectAnswer();
	

private:

	int time;
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
};