#include "Question.h"

Question::Question(std::string question, std::vector<std::string> answers)
{
	this->m_question = question;
	this->m_possibleAnswers = answers;
	this->time = 0;
}

Question::~Question()
{
}

void Question::setQuestion(std::string q)
{
	this->m_question = q;
}

void Question::setAnswers(std::vector<std::string>& pa)
{
	this->m_possibleAnswers = pa;
}

void Question::setTime(int t)
{
	this->time += t;
}

int Question::getTime()
{
	return this->time;
}

std::string Question::getQuestion()
{
	return this->m_question;
}

std::vector<std::string> Question::getPossibleAnswers()
{
	return this->m_possibleAnswers;
}

std::string Question::getCorrectAnswer()
{
	return this->m_possibleAnswers[0];
}
