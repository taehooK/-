#include "GraphMaker.h"
#include "GraphFactory.h"
#include "Graph.h"
#include "CoordinateSystem.h"
#include "Line.h"
#include "Texts.h"
#include "Point.h"
#include "NeuralNetwork.h"
#include "Neuron.h"
#include "Affine.h"
#include "ReLU.h"
#include "Softmax.h"
#include "LayerBox.h"
#include "Arrow.h"
#include "LayerBoxControl.h"
#include "AddBox.h"
#include "Cell.h"
#include "Layer.h"
#include "Neuron.h"
#include "CompositeNeuron.h"
#include "Dropout.h"
#include "Node.h"
#include "Convolution.h"
#include "Pooling.h"
#include "LayerToolTip.h"
#include "RectangleGraph.h"
#include "Table.h"
#include "Sigmoid.h"
#include "ConvolutionFilter.h"
#include "PoolingFilter.h"
#include "SpatialTransformer.h"

GraphMaker::GraphMaker() {
}

GraphMaker::GraphMaker(const GraphMaker& source) {
}

GraphMaker::~GraphMaker() {
}

Graph* GraphMaker::Make(Array<double>& axisX, Array<Long>& axisY, Long left, Long top, Long right, Long bottom) {
	GraphFactory graphFactory;
	Graph *coordinateSystem;
	double magnificationX;
	double magnificationY;
	Long originX;
	Long originY;
	Graph *point;
	Long x;
	Long y;
	Long i;
	double minX = axisX[0];
	double maxX = axisX[0];
	Long minY = axisY[0];
	Long maxY = axisY[0]; //��� �迭��� Ȯ���ϸ� �ּڰ�, �ִ� ã�� ����

	Graph *verticalLine = graphFactory.Make(GRAPH_LINE);
	Graph *horizontalLine = graphFactory.Make(GRAPH_LINE);

	coordinateSystem = graphFactory.Make(GRAPH_COORDINATESYSTEM);
	i = 1; //�ʱ갪�� 0��° �迭����̹Ƿ� �� ������°���� Ȯ��
	while (i < axisX.GetLength()) {
		if (minX > axisX[i]) {
			minX = axisX[i];
		}
		else if (maxX < axisX[i]) {
			maxX = axisX[i];
		}
		if (minY > axisY[i]) {
			minY = axisY[i];
		}
		else if (maxY < axisY[i]) {
			maxY = axisY[i];
		}
		i++;
	}

	magnificationX = (right - left) / ((maxX - minX)*1.0f); //X����ǥ���� = x����� / x���ִ�����(1.0�� �Ǽ�ȭ)
	magnificationY = (bottom - top) / ((maxY - minY)*1.0f); //Y����ǥ���� = y����� / y���ִ�����
	originX = left - magnificationX * minX; //���� = x1 - ����*�ּڰ�
	originY = bottom + magnificationY * minY; //���� = y2 + ����*�ּڰ�
	*dynamic_cast<CoordinateSystem*>(coordinateSystem) = CoordinateSystem(256, magnificationX, magnificationY, originX, originY);
	//������ Graph* ���̹Ƿ� dynamic_cast�� ������ ȣ���� �� �ֵ��� ��.
	COLORREF color = RGB(0, 0, 0);
	*dynamic_cast<Line*>(verticalLine) = Line(left, top, left, bottom, color);
	*dynamic_cast<Line*>(horizontalLine) = Line(left, bottom, right, bottom, color);
	coordinateSystem->Add(verticalLine); //��ǥ���� graphs�� �߰�
	coordinateSystem->Add(horizontalLine);
	i = 0;
	while (i < axisX.GetLength()) { //x��, y���� ������ŭ �ݺ��Ѵ�.
		x = originX + (magnificationX*axisX[i]); //point�� �� x = ����x + ����*����x�൥����
		y = originY - (magnificationY*axisY[i]); //point�� �� y = ����y - ����*����y�൥����
		point = graphFactory.Make(GRAPH_POINT);
		*dynamic_cast<Point*>(point) = Point(x, y, color);
		coordinateSystem->Add(point);
		i++;
	}

	return coordinateSystem;
}

