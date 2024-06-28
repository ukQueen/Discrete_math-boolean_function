#include "Tabl.h"
Tabl::Tabl() {
	x.resize(16, 0);
	y.resize(16, 0);
	z.resize(16, 0);
	e.resize(16, 0);
	f.resize(16, 0);

	int kx = 0, ky = 0, kz = 0, ke = 0;
	for (int i = 0; i < 16; i++) {
		
		x[i] = i < 8 ? 0 : 1;

		y[i] = (i & 4) == 4 || (i & 5) == 5 || (i & 6) == 6 || (i & 7) == 7 ? 1 : 0;

		z[i] = (i & 2) == 2 || (i & 3) == 3 ? 1 : 0;

		e[i] = i & 1 ? 1 : 0;

		f[i] = i == 2 || i == 3 || i == 4 || i == 5 || i == 8 || i == 9 || i == 14 || i == 15 ? 1 : 0;
	}


	BDR = new data('x');

	data* BDR_y0 = new data('y');
	data* BDR_y1 = new data('y');
	BDR->AddFalse(BDR_y0);
	BDR->AddTrue(BDR_y1);

	data* BDR_z0 = new data('z');
	data* BDR_z1 = new data('z');

	BDR_y0->AddFalse(BDR_z1);
	BDR_y0->AddTrue(BDR_z0);
	
	BDR_y1->AddFalse(BDR_z0);
	BDR_y1->AddTrue(BDR_z1);

	data* BDR_0 = new data('0');
	data* BDR_1 = new data('1');

	BDR_z0->AddFalse(BDR_1);
	BDR_z0->AddTrue(BDR_0);

	BDR_z1->AddFalse(BDR_0);
	BDR_z1->AddTrue(BDR_1);

	SDNF = "СДНФ = ";
	SKNF = "СКНФ = ";
	for (int i = 0, k1=0,k2=0; i < 16; i++) {
		if (f[i] == 1) {
			SDNF += x[i] == 1 ? "x " : "!x ";
			SDNF += y[i] == 1 ? "y " : "!y ";
			SDNF += z[i] == 1 ? "z " : "!z ";
			SDNF += e[i] == 1 ? "e " : "!e ";
			SDNF += k1 < 7 ? " V  " : "\0";
			k1++;
		}
		else {
			SKNF += "(";
			SKNF += x[i] == 0 ? "x" : "!x";
			SKNF += " V ";
			SKNF += y[i] == 0 ? "y" : "!y";
			SKNF += " V ";
			SKNF += z[i] == 0 ? "z" : "!z";
			SKNF += " V ";
			SKNF += e[i] == 0 ? "e" : "!e";
			SKNF += ")";
			SDNF += k2 < 7 ? "" : "\0";
			k2++;
		}
	}



	for (int i = 16; i >0; i--) {
		std::vector <bool> buf(i);
		Zhegalkin.push_back(buf);
	}
	for (int i = 0; i < 16; i++) {
		Zhegalkin[0][i] = f[i];
	}
	for (int i = 1; i < 16; i++) {
		for (int j = 0; j < Zhegalkin[i].size(); j++) {
			Zhegalkin[i][j] = Zhegalkin[i - 1][j] == Zhegalkin[i - 1][j + 1] ? 0 : 1;
		}
	}

	Alpha_index.push_back(0);
	Alpha_index.push_back(4);
	Alpha_index.push_back(3);
	Alpha_index.push_back(10);
	Alpha_index.push_back(2);
	Alpha_index.push_back(9);
	Alpha_index.push_back(8);
	Alpha_index.push_back(14);
	Alpha_index.push_back(1);
	Alpha_index.push_back(7);
	Alpha_index.push_back(6);
	Alpha_index.push_back(13);
	Alpha_index.push_back(5);
	Alpha_index.push_back(12);
	Alpha_index.push_back(11);
	Alpha_index.push_back(15);

	Zhegalkin_str = "F = ";
	for (int i = 0; i < 16; i++) {
		if (Zhegalkin[i][0] == 1) {
			Zhegalkin_str += "a_";
			Zhegalkin_str += std::to_string(Alpha_index[i]);
			Zhegalkin_str += " ";
			Zhegalkin_str += x[i] == 1 ? "x" : "";
			Zhegalkin_str += y[i] == 1 ? "y" : "";
			Zhegalkin_str += z[i] == 1 ? "z" : "";
			Zhegalkin_str += e[i] == 1 ? "e" : "";
			Zhegalkin_str += i < 15 ? " + " : "\0";
		}
	}
	
}

void Tabl::Print() {
	std::cout << "  " << "x y z e | f" <<std::endl;
	std::cout << "  -----------\n";
	for (int i = 0; i < 16; i++) {
		std::cout << "  " << x[i] << " " << y[i] << " " << z[i] << " " << e[i] <<" | " << f[i] << std::endl;
	}
}

void Tabl::Search(std::string s) {
	data* buf = this->BDR;
	for (int i = 0; i < 3; i++) {
		if (s[i] == '0')
			buf = buf->False;
		if (s[i] == '1')
			buf = buf->True;
	}
	std::printf("\tОтвет: %c\n", buf->name);
	std::cout << "\tПроверка по таблице истинности: " << Check(s) << std::endl;

}

void Tabl::Print_SDNF() {
	std::cout << std::endl<<SDNF << std::endl;
}

void Tabl::Print_SKNF() {
	std::cout << std::endl<<SKNF << std::endl;
}

