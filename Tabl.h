#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <regex>


struct data {
	char name;
	data* False;
	data* True;

	data(const char s) {
		name = s;
		False = nullptr;
		True = nullptr;
	}
	void AddTrue(data* d) {
		data* buf = this;
		while (buf->True != nullptr) {
			buf = buf->True;
		}
		buf->True = d;
	}

	void AddFalse(data* d) {
		data* buf = this;
		while (buf->False != nullptr) {
			buf = buf->False;
		}
		buf->False = d;
	}
};

class Tabl
{
	std::vector<bool> x;
	std::vector<bool> y;
	std::vector<bool> z;
	std::vector<bool> e;
	std::vector<bool> f;
	data* BDR;
	std::string SDNF;
	std::string SKNF;
	std::vector <std::vector<bool>> Zhegalkin;
	std::vector <int> Alpha_index;
	std::string Zhegalkin_str;

	public:
	Tabl();
	void Print();
	void Print_SDNF();
	void Print_SKNF();
	void Print_Zhegalkin();
	void Search_SDNF(std::string s);
	void Search_Zhegalkin(std::string s);
	void Search(std::string s);
	//добавить в отчет
	void Menu();
	bool Check(std::string s);
	std::string CheckInput_Menu();
	std::string CheckInput();

};