Graph* GraphMaker::Make(string titleX, string titleY, RECT rect, Long xMax, Long yMax) {
	GraphFactory graphFactory;
	Graph *coordinateSystem;

	double magnificationX;
	double magnificationY;
	Long originX;
	Long originY;

	//1. ��ǥ�踦 �����.
	coordinateSystem = graphFactory.Make(GRAPH_COORDINATESYSTEM);
	magnificationX = (rect.right - rect.left) / (xMax*1.0); //X����ǥ���� = x����� / x���ִ�����(1.0�� �Ǽ�ȭ)
	magnificationY = (rect.bottom - rect.top) / (yMax*1.0); //Y����ǥ���� = y����� / y���ִ�����
	originX = rect.left; // - magnificationX * minX; //���� = x1 - ����*�ּڰ�
	originY = rect.bottom; // +magnificationY * minY; //���� = y2 + ����*�ּڰ�
	*dynamic_cast<CoordinateSystem*>(coordinateSystem) = CoordinateSystem(256, magnificationX, magnificationY, originX, originY);
	
	//2. X�� ��, Y�� ���� �����.
	COLORREF color = RGB(0, 0, 0);
	Graph *verticalLine = graphFactory.Make(GRAPH_LINE);
	Graph *horizontalLine = graphFactory.Make(GRAPH_LINE);
	*dynamic_cast<Line*>(verticalLine) = Line(rect.left, rect.top, rect.left, rect.bottom, color);
	*dynamic_cast<Line*>(horizontalLine) = Line(rect.left, rect.bottom, rect.right, rect.bottom, color);
	coordinateSystem->Add(verticalLine); //��ǥ���� graphs�� �߰�
	coordinateSystem->Add(horizontalLine);

	//3. �� ������� �����.
	Graph *xTitleText = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Graph *yTitleText = graphFactory.Make(GRAPH_VERTICALTEXT);
	*dynamic_cast<Text*>(xTitleText) = HorizontalText(rect.left + (rect.right - rect.left) / 2 - 30, rect.bottom + 40, titleX, color); //x���� �߰�, x�༱���� 40��ŭ ������ ���� ����
	*dynamic_cast<Text*>(yTitleText) = VerticalText(rect.left - 70, rect.top + (rect.bottom - rect.top) / 2 + 30, titleY, color); //y���� �߰�, y�༱���� 60��ŭ ������ ���� ����
	coordinateSystem->Add(xTitleText);
	coordinateSystem->Add(yTitleText);
	//����
	Graph *zeroText = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	*dynamic_cast<Text*>(zeroText) = HorizontalText(rect.left - 10, rect.bottom + 10, "0", color); //���� ǥ��
	coordinateSystem->Add(zeroText);

	//4. X���� ���ݼ�, ������ �۾�, Y���� ���ݼ�, ������ �۾��� �����.
	Long i = 0;
	while (i < 4) { //��� 4���� �����
		Graph *xScaleLine = graphFactory.Make(GRAPH_LINE);
		Graph *yScaleLine = graphFactory.Make(GRAPH_LINE);
		Graph *xScaleText = graphFactory.Make(GRAPH_HORIZONTALTEXT);
		Graph *yScaleText = graphFactory.Make(GRAPH_HORIZONTALTEXT);
		//x�� ���ݼ�
		Long scaleX = rect.left + (i + 1) * ((rect.right - rect.left) / 4); //X���ݼ��� x =  X�༱ + (n * ���ݼ� ����)
		*dynamic_cast<Line*>(xScaleLine) = Line(scaleX, rect.bottom - 10, scaleX, rect.bottom, color); //x�࿡�� 10��ŭ �׾��� ���μ�
		coordinateSystem->Add(xScaleLine);

		//x�� ���� ������ �۾�
		string content = to_string((i + 1) * (xMax / 4)); //������ = �ִ�/4 * n
		*dynamic_cast<Text*>(xScaleText) = HorizontalText(scaleX - 20, rect.bottom + 10, content, color); //x�࿡�� 10��ŭ ������ ���� ������ (20���� ���� ����)
		coordinateSystem->Add(xScaleText);

		//y�� ���ݼ�
		Long scaleY = rect.bottom - (i + 1) * ((rect.bottom - rect.top) / 4); //Y���ݼ��� y = Y�༱ - (n * ���ݼ� ����)
		*dynamic_cast<Line*>(yScaleLine) = Line(rect.left, scaleY, rect.left + 10, scaleY, color); //y�࿡�� 10��ŭ �׾��� ���μ�
		coordinateSystem->Add(yScaleLine);

		//y�� ���� ������ �۾�
		content = to_string((i + 1) * (yMax / 4.0)); //������ = �ִ�/4 * n
		content.erase(4, 4); //�Ҽ��� �ڸ���
		*dynamic_cast<Text*>(yScaleText) = VerticalText(rect.left - 45, scaleY - 10, content, color); //y�࿡�� 10��ŭ ������ ���� ������ (25���� ���Ƿ� ����)
		coordinateSystem->Add(yScaleText);

		i++;
	}

	return coordinateSystem;
}

Graph* GraphMaker::Make(Array<Long>& axisX, Array<double>& axisY, RECT rect) {
	GraphFactory graphFactory;
	Graph *coordinateSystem;
	double magnificationX;
	double magnificationY;
	Long originX;
	Long originY;
	Graph *point;
	Long x;
	Long y;
	Long i;
	Long minX = axisX[axisX.Min(CompareLong)];
	minX = minX - minX;
	Long maxX = axisX[axisX.Max(CompareLong)];
	minX = minX + minX * 0.2;
	double minY = axisY[axisY.Min(Compare)];
	double maxY = axisY[axisY.Max(Compare)]; //��� �迭��� Ȯ���ϸ� �ּڰ�, �ִ� ã�� ����
	maxY = maxY + maxY * 0.2;

	Graph *verticalLine = graphFactory.Make(GRAPH_LINE);
	Graph *horizontalLine = graphFactory.Make(GRAPH_LINE);

	coordinateSystem = graphFactory.Make(GRAPH_COORDINATESYSTEM);

	magnificationX = (rect.right - rect.left) / ((maxX - minX)*1.0f); //X����ǥ���� = x����� / x���ִ�����(1.0�� �Ǽ�ȭ)
	magnificationY = (rect.bottom - rect.top) / ((maxY - minY)*1.0f); //Y����ǥ���� = y����� / y���ִ�����
	originX = rect.left - magnificationX * minX; //���� = x1 - ����*�ּڰ�
	originY = rect.bottom + magnificationY * minY; //���� = y2 + ����*�ּڰ�
	*dynamic_cast<CoordinateSystem*>(coordinateSystem) = CoordinateSystem(256, magnificationX, magnificationY, originX, originY);
	//������ Graph* ���̹Ƿ� dynamic_cast�� ������ ȣ���� �� �ֵ��� ��.
	COLORREF color = RGB(0, 0, 0);
	*dynamic_cast<Line*>(verticalLine) = Line(rect.left, rect.top, rect.left, rect.bottom, color);
	*dynamic_cast<Line*>(horizontalLine) = Line(rect.left, rect.bottom, rect.right, rect.bottom, color);
	coordinateSystem->Add(verticalLine); //��ǥ���� graphs�� �߰�
	coordinateSystem->Add(horizontalLine);
	i = 0;
	while (i < axisX.GetLength()) { //x��, y���� ������ŭ �ݺ��Ѵ�.
		x = originX + (magnificationX*axisX[i]); //point�� �� x = ����x + ����*����x�൥����
		y = originY - (magnificationY*axisY[i]); //point�� �� y = ����y - ����*����y�൥����
		point = graphFactory.Make(GRAPH_POINT);
		*dynamic_cast<Point*>(point) = Point(x, y, color);
		coordinateSystem->Add(point);
		i++;
	}

	return coordinateSystem;
}

