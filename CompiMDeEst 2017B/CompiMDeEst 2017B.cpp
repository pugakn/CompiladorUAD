// CompiMDeEst 2017B.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "LexicAnalizer.h"
#include "ErrorModule.h"
#include "SyntacticAnalizer.h"
#include <iostream>
extern "C"             
__declspec(dllexport)  
const int                   
__cdecl
	Compile(char* code, char *str, int len) {
	ErrorModule::m_errors.clear();
	LexicAnalizer parser;
	SyntacticAnalizer synAnalizer;
	parser.Parse(code);
	synAnalizer.Analize(parser);
	ErrorModule::SaveFile();
	std::string errorString;
	for (auto &err : ErrorModule::m_errors)
	{
		errorString += (err.m_description + "\t"+ std::to_string(err.m_lineNum) + "\t"+err.m_line + "\r" +"\n");
	}
	strcpy_s(str,len , errorString.c_str());
	//strcpy_s(str, len, code);
	return 1;
}
int main()
{

	//LexicAnalizer parser;
	//parser.Parse("float 9.c 9.99 jashduh // // usahduhsa&%$###<==>==>*/ >=====>< =9666_nñumeroxDZ24[2259];|| !!!=! != . \n & | + ¿ # ()[]{}(&=!======!!!!!|||||+-+-+*/*/ holaxd.ds xdDDDD24 xd 24.22 ....... 89/*6695&%%%%%%\"hola\"\"\"& | ||+-,.;/*/666.6::;;,,..");
	//parser.Print();
	//ErrorModule::SaveFile();
    return 0;
}

