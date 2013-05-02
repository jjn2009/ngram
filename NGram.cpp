#include "NGram.h"

std::string readTxtFile(std::string fName)
{
	std::string allLines;
	std::string line;
	std::ifstream ngramFile(fName);
	if(ngramFile.is_open())
	{
		while(ngramFile.good())
		{
			getline(ngramFile, line);
			allLines += line+" ";
		}
	}

	return allLines;
}

std::string readNgramFile(std::string fName)
{
	std::string allLines;
	std::string line;
	std::ifstream ngramFile(fName);
	if(ngramFile.is_open())
	{
		while(ngramFile.good())
		{
			getline(ngramFile, line);
			allLines += line+"\n";
		}
	}

	return allLines;
}

void writeFile(std::string fName, std::string data)
{
	std::ofstream file;
	file.open(fName);
	file << data;
	file.close();
}


NGram::Node::Node() {
	hits = 1;
}

void NGram::Node::addHit()
{
	hits++;
}

void NGram::Node::setHit(int newHits)
{
	this->hits = newHits;
}

void NGram::Node::nodePrint(int levels, Node* p, std::string totalSeq)
{
	//Create map iterater set it to the first element of pointer map
	std::map<std::string, Node*>::iterator it;
	it = p->children.begin();

	//Each call of nodePrint goes down one level
	levels--;

	//If we've specified a stopping point or there are no children of current node then stop
	if(levels == 0 || it == p->children.end())
	{
		std::cout << totalSeq << " " << p->hits << std::endl;
	}

			

	//Iterate through children
	while(it != p->children.end() && levels != 0)
	{
		//Spawn another nodePrint
		nodePrint(levels, it->second, totalSeq+it->first + " ");
		it++;
	}
}

int NGram::Node::nodeCount(int levels, Node* p)
{
	int total = 0;
	//Create map iterater set it to the first element of pointer map
	std::map<std::string, Node*>::iterator it;
	it = p->children.begin();

	//Each call of nodePrint goes down one level
	levels--;
	//If we've specified a stopping point or there are no children of current node then stop
	if(levels == 0 || it == p->children.end())
		return 1;
	else
	{
		//Iterate through children
		while(it != p->children.end() && levels != 0)
		{
			//Spawn another nodePrint
			total += nodeCount(levels, it->second);
			it++;
		}

		return total;
	}
}

void NGram::Node::nodeWrite(int levels, Node* p, std::string fName, std::string totalSeq)
{
	std::ofstream fileHandle;
	//Create map iterater set it to the first element of pointer map
	std::map<std::string, Node*>::iterator it;
	it = p->children.begin();

	//Each call of nodePrint goes down one level
	levels--;

	//If we've specified a stopping point or there are no children of current node then stop
	if(levels == 0 || it == p->children.end())
	{
		fileHandle.open(fName,std::ios::app);
		fileHandle << totalSeq;
		fileHandle << " ";
		fileHandle << p->hits;
		fileHandle << "\n";
		fileHandle.close();
	}

	//Iterate through children
	while(it != p->children.end() && levels != 0)
	{
		//Spawn another nodePrint
		nodeWrite(levels, it->second, fName, totalSeq + it->first + " ");
		it++;
	}
}

void NGram::Node::startWrite(int levels, Node* p, std::string fName)
{
	nodeWrite(levels, p, fName);
}

void NGram::Node::add(std::pair<std::string, Node*> newWord)
{
	children.insert(newWord);
}


void NGram::addWord(std::string newWord)
{
	Node *p = &rootNode;
	p->add(std::pair<std::string, Node*>(newWord, new Node));
}

