//FontCommand.h
#ifndef _FONTCOMMAND_H
#define _FONTCOMMNAD_H

#include "Commands.h"

class notepadForm;
class FontCommand : public Command {
public:
	FontCommand(NotepadForm *notepadForm = 0);
	FontCommand(const FontCommand& source);
	~FontCommand();
	virtual void Execute();
	FontCommand& operator =(const FontCommand& source);
};

#endif // !_FONTCOMMAND_H
