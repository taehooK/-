#include "OpenCommand.h"
#include "NeuronForm.h"
#include "Neuron.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkForm.h"
#include "Graph.h"
#include "GraphMaker.h"
#include "ScrollControllers.h"
#include <afxdlgs.h>
#include <fstream>

using namespace std;

OpenCommand::OpenCommand(NeuronForm *neuronForm) 
:Command(neuronForm){
}

OpenCommand::OpenCommand(const OpenCommand& source)
:Command(source){
}

OpenCommand::~OpenCommand(){
}

void OpenCommand::Execute(){
	//1. �ҷ����� �޴��� Ŭ�� ���� ��
	//	1.1.���� ���̾�α׸� ����Ѵ�.
	CFileDialog fileDialog(TRUE);
	int ret = fileDialog.DoModal();
	if (ret == IDOK) { //	1.2.���� ��ư�� ������ ��
		CString pathName = fileDialog.GetPathName(); 	//	1.2.1.���� �̸��� �д´�.
		fstream fs;
		fs.open((LPCTSTR)pathName, ios_base::in);
		if (fs.is_open()) {
			// 1.2.2.������ ����.

			if (this->neuronForm->neuralNetwork != NULL) {//	1.2.3.�������� �Ű���� �����.
				delete this->neuronForm->neuralNetwork;
			}
			this->neuronForm->neuralNetwork = new NeuralNetwork;//	1.2.4.�������� �Ű���� �����.
			this->neuronForm->neuralNetwork->Load(&fs);//	1.2.5.�������� �Ű������ �ҷ��´�.
			fs.close();
		}
		
		//	1.3.�������� ������Ʈ��ũ���� ���̾�ڽ���Ʈ���� �ٽø����.
		if (this->neuronForm->neuralNetworkForm->layerBoxControl != NULL) {
			delete this->neuronForm->neuralNetworkForm->layerBoxControl;
		}
		GraphMaker graphMaker;
		this->neuronForm->neuralNetworkForm->layerBoxControl = graphMaker.Make(dynamic_cast<NeuralNetwork*>(this->neuronForm->neuralNetwork), 20, 30, false);
		this->neuronForm->neuralNetworkForm->scrollController->Update();
		this->neuronForm->neuralNetworkForm->Invalidate();//	1.4.�������� ������Ʈ��ũ���� �����ϴ�.
	}
}

OpenCommand& OpenCommand::operator=(const OpenCommand& source){
	Command::operator=(source);
	return *this;

}