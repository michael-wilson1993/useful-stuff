#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
vector<string> template1,template2;
string dest;
void generateTemplate(string name, vector<string> listname)
{
	name.pop_back();
	name.pop_back();
	int loc = name.size();
	while(name[loc] != '/')
		loc--;
	loc++;
	name.erase(0, loc);
	name = "Test" + name;
	cout << dest  << name << endl;

	ofstream fout(dest+name+".cpp");
	fout << ("/*\n	Author: Michael Wilson\n	proffessor: Dr. John Anvik\n	Class: Cpsc 3720\n*/\n");
	fout << ("CPPUNIT_TEST_SUITE_REGISTRATION(" + name + ");\n");
	fout << ("#include <Exceptions.h>\n");
	fout << ("#include <" + name + ".h>\n\n");

	for(int x = 0; x < listname.size(); x++)
		fout << "void " <<  name << "::" << listname[x] << "()\n{\n\n}\n\n";
	fout.close();

	fout.open(dest+name+".h");
	fout << "/*\nAuthor: Michael Wilson\n	proffessor: Dr. John Anvik\n	Class: Cpsc 3720\n*/\n#pragma once\n\n#include <cppunit/extensions/HelperMacros.h>";
	fout << "class " << name << "\n {\n	CPPUNIT_TEST_SUITE(" << name << ");\n";
	for(int x = 0; x < listname.size(); x++)
		fout << "CPPUNIT_TEST(" << listname[x] << ");\n";
	fout << "	CPPUNIT_TEST_SUITE_END();\n";
	fout << "public:\n";
	for(int x = 0; x < listname.size(); x++)
		fout << "	void " << listname[x] << "();\n";
	fout << "\n};";
	
	fout.close();

}
	
bool testIfFunction(string func)
{
	string names[6] = {"bool", "void", "string", "int", "float", "double"};
	bool checkType = false;
	bool checkparameter = false;
	for(int x = 0; x < 6; x++)
		if(-1 != func.find(names[x]))
			checkType = true;

	if(checkType)
	{
		for(int x = 0; x < func.size(); x++)
		{
			if(func[x] == '=')
				return false;
			if(func[x] == '(')
				checkparameter = true;
		}
	}
	if(checkType && checkparameter)
		return true;
	return false;
}

string getname(string func)
{
	int loc;
	for(int x = 0; x < func.size(); x++)
		if(func[x] == '(')
			loc = x;
	while(func[loc] != ' ' && loc > 0)
		loc--;
	loc++;
	func.erase(0, loc);
	loc = 0;
	while(func[loc] != '(')
		loc++;
	func.erase(loc, func.size()-loc);
	return func;
}

void createTest(string filename)
{
	string temp;
	ifstream fin(filename);
	vector<string> functionList;
	bool functionCheck = false;
	while(getline(fin, temp))
	{
		if(testIfFunction(temp))
		{
			functionList.push_back(getname(temp));
		}
	}
	generateTemplate(filename, functionList);
	for(int x = 0; x < functionList.size(); x++)
		cout << functionList[x] << endl;
}

int main(int argc, char *argv[])
{	
	string location = argv[1]; //location of header files
	string destination = argv[2]; // destination location
	dest = destination;
	string headerListFile = argv[3]; // filelist.txt
	vector<string> headerFileNames;
	ifstream fin(headerListFile);
	string temp;;
	while(getline(fin, temp))
		headerFileNames.push_back(location +temp);
	
	for(int x =0; x < headerFileNames.size(); x++)
		createTest(headerFileNames[x]);


	return 0;
}
