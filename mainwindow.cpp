#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_conv_clicked()
{
	QString list;

	QFileDialog dialog(this);
	QString fName = dialog.getOpenFileName(this, "開く", "", "*.csv");

	if (!fName.isNull()) {
		QFile file(fName);
		file.open(QIODevice::ReadOnly);
		QTextStream stream(&file);
		stream.setCodec("shift_JIS");
		list = stream.readAll();
		file.close();
	}

	if (list.isNull()) {
		return;
	}

	QStringList line = list.split("\r\n");
	int maxCount = line.count();

	QString bgnDate;
	QString endDate;

	for (int n = 0; n < maxCount; ++n) {
		QString text = line.at(n);

		int top = text.indexOf("のトリップデータ");
		int btm = text.indexOf("【NissanConnect　マイカーアプリ】");

		if (top >= 0) {
			bgnDate = text.left(top);
		}
		else if (btm >= 0) {
			bgnDate.clear();
		}
		else if (!bgnDate.isNull()) {
		}
	}
}
