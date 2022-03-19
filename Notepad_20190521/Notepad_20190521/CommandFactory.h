#ifndef _COMMANDFACTORY_H
#define _COMMANDFACTORY_H

class NotepadForm;
class Command;

class CommandFactory {
public:
	CommandFactory();
	CommandFactory(const CommandFactory& source);
	~CommandFactory();
	Command* Make(int uID, NotepadForm *notepadForm);
};

#endif //_COMMANDFACTORY_H