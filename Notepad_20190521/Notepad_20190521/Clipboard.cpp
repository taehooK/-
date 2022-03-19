#include "Clipboard.h"
#include <afxole.h>

Clipboard::Clipboard() {
}

Clipboard::Clipboard(string content) {
	this->content = content;
}

Clipboard::Clipboard(const Clipboard& source) {
	this->content = source.content;
}

Clipboard::~Clipboard() {

}

void Clipboard::Copy(string content) {
	
	if (OpenClipboard(0)) {
		HGLOBAL hglbCopy;
		LPTSTR lptstrCopy;
		unsigned int strSize = content.length();
		hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (strSize + 1) * sizeof(TCHAR));
		if (hglbCopy != 0) {
			lptstrCopy = (LPTSTR)GlobalLock(hglbCopy);
			memcpy(lptstrCopy, content.c_str(), strSize * sizeof(TCHAR));
			lptstrCopy[strSize] = (TCHAR)0;
			GlobalUnlock(hglbCopy);

			EmptyClipboard();
			SetClipboardData(CF_TEXT, hglbCopy);
		}
		CloseClipboard();
	}
}

string Clipboard::GetTextData() {
	unsigned int priority_list = CF_TEXT;
	LPTSTR clipboardData;
	LPTSTR lptstrPaste;
	unsigned int strSize;

	string ret;

	if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT) {
		if (OpenClipboard(0)) {
			HGLOBAL hglbPaste = GetClipboardData(CF_TEXT);
			if (hglbPaste != 0) {
				clipboardData = (LPSTR)GlobalLock(hglbPaste);
				strSize = strlen(clipboardData) + 1;
				
				lptstrPaste = new TCHAR[strSize];
				memcpy(lptstrPaste, clipboardData, strSize * sizeof(TCHAR));
				GlobalUnlock(hglbPaste);
				
				if (lptstrPaste != 0) {
					ret = string(lptstrPaste);
					delete lptstrPaste;
				}
			}
			CloseClipboard();
		}
	}
	return ret;
}

bool Clipboard::IsTextDataAvailable() {
	unsigned int priority_list = CF_TEXT;
	return GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT;
}

Clipboard& Clipboard::operator=(const Clipboard& other) {
	this->content = other.content;
	return *this;
}