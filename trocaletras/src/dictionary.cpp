#include "dictionary.h"
#include <fstream>
#include <iostream>
#include <algorithm>
//#include <stdlib.h>
//#include <thread>

using namespace std;

#define MAX_NUMBER_OF_LETTERS 8
#define MAX_NUMBER_OF_THREADS 4

char lowerAndRemoveAcentosAndCedilha(char in){
  if(in<='Z' && in>='A')
    return in-('Z'-'z');
  if (in == '�'|| in == '�' || in=='�' || in=='�' || in == '�' || in== '�' || in == '�' || in == '�') return 'a';
  if (in == '�'|| in == '�' || in=='�' || in=='�'|| in== '�') return 'e';
  if (in == '�'|| in == '�' || in=='�' || in=='�') return 'i';
  if (in == '�'|| in == '�' || in=='�' || in=='�' || in == '�' || in == '�' || in == '�' || in== '�') return 'o';
  if (in == '�'|| in == '�' || in=='�' || in=='�') return 'u';
  if (in == '�' || in == '�') return 'c';

  return in;
}

void Dictionary::readDICfile(std::string filename)
{
	ifstream fin;

	string temp;

	fin.open(filename.c_str());

	if (fin.fail())
	{
		cout<<"Failed to load <"<<filename << ">\n";
		throw "FAILED TO LOAD DICTIONARY";
	}

	while (!fin.eof())
	{
		getline(fin,temp);
		std::transform(temp.begin(), temp.end(), temp.begin(), lowerAndRemoveAcentosAndCedilha);
		if (temp.length()>1 && temp.find('-')==std::string::npos) //avoid empty lines, chars, and composite words
			if(temp.length()<=MAX_NUMBER_OF_LETTERS) //(LIMIT STUFF) avoid long words (should be optional
				words.push_back(temp);
		//cout<<temp<<endl;
	}

	fin.close();
}

bool compareWords (string i,string j) {
	//ver se o comprimento e menor ou maior
	if (i.length()<j.length()) return true;
	else if (i.length()>j.length()) return false;
	//se tem o mesmo tamanho compara as strings
	return i<j;
}

bool Dictionary::checkIfWordExists(std::string word)
{
	return binary_search(words.begin(), words.end(), word, compareWords);
}

void Dictionary::addNodes(Node *node,int letterIndex)
{
	if (letterIndex>MAX_NUMBER_OF_LETTERS) return; //(LIMIT STUFF)long word, cant use more recursive calls
	int dones=0;
/*cout<<"\n-CC"<<((int)words.size()-1);
cout<<"\nmmm"<<(int)words[99].size();
cout<<"\n"<<words[words.size()-2].length();*/
	for (int i = words.size()-1;
		i>=0 &&	letterIndex<(int)words[i].length()  && (dones<26||freeWordsWhenBuildingDFA);
	i--)
	{
		unsigned int charIndex =  (unsigned int) words[i][letterIndex]- (unsigned int) 'a';
		if(charIndex>26) {cout<<"\n---:::"<<words[i][letterIndex]; continue;}

		Node* aux = &dfa;
		for(int z =0 ; z<letterIndex;z++)
			if(aux==NULL) break;
			else aux=aux->possibleDestinations[(unsigned int) words[i][z]- (unsigned int) 'a'];
		if(aux!=node) continue;

		if( node->possibleDestinations[ charIndex ] == NULL)
		{
			node->possibleDestinations[ charIndex ] = new Node();
			node->possibleDestinations[ charIndex ]->wordDone = false;
		}
		if ( (!node->possibleDestinations[ charIndex ]->wordDone||freeWordsWhenBuildingDFA)
				&& ((int) words[i].length()-1)==letterIndex )
		{
			node->possibleDestinations[ charIndex ]->wordDone = true; dones++;
			if (freeWordsWhenBuildingDFA) words.erase(words.begin()+i,words.begin()+i);
		}
	}
//cout<<"\n__";
	for(int i = 0; i<26;i++) if (node->possibleDestinations[i]!=NULL) {
		/*std::pthread_mutex_lock(&mutex);
		if(numberOfAtiveThreads<=MAX_NUMBER_OF_THREADS)
			{
				numberOfAtiveThreads++;
				std::thread t1(addNodes, node->possibleDestinations[i],letterIndex+1);
			}
		else addNodes(node->possibleDestinations[i],letterIndex+1);
		pthread_mutex_unlock(&mutex);*/
		addNodes(node->possibleDestinations[i],letterIndex+1);
	}
	//numberOfAtiveThreads--;
}



void Dictionary::buildDFA()
{
	for(int i = 0; i<26;i++) dfa.possibleDestinations[i]=NULL;
	addNodes(&dfa,0);
}

void Dictionary::freeNodeHeap(Node* node)
{
	for(int i = 0; i<26;i++) if (node->possibleDestinations[i]!=NULL) freeNodeHeap(node->possibleDestinations[i]);
	delete node;
}
void Dictionary::freeHeap()
{
	for(int i = 0; i<26;i++) if (dfa.possibleDestinations[i]!=NULL) freeNodeHeap(dfa.possibleDestinations[i]);
}