Graph* GraphMaker::Make(NeuralNetwork *neuralNetwork, Long x, Long y, bool isClicked) {
	GraphFactory graphFactory;
	Graph *layerBoxControl = graphFactory.Make(GRAPH_LAYERBOXCONROL);
	string type;
	Graph *layerBox;
	Graph *addBox;
	RECT rect;
	Long boxWidth = layerBoxControl->GetBoxWidth();
	Long boxHeight = layerBoxControl->GetBoxHeight();
	Long interval = layerBoxControl->GetInterval();
	COLORREF color;

	Long i = 0;
	while (i < neuralNetwork->GetLength()) {
		type = dynamic_cast<CompositeNeuron*>(neuralNetwork->GetAt(i))->GetType();
		if (type == "Affine") {
			color = RGB(255, 0, 0); //������
		}
		else if (type == "ReLU") {
			color = RGB(0, 153, 0); //�ʷϻ�
		}
		else if (type == "SoftMax") {
			color = RGB(255, 204, 0); //�����
		}
		else if (type == "Dropout") {
			color = RGB(0, 0, 255); //�Ķ���
		}
		else if (type == "Conv") {
			color = RGB(153, 102, 0); //����
		}
		else if (type == "Pooling") {
			color = RGB(153, 0, 153); //�����
		}
		else if (type == "Sigmoid") {
			color = RGB(0, 255, 255);
		}
		else if (type == "STN") {
			color = RGB(153, 153, 153); //ȸ��
		}
		rect.left = x;
		rect.top = y + i * (boxHeight + interval);
		rect.right = rect.left + boxWidth;
		rect.bottom = rect.top + boxHeight;

		layerBox = new LayerBox(rect, type, color);
		layerBoxControl->Add(layerBox);
		i++;
	}
	//AddBox �߰�
	color = RGB(0, 0, 0);
	rect.left = x;
	rect.top = y + i * (boxHeight + interval);
	rect.right = rect.left + boxWidth;
	rect.bottom = rect.top + boxHeight;
	addBox = new AddBox(rect, color, isClicked);
	layerBoxControl->Add(addBox);

	return layerBoxControl;
}

Graph* GraphMaker::Make(Affine *affine, Long x, Long y) {
	CString number;
	RECT rect;
	GraphFactory graphFactory;
	Graph* affineLayout = graphFactory.Make(GRAPH_LAYOUT);
	Graph *table = graphFactory.Make(GRAPH_TABLE);

	Long rowCount = affine->GetAt(0)->GetLength() + 1; // ����ġ ����(��)
	Long columnCount = affine->GetLength() + 1; // ��� ����(��)
	Long rowInterval = 50;
	Long columnInterval = 100;
	Long width = columnCount * rowInterval;
	Long height = rowCount * columnInterval;
	*dynamic_cast<Table*>(table) = Table((rowCount * columnCount), rowCount, columnCount, width, height, rowInterval, columnInterval);



	// ù��° ���� ������ �߰��Ѵ�.
	COLORREF color = RGB(0, 0, 0);
	Graph *cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x;
	rect.top = y;
	rect.right = rect.left + columnInterval;
	rect.bottom = rect.top + rowInterval;
	*dynamic_cast<Cell*>(cell) = Cell(rect, "", color);

	table->Add(cell);
	Long i = 1;

	while (i < columnCount) {
		number.Format("%d", i);
		rect.left = rect.right;
		rect.top = y;
		rect.right = rect.left + columnInterval;
		rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		table->Add(cell);

		i++;
	}

	Array<double> weights(affine->GetWeights());

	Long j;
	i = 1;
	while (i < rowCount) {
		number.Format("%d", i);
		rect.left = x;
		rect.top = y + i * rowInterval;
		rect.right = rect.left + columnInterval;
		rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		table->Add(cell);

		j = 1;
		while (j < columnCount) {
			number.Format("%.2f", weights.GetAt((j - 1)*(rowCount - 1) + (i - 1)));
			rect.left = rect.right;
			rect.top = y + i * rowInterval;
			rect.right = rect.left + columnInterval;
			rect.bottom = rect.top + rowInterval;
			cell = graphFactory.Make(GRAPH_CELL);
			*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
			table->Add(cell);
			j++;
		}

		i++;
	}


	affineLayout->Add(table);

	return affineLayout;
}


Graph* GraphMaker::Make(ReLU *relu, Long x, Long y) {
	GraphFactory graphFactory;
	Graph *reluLayout = graphFactory.Make(GRAPH_LAYOUT);

	COLORREF color = RGB(0, 0, 0);
	//�Է°� ǥ�� ����
	Graph *inputText = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	*dynamic_cast<Text*>(inputText) = HorizontalText(x, y, "�Է�", color);
	reluLayout->Add(inputText);

	//�Է°� ǥ
	Graph *inputTable = graphFactory.Make(GRAPH_TABLE);
	Long rowCount = 2;
	Long columnCount = relu->GetLength() + 1;
	Long rowInterval = 50;
	Long columnInterval = 100;
	Long width = columnCount * rowInterval; //��ü��=������*
	Long height = rowCount * columnInterval;
	*dynamic_cast<Table*>(inputTable) = Table((rowCount * columnCount), rowCount, columnCount, width, height, rowInterval, columnInterval);

	//inputTable�� ù��° ��
	RECT rect;
	Graph *cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = y + 30;
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval;
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��ȣ", color);

	inputTable->Add(cell);

	CString number;
	//ù��° �࿡ ���� ä���
	Long i = 1;
	while (i < columnCount) {
		number.Format("%d", i);
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		inputTable->Add(cell);
		i++;
	}
	//inputTable�� �� ��° ��
	cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = y + 30 + 50; //100
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval; //200
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��", color);
	inputTable->Add(cell);
	//�ι�° �࿡ �� ä���
	i = 1;
	while (i < columnCount) {
		number.Format("%.4f", relu->GetInput(i - 1));
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		inputTable->Add(cell);
		i++;
	}
	reluLayout->Add(inputTable);

	//����� ǥ�� ����
	Graph *resultText = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	*dynamic_cast<Text*>(resultText) = HorizontalText(x, rect.bottom + 10, "���", color);
	reluLayout->Add(resultText);

	//����� ǥ
	Graph *resultTable = graphFactory.Make(GRAPH_TABLE);
	rowCount = 2;
	columnCount = relu->GetLength() + 1;
	rowInterval = 50;
	columnInterval = 100;
	width = columnCount * rowInterval; //��ü��=������*
	height = rowCount * columnInterval;
	*dynamic_cast<Table*>(resultTable) = Table((rowCount * columnCount), rowCount, columnCount, width, height, rowInterval, columnInterval);

	//resultTable�� ù��° ��
	cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = (rect.bottom + 10) + 30; //����� ����y + 40
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval; //���������y + 40 + 50
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��ȣ", color);

	resultTable->Add(cell);

	number;
	//ù��° �࿡ ���� ä���
	i = 1;
	while (i < columnCount) {
		number.Format("%d", i);
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		resultTable->Add(cell);
		i++;
	}
	//resultTable�� �� ��° ��
	cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = rect.bottom; //100
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval; //200
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��", color);
	resultTable->Add(cell);
	//�ι�° �࿡ �� ä���
	Array<double> results(relu->GetResults());
	i = 1;
	while (i < columnCount) {
		number.Format("%.4f", results.GetAt(i - 1));
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		resultTable->Add(cell);
		i++;
	}
	reluLayout->Add(resultTable);

	return reluLayout;
}

