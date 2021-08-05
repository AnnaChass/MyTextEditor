
#include "MyEditorModel.h"


void MyEditorModel::SetNewBuffer(std::fstream& file)
{
	std::string temp;
	if (std::getline(file, temp))
		buffer.append(temp);
	while (std::getline(file, temp))
	{
		buffer.append("\n");
		buffer.append(temp);
	}
	NotifyFileUpdate();
}
bool MyEditorModel::OpenFile(std::fstream& file, char* filename, std::ios_base::openmode mode)
{
	char full_filename[128] = { 0 };
	strcpy(full_filename, FILEPATH);
	strcat(full_filename, filename);
	if (file.is_open())
	{
		file.close();
	}
	file.open(full_filename, mode);
	if (!file.is_open())
	{
		printw("\nError %d: opening file is failed.\n", GetLastError()); refresh();
		return false;
	}
	strcpy(filename_, filename);
	SetNewBuffer(file);
	CountStringNumber();
	NotifyFilenameUpdate();
	return file.is_open();
}
void MyEditorModel::CleanFile(std::fstream& file)
{
	file.close();
	OpenFile(file, filename_, std::fstream::out);
}
void MyEditorModel::CloseFile(std::fstream& file)
{
	file.close();
}
void MyEditorModel::WriteDown(std::fstream& file)
{
	file << buffer.c_str();
	file_is_changed_ = false;
}

int MyEditorModel::GetStringNumber()
{
	return string_number_;
}
const char* MyEditorModel::GetFileName()
{
	return static_cast<const char*>(filename_);
}
const char* MyEditorModel::GetBuffer()
{
	return buffer.c_str();
}
int MyEditorModel::GetBufferSize()
{
	return buffer.size();
}
const char* MyEditorModel::GetSubBuffer(int pos, int len)
{
	return buffer.substr(pos, len);
}
char MyEditorModel::GetSymbolByPos(int pos)
{
	if (pos > buffer.size() - 1) return -1;
	return buffer[pos];
}
void MyEditorModel::CountStringNumber()
{
	int eof_pos = buffer.size() - 1, cur_pos = 0, next_line_break_pos = 0;
	while (eof_pos > cur_pos)
	{
		next_line_break_pos = buffer.find("\n", cur_pos);
		if (next_line_break_pos == -1) next_line_break_pos = eof_pos;
		string_number_ += (next_line_break_pos - cur_pos) / COLS + (((next_line_break_pos - cur_pos) % COLS > 0) ? 1 : 0);
		if (next_line_break_pos == cur_pos) string_number_++;
		cur_pos = next_line_break_pos + 1;
	}
	NotifyStringUpdate();
}

void MyEditorModel::SetSymbol(char c, int pos)
{
	if (pos >= buffer.size()) return;
	buffer[pos] = c;
	NotifyFileUpdate();
}
bool MyEditorModel::OpenNewFile(char* filename)
{
	if (!OpenFile(file_, filename, std::fstream::in | std::fstream::out)) return false;
	memset(filename_, 0, sizeof(filename_));
	strcpy(filename_, filename);
	return true;
}
void MyEditorModel::WriteDownCurrentFile()
{
	CleanFile(file_);
	WriteDown(file_);
}
void MyEditorModel::WriteDownOtherFile(char* filename)
{
	std::fstream file;
	OpenFile(file, filename, std::fstream::out);
	WriteDown(file);
	CloseFile(file);
}
void MyEditorModel::CloseCurrentFile()
{
	CloseFile(file_);
}
bool MyEditorModel::FileIsChanged()
{
	return file_is_changed_;
}
void MyEditorModel::BufferInsert(const char* str, int pos)
{
	buffer.insert(pos, str);
	NotifyFileUpdate();
	NotifyStringUpdate();
}
int MyEditorModel::FindNextFrom(char c, int pos)
{
	char str[2] = { c , '\0' };
	return buffer.find(str, pos);
}
void MyEditorModel::DeleteSymbol(int pos)
{
	buffer.erase(pos, 1);
}