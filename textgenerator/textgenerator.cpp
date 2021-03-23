//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<unordered_set>
#include<unordered_map>
#include<map>
#include<string>
#include<windows.h>
#include<fstream>
#include<vector>
#include<stdio.h>
#include<conio.h>

#define START "["
#define END "]"
struct Pairs {
	double probability; //частота встречи наиболее вероятных слов 
	std::vector<std::string> actualWords; //наиболее вероятные слова ,выбираем следующее именно из них
	// если их несколько - с помощью функции рандома
	std::unordered_map<std::string, double> words;
};
class TextHandle {
public:
	std::unordered_map<std::string, Pairs> Chain; //цепь маркова , слово - последующие возможные слова
	void wordDivision(std::string str, int i, std::string previous) { //разделение текста на слова
		std::string word = "";
		for (i; i < str.size() && unsigned char(str[i]) > 191 && unsigned char(str[i]) < 256; ++i) {
			if (unsigned char(str[i]) < 224) {//перевод слов с большой буквы на слова с маленькой
				char c = str[i] + 32;
				word.push_back(c);
			}
			else
				word.push_back(str[i]);
		}
		if (word != "")
			addingWords(word, previous);//добавление слов в цепь
		else {
			++i;
			word = previous;
		}
		if (i != str.size())
			wordDivision(str, i, word);//вызов функции пока не обработаем весь текст
		else {
			addingWords(END, word);
		}
	}
	void addingWords(std::string word, std::string previous) {
		auto pos = Chain.find(previous);
		if (pos->second.words.empty()) {
			pos->second.words.insert(std::make_pair(word, 1));
			pos->second.actualWords.push_back(word);
			pos->second.probability = 1;
		}
		else {
			if (pos->second.words.find(word) == pos->second.words.end())
				pos->second.words.insert(std::make_pair(word, 0));
			auto it = pos->second.words.find(word);
			it->second++;
			if (it->second > pos->second.probability && previous != word) {
				pos->second.actualWords.clear();
				pos->second.actualWords.push_back(word);
				pos->second.probability = it->second;
			}
			else if (it->second == pos->second.probability || previous == word)
				pos->second.actualWords.push_back(word);
		}
		Pairs edges;
		Chain.insert(std::make_pair(word, edges));
	}
	void push(std::string text) {
		Pairs edges;
		Chain.insert(std::make_pair(START, edges));
		wordDivision(text, 0, START);
	}
	/*void Show() {
		for (auto it : Chain) {
			std::cout << "word: " << it.first << std::endl;
			std::cout << "actual words : " << std::endl;
			for (auto k = 0; k < it.second.actualWords.size(); ++k) {
				std::cout << it.second.actualWords[k] << " ";
			}
			std::cout << '\n' << "all words: " << '\n';
			for (auto k : it.second.words) {
				std::cout << k.first << "  " << k.second << std::endl;
			}
			std::cout << std::endl;
		}
	}*/
};
class TextGenerator {
	TextHandle marcov;
	std::string newText = "";
	int countingProbability(std::string word) {
		int pos;
		if (marcov.Chain.find(word)->second.actualWords.size() > 1) {
			pos = std::rand() % marcov.Chain.find(word)->second.actualWords.size();
		}
		else {
			pos = 0;
		}
		return pos;
	}
	void compliationOfNewText(std::string word) {
		std::string newWord = marcov.Chain.find(word)->second.actualWords[countingProbability(word)];
		if (newWord != END) {
			newText.push_back(' ');
			newText = newText + newWord;
			compliationOfNewText(newWord);
		}
		else {
			newText.push_back('.');
		}
	}

public:
	std::string NewTextGenerator() {
		compliationOfNewText(START);
		return newText;
	}
	void Push(std::string text) {
		marcov.push(text);
	}
};
int main() {
	SetConsoleCP(1251); //для чтения и обработки русского языка
	SetConsoleOutputCP(1251); //для вывода русского языка
	//SetConsoleCP(866); //для чтения и обработки русского языка
	//SetConsoleOutputCP(866); //для вывода русского языка
	//setlocale(LC_ALL, ".1251");
	//system("chcp 65001");
	//setlocale(LC_CTYPE, "Russian");

	std::string text, text1;
	TextGenerator k;
	std::fstream out;
	out.open("text.txt", std::ios::app);
	if (out.is_open())
		out << ']' << std::endl;
	out.close();
	std::ifstream in("text.txt");
	if (in.is_open()) {
		std::getline(in, text, ']');
	}
	in.close();
	k.Push(text);
	text1 = k.NewTextGenerator();
	//std::cout << text;
	//std::fstream out;
	out.open("text1.txt");
	if (out.is_open())
		out << k.NewTextGenerator() << std::endl;
	out.close();
	std::fstream op("text1.txt");
	if (op.is_open()) {
		std::getline(op, text1);
		std::cout << text1;
	}
	std::cin.get();
	
	return 0;
}
