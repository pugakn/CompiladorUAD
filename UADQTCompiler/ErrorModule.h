#include <string>
#include <vector>
struct Error
{
	int m_lineNum;
	std::string m_description;
	std::string m_line;
	std::string m_phase;
	Error(std::string phase, int lineNum, std::string description, std::string line);
	Error() {};
};
class ErrorModule
{
private:
	const size_t MAX_ERRORS = 10;
	//static const std::string phases[1];
public:
	static void PushError(std::string phase, int lineNum, std::string description, std::string line);
	static std::vector<Error> m_errors;
	static void SaveFile();
	ErrorModule();
	~ErrorModule();
};

