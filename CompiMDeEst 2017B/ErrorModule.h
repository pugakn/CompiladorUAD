#pragma once
#include <string>
#include <vector>
struct Error
{
	enum Phase
	{
		LEXIC
	};
	int m_lineNum;
	std::string m_description;
	std::string m_line;
	Error::Phase m_phase;
	Error(Error::Phase phase, int lineNum, std::string description, std::string line);
	Error() {};
};
class ErrorModule
{
private:
	const size_t MAX_ERRORS = 10;
	//static const std::string phases[1];
public:
	static void PushError(Error::Phase phase, int lineNum, std::string description, std::string line);
	static std::vector<Error> m_errors;
	static void SaveFile();
	ErrorModule();
	~ErrorModule();
};

