#include "SaveCommand.h"
#include "NeuronForm.h"
#include "Neuron.h"
#include <afxdlgs.h>
#include <fstream>

using namespace std;

SaveCommand::SaveCommand(NeuronForm *neuronForm)
:Command(neuronForm){
}

SaveCommand::SaveCommand(const SaveCommand& source)
:Command(source){
}

SaveCommand::~SaveCommand(){
}

void SaveCommand::Execute(){
	//�����ϴ� �޴��� ������ ��
	//1.1.���� ���̾�α׸� ����Ѵ�.
	CFileDialog fileDialog(FALSE);
	int ret = fileDialog.DoModal();
	if (ret == IDOK) {//1.2.���� ���̾�α׿��� Ȯ�� ��ư�� ������ ��
		CString pathName = fileDialog.GetPathName(); //1.2.1.���� �̸��� �д´�.
		fstream fs;
		fs.open((LPCTSTR)pathName, ios_base::out); //1.2.2.������ �����.

		if (fs.is_open()) {
			if (this->neuronForm->neuralNetwork->GetLength() > 0) {
				this->neuronForm->neuralNetwork->Save(&fs); //1.2.3.�������� ������ũ��ũ���� �����Ѵ�.
			}
			fs.close(); //1.2.3.������ �ݴ´�.
		}
	}

}

SaveCommand& SaveCommand::operator=(const SaveCommand& source){
	Command::operator=(source);
	return *this;
}