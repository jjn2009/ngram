/*
	Not quite working missing predict function
*/

#include "NGram.h"
using namespace std;

int main(int argc, char* argv[]) {
	NGram myngram;
	string userInput;
	
	string usage = "Usage:\n\tngram.exe <output model filename?\
				\n\t\t-Takes in a sentence from stdin then builds a ngram tree and\
				\n\t\toutputs specified format to the specified file\n\tngram.exe <input model filename> <n>\
				\n\t\t-Takes in specified file name and n for the ngram to be used\n\t\tin prediticing the next word of input from stdin";
	
	if(argc < 2)
		cout << usage << endl;
	else if(argc == 3)
	{
		myngram.addModelFile(argv[1]);
		cout << "Enter sentence for prediction: " << endl;
		getline(cin, userInput);
		myngram.predict(userInput);
	}
	else
	{
		cout << usage << endl;
	}
	return 0;
}