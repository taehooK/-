#include "CommandFactory.h"
#include "Commands.h"
#include "FontCommand.h"
#include "NewCommand.h"
#include "OpenCommand.h"
#include "SaveCommand.h"
#include "SaveAsCommand.h"
#include "NotepadForm.h"
#include "resource.h"

CommandFactory::CommandFactory() {
}
CommandFactory::CommandFactory(const CommandFactory& source) {
}
CommandFactory::~CommandFactory() {
}
Command* CommandFactory::Make(int uID, NotepadForm *notepadForm) {
	Command *command = 0;
	if (uID == IDM_FORMAT_FONT) {
		command = new FontCommand(notepadForm);
	}
	else if (uID == IDM_FILE_NEW) {
		command = new NewCommand(notepadForm);
	}
	else if (uID == IDM_FILE_OPEN) {
		command = new OpenCommand(notepadForm);
	}
	else if (uID == IDM_FILE_SAVE) {
		command = new SaveCommand(notepadForm);
	}
	else if (uID == IDM_FILE_SAVEAS) {
		command = new SaveAsCommand(notepadForm);
	}
	else if (uID == IDM_AUTO_LINEBREAK) {
		command = new AutoLineBreakCommand(notepadForm);

	}

	return command;
}