Graph* GraphMaker::Make(Softmax *softmax, Long x, Long y) {
	GraphFactory graphFactory;
	Graph *softmaxLayout = graphFactory.Make(GRAPH_LAYOUT);

	COLORREF color = RGB(0, 0, 0);
	//�Է°� ǥ�� ����
	Graph *inputText = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	*dynamic_cast<Text*>(inputText) = HorizontalText(x, y, "�Է�", color);
	softmaxLayout->Add(inputText);

	//�Է°� ǥ
	Graph *inputTable = graphFactory.Make(GRAPH_TABLE);
	Long rowCount = 2;
	Long columnCount = softmax->GetLength() + 1;
	Long rowInterval = 50;
	Long columnInterval = 100;
	Long width = columnCount * rowInterval; //��ü��=������*
	Long height = rowCount * columnInterval;
	*dynamic_cast<Table*>(inputTable) = Table((rowCount * columnCount), rowCount, columnCount, width, height, rowInterval, columnInterval);

	//inputTable�� ù��° ��
	RECT rect;
	Graph *cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = y + 30;
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval;
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��ȣ", color);

	inputTable->Add(cell);

	CString number;
	//ù��° �࿡ ���� ä���
	Long i = 1;
	while (i < columnCount) {
		number.Format("%d", i);
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		inputTable->Add(cell);
		i++;
	}
	//inputTable�� �� ��° ��
	cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = y + 30 + 50; //100
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval; //200
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��", color);
	inputTable->Add(cell);
	//�ι�° �࿡ �� ä���
	i = 1;
	while (i < columnCount) {
		number.Format("%.4f", softmax->GetInput(i - 1));
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		inputTable->Add(cell);
		i++;
	}
	softmaxLayout->Add(inputTable);

	//����� ǥ�� ����
	Array<double> results(softmax->GetResults());
	double sum = 0.0;
	i = 0;
	while (i < results.GetLength()) {
		sum += results.GetAt(i);
		i++;
	}
	CString title;
	title.Format("��� (�� : %d)", (Long)sum);
	Graph *resultText = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	*dynamic_cast<Text*>(resultText) = HorizontalText(x, rect.bottom + 10, (LPCTSTR)title, color);
	softmaxLayout->Add(resultText);

	//����� ǥ
	Graph *resultTable = graphFactory.Make(GRAPH_TABLE);
	rowCount = 2;
	columnCount = softmax->GetLength() + 1;
	rowInterval = 50;
	columnInterval = 100;
	width = columnCount * rowInterval; //��ü��=������*
	height = rowCount * columnInterval;
	*dynamic_cast<Table*>(resultTable) = Table((rowCount * columnCount), rowCount, columnCount, width, height, rowInterval, columnInterval);

	//resultTable�� ù��° ��
	cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = (rect.bottom + 10) + 30; //����� ����y + 40
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval; //���������y + 40 + 50
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��ȣ", color);

	resultTable->Add(cell);

	number;
	//ù��° �࿡ ���� ä���
	i = 1;
	while (i < columnCount) {
		number.Format("%d", i - 1);
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		resultTable->Add(cell);
		i++;
	}
	//resultTable�� �� ��° ��
	cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = rect.bottom; //100
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval; //200
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��", color);
	resultTable->Add(cell);
	//�ι�° �࿡ �� ä���
	i = 1;
	while (i < columnCount) {
		number.Format("%.4f", results.GetAt(i - 1));
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		resultTable->Add(cell);
		i++;
	}
	softmaxLayout->Add(resultTable);

	return softmaxLayout;
}

