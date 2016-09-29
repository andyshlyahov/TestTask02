#include <direct.h>
#include <Windows.h>
#include <algorithm>

class Tester
{
private:
	Container<std::string, std::string> container;
	bool lower_flag = false;

public:
	Tester() {};
	Tester(Container<std::string, std::string> _container) { container = _container; };
	void scan(std::string directory);
	void scan();
	void find(std::string filename);
};

void Tester::scan(std::string directory)
{
	if (!lower_flag)
	{
		std::transform(directory.begin(), directory.end(), directory.begin(), ::tolower);
		lower_flag = true;
	}

	HANDLE hSearch = NULL;
	WIN32_FIND_DATA findData;

	hSearch = FindFirstFile(directory.c_str(), &findData);

	if (hSearch == INVALID_HANDLE_VALUE)
	{
		std::cout << "Invalid path_to_the_directory" << std::endl;
		return;
	}

	if (hSearch != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(".", findData.cFileName) && strcmp("..", findData.cFileName))
			{
				DWORD32 flag = FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes;
				if (flag != FILE_ATTRIBUTE_DIRECTORY)
				{
					if (!strcmp(findData.cFileName,"New Text Document.txt"))
					{
						flag = flag;
					}
					container.add(findData.cFileName, directory);
				}
			}

			FindNextFile(hSearch, &findData);

		} while (GetLastError() != ERROR_NO_MORE_FILES);
	}

	directory = directory.substr(0, directory.length() - 2);
	if (directory[directory.length() - 1] == ':')
	{
		lower_flag = false;
		return;
	}

	const size_t last_slash_idx = directory.rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		directory = directory.substr(0, last_slash_idx + 1);
		directory = directory + '*';
	}
	scan(directory);
}

void Tester::scan()
{
	char current_work_dir[FILENAME_MAX];
	_getcwd(current_work_dir, sizeof(current_work_dir));

	scan(current_work_dir);
}

void Tester::find(std::string filename)
{
	std::queue<std::string> values = container.get(filename);
	if (values.empty())
	{
		std::cout << "There is no such file!" << std::endl;
		return;
	}
	std::cout << "File " << filename << " can be found in directories:" << std::endl;
	while (!values.empty())
	{
		std::cout << values.front() << std::endl;
		values.pop();
	}
}
