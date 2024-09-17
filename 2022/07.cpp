#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

vector<string> getFile(const char* name) {
	vector<string> lines;

	ifstream file(name);
	string str;
	while (getline(file, str)) {
		lines.emplace_back(str);
	}
	file.close();

	return lines;
}

uint64_t part1(const char* file);
uint64_t part2(const char* file);

int main(int argc, char** argv) {
	if (argc <= 1) {
		cout << "Usage: " << argv[0] << " [FILE_NAME]" << endl;

		return 0;
	}

	cout << "Day " << __FILE__[0] << __FILE__[1] << " part 1 : " << part1(argv[1]) << endl;
	cout << "Day " << __FILE__[0] << __FILE__[1] << " part 2 : " << part2(argv[1]) << endl;
}

class File {
  public:
	File(const std::size_t size, const std::string name) : mSize(size), mName(name) {}

	virtual std::size_t size() const noexcept { return mSize; }

  private:
	std::size_t mSize;
	std::string mName;
};

class Dir : public File {
  public:
	Dir(const std::string name) : File(0, name) {}

	std::size_t size() const noexcept override {
		return std::accumulate(mFiles.begin(), mFiles.end(), 0,
							   [](int x, const File* file) { return x + file->size(); });
	}
	void addFile(const File* file) noexcept { mFiles.emplace_back(file); }

  private:
	std::vector<const File*> mFiles;
};

uint64_t part1(const char* file) {
	const auto& lines = getFile(file);

	vector<File*> files;
	std::string pwd = "";
	stack<Dir*> pwdS;

	for (std::size_t i = 0; i < lines.size(); ++i) {
		const char* content = lines[i].data();

		assert(content[0] == '$');

		content += 2;

		if (content[0] == 'c') {
			content += 3;
			if (content[0] == '/') {
				files.clear();
				pwd = "";

				while (!pwdS.empty()) {
					pwdS.pop();
				}

				continue;
			}

			if (content[0] == '.' && content[1] == '.') {
				pwd.erase(pwd.find_last_of('/'));
				pwdS.pop();

				continue;
			}

			pwd += '/';
			pwd += content;
		} else {
			assert(content[0] == 'l');

			do {
				++i;

				const std::string entry = lines[i].data();
				std::stringstream entrys(entry);
				std::string name;

				if (entry.starts_with("dir")) {
					entrys >> name;
					entrys >> name;

					files.emplace_back(new Dir(pwd + '/' + name));
				} else {
					std::size_t size;
					entrys >> size >> name;

					files.emplace_back(new File(size, pwd + '/' + name));
				}
			} while (lines[i + 1][0] != '$');

			/*
			NOTE:
			dir e
			29116 f
			2557 g
			62596 h.lst
			 */
		}
	}

	return 0;
}

uint64_t part2(const char* file) { return 0; }