Graph* GraphMaker::Make(Dropout *dropout, Long x, Long y) {
	GraphFactory graphFactory;
	Graph *dropoutLayout = graphFactory.Make(GRAPH_LAYOUT);

	COLORREF color = RGB(0, 0, 0);
	//�Է°� ǥ�� ����
	Graph *inputText = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	*dynamic_cast<Text*>(inputText) = HorizontalText(x, y, "�Է�", color);
	dropoutLayout->Add(inputText);

	//�Է°� ǥ
	Graph *inputTable = graphFactory.Make(GRAPH_TABLE);
	Long rowCount = 2;
	Long columnCount = dropout->GetLength() + 1;
	Long rowInterval = 50;
	Long columnInterval = 100;
	Long width = columnCount * rowInterval; //��ü��=������*
	Long height = rowCount * columnInterval;
	*dynamic_cast<Table*>(inputTable) = Table((rowCount * columnCount), rowCount, columnCount, width, height, rowInterval, columnInterval);

	//inputTable�� ù��° ��
	RECT rect;
	Graph *cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = y + 30;
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval;
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��ȣ", color);

	inputTable->Add(cell);

	CString number;
	//ù��° �࿡ ���� ä���
	Long i = 1;
	while (i < columnCount) {
		number.Format("%d", i);
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		inputTable->Add(cell);
		i++;
	}
	//inputTable�� �� ��° ��
	cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = y + 30 + 50; //100
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval; //200
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��", color);
	inputTable->Add(cell);
	//�ι�° �࿡ �� ä���
	i = 1;
	while (i < columnCount) {
		number.Format("%.4f", dropout->GetInput(i - 1));
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		inputTable->Add(cell);
		i++;
	}
	dropoutLayout->Add(inputTable);

	//����� ǥ�� ����
	Graph *resultText = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	*dynamic_cast<Text*>(resultText) = HorizontalText(x, rect.bottom + 10, "���", color);
	dropoutLayout->Add(resultText);

	//����� ǥ
	Graph *resultTable = graphFactory.Make(GRAPH_TABLE);
	rowCount = 2;
	columnCount = dropout->GetLength() + 1;
	rowInterval = 50;
	columnInterval = 100;
	width = columnCount * rowInterval; //��ü��=������*
	height = rowCount * columnInterval;
	*dynamic_cast<Table*>(resultTable) = Table((rowCount * columnCount), rowCount, columnCount, width, height, rowInterval, columnInterval);

	//resultTable�� ù��° ��
	cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = (rect.bottom + 10) + 30; //����� ����y + 40
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval; //���������y + 40 + 50
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��ȣ", color);

	resultTable->Add(cell);

	//ù��° �࿡ ���� ä���
	i = 1;
	while (i < columnCount) {
		number.Format("%d", i);
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		resultTable->Add(cell);
		i++;
	}
	//resultTable�� �� ��° ��
	cell = graphFactory.Make(GRAPH_CELL);
	rect.left = x; //10
	rect.top = rect.bottom; //100
	rect.right = rect.left + columnInterval; //110
	rect.bottom = rect.top + rowInterval; //200
	*dynamic_cast<Cell*>(cell) = Cell(rect, "��", color);
	resultTable->Add(cell);
	//�ι�° �࿡ �� ä���
	Graph *line1;
	Graph *line2;
	Array<double> results(dropout->GetResults());
	i = 1;
	while (i < columnCount) {
		number.Format("%.4f", results.GetAt(i - 1));
		rect.left = rect.right;
		//rect.top = y;
		rect.right = rect.left + columnInterval;
		//rect.bottom = rect.top + rowInterval;
		cell = graphFactory.Make(GRAPH_CELL);
		//��Ȱ��ȭ�� ���� cell�� Xǥ���ϱ�
		if (dynamic_cast<Node*>(dropout->GetAt(i - 1))->GetIsActive() == false) {
			line1 = graphFactory.Make(GRAPH_LINE);
			*dynamic_cast<Line*>(line1) = Line(rect.left, rect.top, rect.right, rect.bottom, color);
			resultTable->Add(line1);
			line2 = graphFactory.Make(GRAPH_LINE);
			*dynamic_cast<Line*>(line2) = Line(rect.left, rect.bottom, rect.right, rect.top, color);
			resultTable->Add(line2);
			number = " ";
		}
		*dynamic_cast<Cell*>(cell) = Cell(rect, (LPCTSTR)number, color);
		resultTable->Add(cell);
		i++;
	}
	dropoutLayout->Add(resultTable);

	return dropoutLayout;
}

