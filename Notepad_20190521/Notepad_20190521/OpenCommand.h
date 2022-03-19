#ifndef _OPENCOMMAND_H
#define _OPENCOMMAND_H

#include "Commands.h"
class NotepadForm;

class OpenCommand : public Command {
public:
	OpenCommand(NotepadForm *notepadForm = 0);
	OpenCommand(const OpenCommand& source);
	virtual ~OpenCommand();
	virtual void Execute();

	OpenCommand& operator =(const OpenCommand& source);
};

#endif //_OPENCOMMAND_H