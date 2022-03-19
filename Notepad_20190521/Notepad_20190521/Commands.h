//Commands.h
#ifndef _COMMANDS_H
#define _COMMANDS_H

class NotepadForm;
class Command{
public:
	Command(NotepadForm *notepadForm = 0);
	Command(const Command& source);
	virtual ~Command() = 0;
	virtual void Execute() = 0;
	Command& operator=(const Command& source);
protected:
	NotepadForm *notepadForm;
};

//AutoLineBreakCommand.h
class AutoLineBreakCommand : public Command {
public:
	AutoLineBreakCommand(NotepadForm *notepadForm = 0);
	AutoLineBreakCommand(const AutoLineBreakCommand& source);
	virtual ~AutoLineBreakCommand();
	virtual void Execute();
	AutoLineBreakCommand& operator=(const AutoLineBreakCommand& source);

};


#endif // !_COMMANDS_H