Graph* GraphMaker::Make(Convolution *convolution, Long x, Long y) {
	GraphFactory graphFactory;
	Graph *convolutionLayout = graphFactory.Make(GRAPH_LAYOUT);

	RECT rect = { x, y, x + 10, y + 10 };

	//����ġ
	Long n = 0;
	//1. ������ ����ť�� ������ŭ �ݺ��Ѵ�.
	Long i = 0;
	while (i < convolution->GetLength()) {
		//1.1. ����ġ����� ũ�⸦ ���Ѵ�.
		Long filterSize = dynamic_cast<Convolution*>(convolution)->GetFilterSize();
		Matrix<double> weights(filterSize, filterSize);
		if (i != 0 && (i % 16 == 0)) { //����ť�꺰�� '����'�ؼ� ����ϱ�
			n = 0; //rect.left�� x�� ����� ����ġ
			y += filterSize * 10 + 5; //�����ϱ�
		}
		//����ť�꺰 ����ȼ� �ʱ� ��ǥ = x, y+(�ݺ�Ƚ��*5*���ũ��), x+5, y+5
		rect = { x + (n * 10 * filterSize) + 20, y, x + (n * 10 * filterSize) + 20 + 10, y + 10 };
		n++;
		//1.2. ����ť�긦 �����´�.
		Neuron *filterCube = convolution->GetAt(i);
		//1.3. ���� ����ť���� ���� ������ŭ �ݺ��Ѵ�.
		Long j = 0;
		while (j < filterCube->GetLength()) {
			//1.3.1. ���͸� �����´�.
			Neuron *filter = filterCube->GetAt(j);
			//1.3.2. ���ͺ� ����ġ ����� �����.
			Long m = 0;
			//1.3.3. ����ġ ����� ũ�⸸ŭ �ݺ��Ѵ�.
			Long k = 0;
			while (k < weights.GetRow()) {
				//1.3.3.1. ����ġ ����� ũ�⸸ŭ �ݺ��Ѵ�.
				Long l = 0;
				while (l < weights.GetColumn()) {
					//1.3.3.1.1. ���ͺ� ����ġ�� �����´�.
					weights.Store(m++, dynamic_cast<ConvolutionFilter*>(filter)->GetWeight(k, l));
					l++;
				}
				k++;
			}
			j++;
		}
		//1.4. ���� ����ť���� ����ġ ����� �ȼ����� ���ϴ�. (1 - ��) * 255
		//1.5. ��� ����� �� ����ŭ �ݺ��Ѵ�.

		j = 0;
		while (j < weights.GetRow()) {
			//����� �簢���� ��ǥ = �ʱ���ǥ.left, �ʱ���ǥ.top + (�� �ݺ�Ƚ�� * 5), �ʱ���ǥ.right, �ʱ���ǥ.bottom + (�� �ݺ�Ƚ�� *5)
			RECT weightsRect = { rect.left + (n * 10), rect.top + (j * 10), rect.right + (n * 10), rect.bottom + (j * 10) };
			//1.7.1. ��� ����� �� ����ŭ �ݺ��Ѵ�.
			Long k = 0;
			while (k < weights.GetColumn()) {
				double rgb = weights.GetAt(j, k);
				COLORREF color;
				if (rgb >= 0.0) {
					rgb = rgb * 100;
					color = RGB(255 - (int)rgb, 255, 255);
				}
				else {
					rgb = -rgb * 100;
					color = RGB(255, 255 - (int)rgb, 255);
				}
				//1.7.1.1. Layout�� ������ ����� �ȼ����� �簢���� �߰��ϴ�.
				Graph *rectangle
					= new Cell(CRect(weightsRect.left + (k * 10), weightsRect.top, weightsRect.right + (k * 10), weightsRect.bottom),
						"", color);
				convolutionLayout->Add(rectangle);
				k++;
			}
			j++;
		}
		i++;
	}

	//��� ���
	n = 0;
	y += 100;
	//1. ������ ����ť�� ������ŭ �ݺ��Ѵ�.
	i = 0;
	while (i < convolution->GetLength()) {
		//1.1. �������� ũ�⸦ ���Ѵ�.
		Long resultsSize = dynamic_cast<ConvolutionFilter*>(convolution->GetAt(0)->GetAt(0))->GetResultsSize();
		if (i != 0 && (i % 4 == 0)) { //ȭ�鿡 ���η� 4���� ����� ���� �� ���� '����'�ؼ� ����ϱ�
			n = 0; //rect.left�� x�� ����� ����ġ
			y += resultsSize * 10 + 5; //�����ϱ�
		}
		//����ť�꺰 ����ȼ� �ʱ� ��ǥ = x, y+(�ݺ�Ƚ��*5*���ũ��), x+5, y+5
		rect = { x + (n * 10 * resultsSize) + 20, y, x + (n * 10 * resultsSize) + 20 + 10, y + 10 };
		n++;
		//1.2. �������� �����.
		Matrix<double> results(resultsSize, resultsSize);
		//1.3. �������� �ʱ�ȭ�Ѵ�.
		Long j = 0;
		while (j < resultsSize*resultsSize) {
			results.Store(j, 0.0);
			j++;
		}
		//1.4. ����ť�긦 �����´�.
		Neuron *filterCube = convolution->GetAt(i);
		//1.5. ���� ����ť���� ���� ������ŭ �ݺ��Ѵ�.
		j = 0;
		while (j < filterCube->GetLength()) {
			//1.5.1. ���͸� �����´�.
			Neuron *filter = filterCube->GetAt(j);
			//1.5.2. ���ͺ� ����� ����� �����.
			Matrix<double> filterResults(resultsSize, resultsSize);
			Long m = 0;
			//1.5.3. �������� ũ�⸸ŭ �ݺ��Ѵ�.
			Long k = 0;
			while (k < filterResults.GetRow()) {
				//1.5.3.1. �������� ũ�⸸ŭ �ݺ��Ѵ�.
				Long l = 0;
				while (l < filterResults.GetColumn()) {
					//1.5.3.1.1. ���ͺ� ����� �����´�.
					filterResults.Store(m++, dynamic_cast<ConvolutionFilter*>(filter)->GetResult(k, l));
					l++;	
				}
				k++;
			}
			//1.5.4. ����ť�꺰 ����� ���ϴ�.
			results.Plus(filterResults);
			j++;
		}
		//1.6. ���� ����ť���� ��� ����� �ȼ����� ���ϴ�. (1 - ��) * 255
		//1.7. ��� ����� �� ����ŭ �ݺ��Ѵ�.
		j = 0;
		while (j < results.GetRow()) {
			//����� �簢���� ��ǥ = �ʱ���ǥ.left, �ʱ���ǥ.top + (�� �ݺ�Ƚ�� * 5), �ʱ���ǥ.right, �ʱ���ǥ.bottom + (�� �ݺ�Ƚ�� *5)
			RECT resultsRect = { rect.left + (n * 10), rect.top + (j * 10), rect.right + (n * 10), rect.bottom + (j * 10) };
			//1.7.1. ��� ����� �� ����ŭ �ݺ��Ѵ�.
			Long k = 0;
			while (k < results.GetColumn()) {
				double rgb = results.GetAt(j, k);
				COLORREF color;
				if (rgb >= 0.0) {
					rgb = rgb * 100;
					color = RGB(255 - (int)rgb, 255, 255);
				}
				else {
					rgb = -rgb * 100;
					color = RGB(255, 255 - (int)rgb, 255);
				}
				//1.7.1.1. Layout�� ������ ����� �ȼ����� �簢���� �߰��ϴ�.
				Graph *rectangle
					= new Cell(CRect(resultsRect.left + (k * 10), resultsRect.top, resultsRect.right + (k * 10), resultsRect.bottom),
						"", color);
				convolutionLayout->Add(rectangle);
				k++;
			}
			j++;
		}
		i++;
	}

	return convolutionLayout;
}

Graph* GraphMaker::Make(Pooling *pooling, Long x, Long y) {
	GraphFactory graphFactory;
	Graph *poolingLayout = graphFactory.Make(GRAPH_LAYOUT);

	RECT rect = { x, y, x + 10, y + 10 };

	Long n = 0;
	Long i = 0;
	while (i < pooling->GetLength()) {
		Neuron *filter = pooling->GetAt(i);
		//1.1. �������� ũ�⸦ ���Ѵ�.
		Long resultsSize = dynamic_cast<PoolingFilter*>(filter)->GetResultSize();

		if (i != 0 && (i % 4 == 0)) { //ȭ�鿡 ���η� 4���� ����� ���� �� ���� '����'�ؼ� ����ϱ�
			n = 0; //rect.left�� x�� �����
			y += resultsSize * 10 + 5; //�����ϱ�
		}
		//����(ť��)�� ����ȼ� �ʱ� ��ǥ = x, y+(�ݺ�Ƚ��*5*���ũ��), x+5, y+5
		RECT rect = { x + (n * 10 * resultsSize) + 20, y, x + (n * 10 * resultsSize) + 20 + 10, y + 10 };
		n++;
		//1.2. �������� �����.
		Matrix<double> results(resultsSize, resultsSize);
		Long j = 0;
		while (j < results.GetRow()) {
			Long k = 0;
			while (k < results.GetColumn()) {
				results.Store(j, k, dynamic_cast<PoolingFilter*>(filter)->GetResult(j, k));
				k++;
			}
			j++;
		}
		j = 0;
		while (j < results.GetRow()) {
			//����� �簢���� ��ǥ = �ʱ���ǥ.left, �ʱ���ǥ.top + (�� �ݺ�Ƚ�� * 5), �ʱ���ǥ.right, �ʱ���ǥ.bottom + (�� �ݺ�Ƚ�� *5)
			RECT resultsRect = { rect.left + (n * 10), rect.top + (j * 10), rect.right + (n * 10), rect.bottom + (j * 10) };
			//1.7.1. ��� ����� �� ����ŭ �ݺ��Ѵ�.
			Long k = 0;
			while (k < results.GetColumn()) {
				double rgb = results.GetAt(j, k);
				COLORREF color;
				if (rgb >= 0.0) {
					rgb = rgb * 100;
					color = RGB(255 - (int)rgb, 255, 255);
				}
				else {
					rgb = -rgb * 100;
					color = RGB(255, 255 - (int)rgb, 255);
				}
				//1.7.1.1. Layout�� ������ ����� �ȼ����� �簢���� �߰��ϴ�.
				Graph *rectangle
					= new Cell(CRect(resultsRect.left + (k * 10), resultsRect.top, resultsRect.right + (k * 10), resultsRect.bottom),
						"", color);
				poolingLayout->Add(rectangle);
				k++;
			}
			j++;
		}
		i++;
	}

	return poolingLayout;
}

