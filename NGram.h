#ifndef NGram_h
#define NGram_h

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>

class NGram {
public:
	class Node {
		
	public:
		int hits;
		std::map<std::string, Node*> children;
		
		Node();

		void addHit();
		void setHit(int newHits);
		void nodePrint(int levels, Node* p, std::string totalSeq = "");
		int nodeCount(int levels, Node* p);
		void nodeWrite(int levels, Node* p, std::string fName, std::string totalSeq = "");
		void startWrite(int levels, Node* p, std::string fName);
		void add(std::pair<std::string, Node*> newWord);
		std::string predict(Node* p, std::string sentence, std::string totalSeq = "");
	};

	void addWord(std::string newWord);
	void addSequence(std::string sent);
	void addModelFile(std::string fName);
	void addFile(std::string fName);
	void print(int levels);
	void ngramWrite(std::string fName, int levels = 3);
	std::string predict(std::string sentence, std::string totalSeq = "");
	Node rootNode;
};

#endif