#include "Server.h"

Server::Server(IDatabase* db):
	m_dataBase(db), m_handlerFactory(db), m_communicator(m_handlerFactory)
{
}

Server::~Server()
{
}

void Server::run()
{
	std::string buffer = "";
	//Create the thread that handles the messages
	std::thread t_connector(&Communicator::startHandleRequests, std::ref(m_communicator));
	t_connector.detach();

	//Get to endless loop and listen to user
	while (true)
	{
		std::cout << "Enter:\n" <<
			"EXIT - To shut down the server.\n" <<
			"ADD QUESTION - To add a question to the DB.\n" <<
			"ALL QUESTIONS - To show all the questions." << std::endl;
		std::getline(std::cin, buffer);

		if (buffer.compare("EXIT") ==0)
		{
			break;
		}
		else if (buffer.compare("ADD QUESTION") == 0)
		{
			this->m_dataBase->addQuestion(getQuestion());
		}
		else if (buffer.compare("ALL QUESTIONS") == 0)
		{
			//Get all the questions and all of them
			std::vector<QuestionS> questions = this->m_dataBase->getAllQuestions();
			for (QuestionS question : questions)
			{
				printQuestion(question);
			}
		}
		system("pause");
		system("cls");
	}
}

QuestionS Server::getQuestion()
{
	QuestionS question;
	std::string buffer;
	//Get the question itself
	std::cout << "Enter the question:" << std::endl;
	std::getline(std::cin, buffer);
	question.question = buffer;
	//Get all the answers
	std::cout << "Enter the correct answer:" << std::endl;
	std::getline(std::cin, buffer);
	question.correctAns = buffer;
	std::cout << "Enter answer number 2:" << std::endl;
	std::getline(std::cin, buffer);
	question.ans2 = buffer;
	std::cout << "Enter answer number 3:" << std::endl;
	std::getline(std::cin, buffer);
	question.ans3 = buffer;
	std::cout << "Enter answer number 4:" << std::endl;
	std::getline(std::cin, buffer);
	question.ans4 = buffer;

	return question;
}

void Server::printQuestion(QuestionS& question)
{
	std::cout << "Id: " + std::to_string(question.questionId) << std::endl;
	std::cout << "Question: " + question.question << std::endl;
	std::cout << "Correct answer: " + question.correctAns << std::endl;
	std::cout << "Answer number 2: " + question.ans2 << std::endl;
	std::cout << "Answer number 3: " + question.ans3 << std::endl;
	std::cout << "Answer number 4: " + question.ans4 << std::endl;
}
