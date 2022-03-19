#ifndef _CLIPBOARD_H

#include <string>

using namespace std;

typedef long int Long;

class Clipboard {
public:
	Clipboard();
	Clipboard(string content);
	Clipboard(const Clipboard& source);
	~Clipboard();
	void Copy(string content);
	string GetTextData();
	bool IsTextDataAvailable();

	Clipboard& operator=(const Clipboard& other);

private:
	string content;
};

#endif // !_CLIPBOARD_H
