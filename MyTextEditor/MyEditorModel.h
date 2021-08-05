#pragma once
#include <curses.h>
#include <fstream>
#include <Windows.h>
#include "Observer.h"
#include "MyString.h"
#define FILEPATH "C:\\Users\\Anna\\Desktop\\"

class MyEditorModel : public Observable
{
private:
	MyString<char> buffer;
	std::fstream file_;
	char filename_[32];
	bool file_is_changed_ = false;
	int string_number_ = 0;

	void SetNewBuffer(std::fstream& file);
	bool OpenFile(std::fstream& file, char* filename, std::ios_base::openmode mode);
	void CleanFile(std::fstream& file);
	void CloseFile(std::fstream& file);
	void WriteDown(std::fstream& file);
	void CountStringNumber();

public:
	int GetStringNumber();
	const char* GetFileName();
	const char* GetBuffer();
	int GetBufferSize();
	const char* GetSubBuffer(int pos, int len);
	char GetSymbolByPos(int pos);

	void SetSymbol(char c, int pos);
	void BufferInsert(const char* str, int pos);
	void DeleteSymbol(int pos);

	bool OpenNewFile(char* filename);
	void WriteDownCurrentFile();
	void WriteDownOtherFile(char* filename);
	void CloseCurrentFile();
	bool FileIsChanged();

	int FindNextFrom(char c, int pos);
};

