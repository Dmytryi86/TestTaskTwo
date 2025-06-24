#include <locale>
#include <iostream>

#include <string>
#include <fstream>

#include <thread>
#include <unordered_set>

// удаляем ведущие нули из строки, содержащей число
std::string trimLeadingZeros(const std::string& numStr) 
{
	size_t first_non_zero = numStr.find_first_not_of('0');
	
	// Все нули или пустая строка
	if (first_non_zero == std::string::npos) 
	{
		return "0";
	}
	return numStr.substr(first_non_zero);
}

// получаем номер рейса из строки
std::string extractNumberString(const std::string& s) {
	
	size_t start_pos = 0;
	while (start_pos < s.size() && !std::isdigit(static_cast<unsigned char>(s[start_pos]))) 
	{
		++start_pos;
	}
	
	// Если цифр нет, возвращаем пустую строку
	if (start_pos == s.size()) {
		return "";
	}
	
	return s.substr(start_pos);
}

// очищаем код рейса от чисел в строке
std::string extractLeadingLetters(const std::string& s) 
{
	std::string result;
	for (char ch : s) 
	{
		if (std::isalpha(static_cast<unsigned char>(ch))) 
		{
			result += ch;
		} 
		else 
		{
			break;
		}
	}
	return result;
}

// парсим строку и получаем уникальный номер рейса
std:: string parserString(const std::string& str) 
{
	// удаляем все пробелы в строке
	auto remove_spaces = [](const std::string& s) 
	{
		std::string res;
		res.reserve(s.size());
		
		for (char c : s) 
		{
			if (c != ' ')
				res += c;
		}
		return res;
	};
	
	// очищаем код рейса от чисел
	auto code = extractLeadingLetters(str);
	
	// удаляем все пробелы в строке
	auto numStr = remove_spaces(str);
	
	// получаем номер рейса
	numStr = extractNumberString(numStr);
	
	// удаляем ведущие нули из номера рейса
	numStr = trimLeadingZeros(numStr);
	
	return code.append(numStr);
}

// функция обработки файла
void processFile(const std::string& inputFile, const std::string& outputFile) 
{
	std::ifstream fin(inputFile);
	
	if (!fin) 
	{
		std::cerr << "Не удалось открыть файл " << inputFile << std::endl;
		return;
	}
	
	std::unordered_set<std::string> uniqueCombinations;
	
	std::string line;
	std::string numCode;
	
	// чтение строк и сбор уникальных комбинаций
	while (std::getline(fin, line)) 
	{
		// парсим строку line
		numCode = parserString(line);
		uniqueCombinations.insert(numCode);
	}
	
	fin.close();
	
	// запись уникальных комбинаций в выходной файл
	std::ofstream fout(outputFile);
	
	if (!fout) 
	{
		std::cerr << "Не удалось открыть файл " << outputFile << " для записи" << std::endl;
		return;
	}
	
	for (const auto& comb : uniqueCombinations) 
	{
		fout << comb << "\n";
	}
}

int main() 
{
	setlocale(LC_ALL, "Russian");
	
	// запуск обработки файлов в параллельных потоках
	std::thread t1(processFile, "1_in.txt", "1_out.txt");
	std::thread t2(processFile, "2_in.txt", "2_out.txt");
	
	t1.join();
	t2.join();
	
	std::cout << "Обработка завершена." << std::endl;
	return 0;
}
