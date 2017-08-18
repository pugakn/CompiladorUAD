#include "ErrorModule.h"
#include <fstream>
using namespace std;

void ErrorModule::PushError(std::string phase, int lineNum, std::string description, std::string line)
{
    if (m_errors.size() < 50)
	{
		Error error(phase, lineNum, description, line);
		m_errors.push_back(error);
	}
}



std::vector<Error> ErrorModule::m_errors;
void ErrorModule::SaveFile()
{
	ofstream file;
	file.open("errors.err");
	for (auto &it : m_errors)
	{
		file << it.m_lineNum <<"\t" << it.m_description << "\t" << it.m_line << "\t" << it.m_phase <<endl;
	}
	file.close();
}
ErrorModule::ErrorModule()
{
}


ErrorModule::~ErrorModule()
{
}

Error::Error(std::string phase,int lineNum, std::string description, std::string line)
{
	m_phase = phase;
	m_line = line;
	m_description = description;
	m_lineNum = lineNum;
}