void NGram::addSequence(std::string sent)
{
	Node *p = &rootNode;
	Node *tempP;
	std::map<std::string, Node*>::iterator it;
	std::string buffer;
	std::stringstream ss(sent);
	std::string sentence;
	unsigned int j = 0;

	std::vector<std::string> words;
		
	//add words to a vector
	while (ss >> buffer)
		words.push_back(buffer);
		
	//loop through words
	for(unsigned int i = 0; i < words.size(); i++)
	{
		//reset pointer to root
		p = &rootNode;
			
		//If at the end of words vector break
		if(i+j > words.size())
				break;
		//loop through i to i+3
		for(j = 0; j < 3; j++)
		{
			sentence += words[i+j] + " ";

			//If word is already in ngram then add hit change pointer to next word
			if(p->children.count(words[i+j]))
			{
				p->children.find(words[i+j])->second->addHit();
				p = p->children.find(words[i+j])->second;
			}
			//else add the word to the tree set p to the new node
			else
			{
				tempP = p;
				tempP->add(std::pair<std::string, Node*>(words[i+j], p = new Node));
			}
		}
		sentence = "";
	}
}

void NGram::addModelFile(std::string fName)
{
	Node *p = &rootNode;
	Node *tempP;
	std::map<std::string, Node*>::iterator it;
	std::string line;
	std::ifstream ngramFile(fName);
	bool numeric;
	int n = 0, k = 0, tempint = 0;


	if(ngramFile.is_open())
	{
		//loop through all the lines
		while(ngramFile.good())
		{
			p = &rootNode;
			getline(ngramFile, line);
			std::string temp;

				
			std::string buffer;
			std::stringstream ss(line);

			std::vector<std::string> words;

			while (ss >> buffer)
				words.push_back(buffer);
			//loop through all the words
			for(unsigned int j = 0; j < words.size(); j++)
			{
				//assume numeric
				numeric = true;
				//loop through each character checking each is numeric
				//to see if the whole is numeric
				for(unsigned int l = 0; l < words[j].size(); l++)
				{
					if(!isdigit(words[j][l]))
					{
						numeric = false;
						break;
					}
				}

				//if k == 0 then we need to parse n and k
				if(k == 0)
				{
					//if first word is numeric record the number of n for the ngram
					if(numeric && j == 0)
					{
						n = atoi(words[j].c_str());
					}
					else if(numeric && j == 1)
					{
						k = atoi(words[j].c_str()) + 1;
					}
				}
				//else add the words
				else
				{
					//If word is already in ngram then change pointer to next word
					if(p->children.count(words[j]))
					{
						p = p->children.find(words[j])->second;
					//else add the word to the tree set p to the new node
					}
					else
					{
						tempP = p;
						tempP->add(std::pair<std::string, Node*>(words[j], new Node));

						if(j+1 == n)
						{
							tempint = atoi(words[j+1].c_str());
							p->children.find(words[j])->second->setHit(tempint);
							break;
						}
							
						p = p->children.find(words[j])->second;
					}
				}
			}
			k--;
		}
	}
}

void NGram::addFile(std::string fName)
{
	addSequence(readTxtFile(fName));
}

void NGram::print(int levels)
{
	levels++;
	std::cout << levels-1 << " " << rootNode.nodeCount(levels, &rootNode) << std::endl;
	rootNode.nodePrint(levels, &rootNode);
}

void NGram::ngramWrite(std::string fName, int levels)
{
	levels++;
	std::ofstream fileHandle;
	fileHandle.open(fName);
	fileHandle.close();
		
	for(int i = 1; i < levels; i++)
	{
		fileHandle.open(fName,std::ios::app);
		fileHandle << i;
		fileHandle << " ";
		fileHandle << rootNode.nodeCount(i+1, &rootNode);
		fileHandle << "\n";
		fileHandle.close();
		rootNode.nodeWrite(i+1, &rootNode, fName);
	}
}

std::string NGram::predict(std::string sentence, std::string totalSeq)
{
	return rootNode.predict(&rootNode, sentence, totalSeq);
}

std::string NGram::Node::predict(Node* p, std::string sentence, std::string totalSeq)
{
	//Create map iterater set it to the first element of pointer map
	std::map<std::string, Node*>::iterator it;
	it = p->children.begin();
	
	//If we've specified a stopping point or there are no children of current node then stop
	if(it == p->children.end())
	{
		std::cout << totalSeq << " " << p->hits << std::endl;
	}

			

	//Iterate through children
	while(it != p->children.end())
	{
		//Spawn another nodePrint
		predict(it->second, totalSeq+it->first + " ");
		it++;
	}
	return "";
}