Graph* GraphMaker::Make(SpatialTransformer *spatialTransformer, Long x, Long y) {
	GraphFactory graphFactory;
	Graph *spatialTransformerLayout = graphFactory.Make(GRAPH_LAYOUT);

	Graph *text = new HorizontalText(x, y-30, "Input", RGB(0, 0, 0));
	spatialTransformerLayout->Add(text);

	RECT rect = { x, y, x + 10, y + 10 };

	//1.1. �Է�(���)�� ũ�⸦ ���Ѵ�.
	Long inputSize = sqrt(spatialTransformer->GetInputLength());

	//1.2. �Է������ �����.
	Matrix<double> inputs(inputSize, inputSize);
	Long k = 0;
	Long i = 0;
	while (i < inputs.GetRow()) {
		Long j = 0;
		while (j < inputs.GetColumn()) {
			inputs.Store(i, j, spatialTransformer->GetInput(k++));
			j++;
		}
		i++;
	}
	i = 0;
	while (i < inputs.GetRow()) {
		//����� �簢���� ��ǥ = �ʱ���ǥ.left, �ʱ���ǥ.top + (�� �ݺ�Ƚ�� * 5), �ʱ���ǥ.right, �ʱ���ǥ.bottom + (�� �ݺ�Ƚ�� *5)
		RECT inputsRect = { rect.left, rect.top + (i * 10), rect.right, rect.bottom + (i * 10) };
		//1.7.1. ��� ����� �� ����ŭ �ݺ��Ѵ�.
		Long j = 0;
		while (j < inputs.GetColumn()) {
			double rgb = inputs.GetAt(i, j);
			COLORREF color;
			if (rgb >= 0.0) {
				rgb = rgb * 100;
				color = RGB(255 - (int)rgb, 255, 255);
			}
			else {
				rgb = -rgb * 100;
				color = RGB(255, 255 - (int)rgb, 255);
			}
			//1.7.1.1. Layout�� ������ ����� �ȼ����� �簢���� �߰��ϴ�.
			Graph *cell
				= new Cell(CRect(inputsRect.left + (j * 10), inputsRect.top, inputsRect.right + (j * 10), inputsRect.bottom),
					"", color);
			spatialTransformerLayout->Add(cell);
			j++;
		}
		i++;
	}

	rect.left += 300;
	rect.right += 300;

	text = new HorizontalText(rect.left, y-30, "Output", RGB(0, 0, 0));
	spatialTransformerLayout->Add(text);

	//1.1. �Է�(���)�� ũ�⸦ ���Ѵ�.
	Long resultSize = sqrt(spatialTransformer->GetResultLength());

	//1.2. �Է������ �����.
	Matrix<double> results(resultSize, resultSize);
	k = 0;
	i = 0;
	while (i < results.GetRow()) {
		Long j = 0;
		while (j < results.GetColumn()) {
			results.Store(i, j, spatialTransformer->GetResult(k++));
			j++;
		}
		i++;
	}
	i = 0;
	while (i < results.GetRow()) {
		//����� �簢���� ��ǥ = �ʱ���ǥ.left, �ʱ���ǥ.top + (�� �ݺ�Ƚ�� * 5), �ʱ���ǥ.right, �ʱ���ǥ.bottom + (�� �ݺ�Ƚ�� *5)
		RECT resultsRect = { rect.left, rect.top + (i * 10), rect.right, rect.bottom + (i * 10) };
		//1.7.1. ��� ����� �� ����ŭ �ݺ��Ѵ�.
		Long j = 0;
		while (j < results.GetColumn()) {
			double rgb = results.GetAt(i, j);
			COLORREF color;
			if (rgb >= 0.0) {
				rgb = rgb * 100;
				color = RGB(255 - (int)rgb, 255, 255);
			}
			else {
				rgb = -rgb * 100;
				color = RGB(255, 255 - (int)rgb, 255);
			}
			//1.7.1.1. Layout�� ������ ����� �ȼ����� �簢���� �߰��ϴ�.
			Graph *cell
				= new Cell(CRect(resultsRect.left + (j * 10), resultsRect.top, resultsRect.right + (j * 10), resultsRect.bottom),
					"", color);
			spatialTransformerLayout->Add(cell);
			j++;
		}
		i++;
	}

	return spatialTransformerLayout;
}

Graph* GraphMaker::Make(Affine *affine, RECT rect) {
	GraphFactory graphFactory;

	Graph *layerToolTip = graphFactory.Make(GRAPH_LAYERTOOLTIP);

	COLORREF color = RGB(255, 255, 255);
	Graph *rectangleGraph = graphFactory.Make(GRAPH_RECTANGLE);
	*dynamic_cast<RectangleGraph*>(rectangleGraph) = RectangleGraph(rect, color);
	layerToolTip->Add(rectangleGraph);

	Graph *text = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long nodeCount = affine->GetLength();
	string content = "��� ���� : ";
	content.append(to_string(nodeCount));

	color = RGB(0, 0, 0);
	Long x = rect.left + 5;
	Long y = rect.top + 5;
	*dynamic_cast<HorizontalText*>(text) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text);

	return layerToolTip;
}

Graph* GraphMaker::Make(ReLU *relu, RECT rect) {
	GraphFactory graphFactory;

	Graph *layerToolTip = graphFactory.Make(GRAPH_LAYERTOOLTIP);

	COLORREF color = RGB(255, 255, 255);
	Graph *rectangleGraph = graphFactory.Make(GRAPH_RECTANGLE);
	*dynamic_cast<RectangleGraph*>(rectangleGraph) = RectangleGraph(rect, color);
	layerToolTip->Add(rectangleGraph);

	Graph *text = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long nodeCount = relu->GetLength();
	string content = "��� ���� : ";
	content.append(to_string(nodeCount));

	color = RGB(0, 0, 0);
	Long x = rect.left + 5;
	Long y = rect.top + 5;
	*dynamic_cast<HorizontalText*>(text) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text);

	return layerToolTip;
}