void Tabl::Search_SDNF(std::string s) {
	std::string s_out = "";
	for (int i = 0; i < 4; i++) {
		s_out += s[i] == '0' ? "1" : "0";
	}
	
	std::string buf="";
	for (int i = 0; SDNF[i] != '\0'; i++) {
		buf += SDNF[i] == 'x' ? SDNF[i - 1] == '!' ? std::string(1, s_out[0]) : std::string(1, s[0]) : "";
		buf += SDNF[i] == 'y' ? SDNF[i - 1] == '!' ? std::string(1, s_out[1]) : std::string(1, s[1]) : "";
		buf += SDNF[i] == 'z' ? SDNF[i - 1] == '!' ? std::string(1, s_out[2]) : std::string(1, s[2]) : "";
		buf += SDNF[i] == 'e' ? SDNF[i - 1] == '!' ? std::string(1, s_out[3]) : std::string(1, s[3]) : "";
		buf += SDNF[i] == 'V' ? "+" : "";
		buf += SDNF[i + 1] == '\0' ? "+!\0" : "";
	}

	std::vector<int> result;
	for (int i = 0, k = 0; buf[i] != '!'; i += 5) {
		result.push_back(1);
		result[k] &= buf[i] == '1' ? 1 : 0;
		result[k] &= buf[i + 1] == '1' ? 1 : 0;
		result[k] &= buf[i + 2] == '1' ? 1 : 0;
		result[k] &= buf[i + 3] == '1' ? 1 : 0;
		k++;
	}
	int Result=0;
	for (int i = 0; i < result.size(); i++) {
		Result |=  result[i];
	}
	std::cout << "\tОтвет: " << Result << std::endl;
	std::cout << "\tПроверка по таблице истинности: " << Check(s) << std::endl;
}

void Tabl::Print_Zhegalkin() {
	std::string s = " ";
	std::printf(" Полином Жегалкина:\n");
	std::printf(" -----------------\n");
	for (int i = 0; i < 16; i++) {
		std::cout << "a_" << Alpha_index[i] <<'\t' << s;
			s += " ";
		for (int j = 0; j < Zhegalkin[i].size(); j++) {
			std::cout << Zhegalkin[i][j] << " ";
		}
		std::printf("\n");
	}
	std::cout <<std::endl<< Zhegalkin_str<<std::endl;
}

void Tabl::Search_Zhegalkin(std::string s) {
	std::string buf = "";
	for (int i = 0; Zhegalkin_str[i] != '\0'; i++) {
		buf += Zhegalkin_str[i] == 'x' ? std::string(1,s[0]) : "";
		buf += Zhegalkin_str[i] == 'y' ? std::string(1,s[1]) : "";
		buf += Zhegalkin_str[i] == 'z' ? std::string(1,s[2]) : "";
		buf += Zhegalkin_str[i] == 'e' ? std::string(1,s[3]) : "";
		buf += Zhegalkin_str[i] == '+' ? "+" : "";
	}

	bool result=buf[0]=='0'?0:1;
	bool buffer= buf[2] == '0' ? 0 : 1;
	bool buffer1= buf[4] == '0' ? 0 : 1;
	result = result != buffer ? 1 : 0;
	result = result != buffer1 ? 1 : 0;
	std::cout << "\tОтвет: " << result << std::endl;
	std::cout << "\tПроверка по таблице истинности: " << Check(s) << std::endl;
}

bool Tabl::Check(std::string s) {
	bool result = 0;
	bool s_0 = s[0] == '0' ? 0 : 1;
	bool s_1 = s[1] == '0' ? 0 : 1;
	bool s_2 = s[2] == '0' ? 0 : 1;
	bool s_3 = s[3] == '0' ? 0 : 1;

	for (int i = 0; i < 16; i++)
		if (x[i] == s_0)
			if (y[i] == s_1)
				if (z[i] == s_2)
					if (e[i] == s_3) {
						result = f[i];
						return result;;
					}
}

std::string Tabl::CheckInput_Menu() {
		std::string s;
	while (true) {
		std::cin >> s;
		try {
			if (stoi(s) >= 0 && stoi(s) <= 7) {
				break;
			}
			else {
				std::cout << "Ошибка ввода! Ведите число от 0 до 7\n";
			}
		}
		catch (const std::invalid_argument& e) {
			std::cout << "Ошибка ввода! Неверный формат числа\n";
		}
	}
	return s;
}

void Tabl::Menu() {
	while (true) {
		printf("\nВыберите что вы хотите:\n");
		printf(" [1]\tВывести на экран таблицу истинности\n");
		printf(" [2]\tПолином Жегалкина\n");
		printf(" [3]\tСДНФ\n");
		printf(" [4]\tСКНФ\n");
		printf(" [5]\tНайти значение по БДР\n");
		printf(" [6]\tНайти значение по СДНФ\n");
		printf(" [7]\tНайти значение по полиному Жегалкина\n");
		printf(" [0]\tВыход\n");
		std::string s;
		s = CheckInput_Menu();
		bool Out = 0;
		switch (stoi(s)) {
		case 0:
			Out = 1;
			break;
		case 1:
			Print();
			break;
		case 2:
			Print_Zhegalkin();
			break;
		case 3:
			Print_SDNF();
			break;
		case 4:
			Print_SKNF();
			break;
		case 5:
			s = CheckInput();
			Search(s);
			break;

		case 6:
			s = CheckInput();
			Search_SDNF(s);
			break;


		case 7:
			s = CheckInput();
			Search_Zhegalkin(s);
			break;
		}
		if (Out)
			break;
		system("pause");
	}
}

std::string Tabl::CheckInput() {
	std::regex BDR("^[0-1]{4}$");
	std::string s;
	std::printf("\nВведите значения переменных x, у, z, e в однустрочку без пробелов (пример: 1010)\n");
	while (true) {
		std::cin >> s;
		if (regex_match(s, BDR))
			break;
		else
			std::printf("Ошибка в вводе, попробуйте снова\n");
	}
	return s;
}