Graph* GraphMaker::Make(Softmax *softmax, RECT rect) {
	GraphFactory graphFactory;

	Graph *layerToolTip = graphFactory.Make(GRAPH_LAYERTOOLTIP);

	COLORREF color = RGB(255, 255, 255);
	Graph *rectangleGraph = graphFactory.Make(GRAPH_RECTANGLE);
	*dynamic_cast<RectangleGraph*>(rectangleGraph) = RectangleGraph(rect, color);
	layerToolTip->Add(rectangleGraph);

	Graph *text = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long nodeCount = softmax->GetLength();
	string content = "��� ���� : ";
	content.append(to_string(nodeCount));

	color = RGB(0, 0, 0);
	Long x = rect.left + 5;
	Long y = rect.top + 5;
	*dynamic_cast<HorizontalText*>(text) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text);

	return layerToolTip;
}

Graph* GraphMaker::Make(Dropout *dropout, RECT rect) {
	GraphFactory graphFactory;

	Graph *layerToolTip = graphFactory.Make(GRAPH_LAYERTOOLTIP);

	COLORREF color = RGB(255, 255, 255);
	Graph *rectangleGraph = graphFactory.Make(GRAPH_RECTANGLE);
	*dynamic_cast<RectangleGraph*>(rectangleGraph) = RectangleGraph(rect, color);
	layerToolTip->Add(rectangleGraph);

	Graph *text = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long nodeCount = dropout->GetLength();
	string content = "��� ���� : ";
	content.append(to_string(nodeCount));

	color = RGB(0, 0, 0);
	Long x = rect.left + 5;
	Long y = rect.top + 5;
	*dynamic_cast<HorizontalText*>(text) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text);

	Graph *text2 = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	double ratio = dropout->GetRatio();
	content = "���� ���� : ";
	content.append(to_string(ratio));
	content.erase(5, 4);
	y = y + 15;
	*dynamic_cast<HorizontalText*>(text2) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text2);

	return layerToolTip;
}

Graph* GraphMaker::Make(Convolution *convolution, RECT rect) {
	GraphFactory graphFactory;

	Graph *layerToolTip = graphFactory.Make(GRAPH_LAYERTOOLTIP);

	COLORREF color = RGB(255, 255, 255);
	Graph *rectangleGraph = graphFactory.Make(GRAPH_RECTANGLE);
	*dynamic_cast<RectangleGraph*>(rectangleGraph) = RectangleGraph(rect, color);
	layerToolTip->Add(rectangleGraph);

	Graph *text = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long filterCount = convolution->GetLength();
	string content = "���� ���� : ";
	content.append(to_string(filterCount));

	color = RGB(0, 0, 0);
	Long x = rect.left + 5;
	Long y = rect.top + 5;
	*dynamic_cast<HorizontalText*>(text) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text);

	Graph *text2 = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long filterSize = convolution->GetFilterSize();
	content = "���� ũ�� : ";
	content.append(to_string(filterSize));
	y = y + 15;
	*dynamic_cast<HorizontalText*>(text2) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text2);

	Graph *text3 = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long stride = convolution->GetStride();
	content = "��Ʈ���̵� : ";
	content.append(to_string(stride));
	y = y + 15;
	*dynamic_cast<HorizontalText*>(text3) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text3);

	Graph *text4 = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long pad = convolution->GetPad();
	content = "�е� : ";
	content.append(to_string(pad));
	y = y + 15;
	*dynamic_cast<HorizontalText*>(text4) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text4);

	return layerToolTip;
}

Graph* GraphMaker::Make(Pooling *pooling, RECT rect) {
	GraphFactory graphFactory;

	Graph *layerToolTip = graphFactory.Make(GRAPH_LAYERTOOLTIP);

	COLORREF color = RGB(255, 255, 255);
	Graph *rectangleGraph = graphFactory.Make(GRAPH_RECTANGLE);
	*dynamic_cast<RectangleGraph*>(rectangleGraph) = RectangleGraph(rect, color);
	layerToolTip->Add(rectangleGraph);

	Graph *text = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long filterCount = pooling->GetLength();
	string content = "���� ���� : ";
	content.append(to_string(filterCount));

	color = RGB(0, 0, 0);
	Long x = rect.left + 5;
	Long y = rect.top + 5;
	*dynamic_cast<HorizontalText*>(text) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text);

	Graph *text2 = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long filterSize = pooling->GetSize();
	content = "���� ũ�� : ";
	content.append(to_string(filterSize));
	y = y + 15;
	*dynamic_cast<HorizontalText*>(text2) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text2);

	return layerToolTip;
}

Graph* GraphMaker::Make(Sigmoid *sigmoid, RECT rect) {
	GraphFactory graphFactory;

	Graph *layerToolTip = graphFactory.Make(GRAPH_LAYERTOOLTIP);

	COLORREF color = RGB(255, 255, 255);
	Graph *rectangleGraph = graphFactory.Make(GRAPH_RECTANGLE);
	*dynamic_cast<RectangleGraph*>(rectangleGraph) = RectangleGraph(rect, color);
	layerToolTip->Add(rectangleGraph);

	Graph *text = graphFactory.Make(GRAPH_HORIZONTALTEXT);
	Long nodeCount = sigmoid->GetLength();
	string content = "��� ���� : ";
	content.append(to_string(nodeCount));

	color = RGB(0, 0, 0);
	Long x = rect.left + 5;
	Long y = rect.top + 5;
	*dynamic_cast<HorizontalText*>(text) = HorizontalText(x, y, content, color);
	layerToolTip->Add(text);

	return layerToolTip;
}


GraphMaker& GraphMaker::operator =(const GraphMaker& source) {
	return *this;
}

int CompareLong(void *one, void *other) {
	Long *one_ = static_cast<Long*>(one);
	Long *other_ = static_cast<Long*>(other);
	int ret;

	if (*one_ < *other_) {
		ret = -1;
	}
	else if (*one_ == *other_) {
		ret = 0;
	}
	else if (*one_ > *other_) {
		ret = 1;
	}
	return ret;
}