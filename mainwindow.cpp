#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
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

/*
00	2021/07/11 19:14:37のトリップデータ
01	-------------------------
02	　滋賀県大津市大江６丁目
03	　＃緯度34.97299154
04	　＃経度135.92465115
05	　↓串八
06	　滋賀県草津市大路３丁目
07	　＃緯度35.01802572
08	　＃経度135.96906521
09	-------------------------
10	▽出発時間
11	　2021/07/11 19:14:37
12	▽到着時間
13	　2021/07/11 19:32:09
14	▽走行データ
15	　・走行距離：　7.9km
16	　・運転時間：　17分32秒
17	　・平均速度：　27.0km/h
18	　・平均燃費：　25.6km/L
19	【NissanConnect　マイカーアプリ】
 */

int MainWindow::makeData(QStringList &line, int index)
{
	QString text;
	QString data;
	int find;

	int row = ui->tableWidget->rowCount();
	ui->tableWidget->setRowCount(row + 1);

	// 出発時間
	text = line.at(index + 11);
	data = text.mid(1, 10);
	ui->tableWidget->setItem(row, 0, new QTableWidgetItem(data));
	data = text.right(8);
	ui->tableWidget->setItem(row, 1, new QTableWidgetItem(data));

	// 到着時間
	text = line.at(index + 13);
	data = text.mid(1, 10);
	ui->tableWidget->setItem(row, 5, new QTableWidgetItem(data));
	data = text.right(8);
	ui->tableWidget->setItem(row, 6, new QTableWidgetItem(data));

	// 走行距離
	text = line.at(index + 15);
	data = text.mid(8);
	ui->tableWidget->setItem(row, 7, new QTableWidgetItem(data));

	// 距離-3
	find = data.indexOf("km");
	if (find >= 0) {
		data = data.mid(0, find);
	}
	ui->tableWidget->setItem(row, 3, new QTableWidgetItem(data));

	// 運転時間
	text = line.at(index + 16);
	data = text.mid(8);
	ui->tableWidget->setItem(row, 8, new QTableWidgetItem(data));

	// 時間-4
	int time = data.mid(0, 2).toInt() * 60 + data.mid(3, 2).toInt();
	data = QString::number(time);
	ui->tableWidget->setItem(row, 4, new QTableWidgetItem(data));

	// 平均速度
	text = line.at(index + 17);
	data = text.mid(8);
	ui->tableWidget->setItem(row, 9, new QTableWidgetItem(data));

	// 平均燃費
	text = line.at(index + 18);
	data = text.mid(8);
	ui->tableWidget->setItem(row, 10, new QTableWidgetItem(data));

	// 燃費-2
	find = data.indexOf("km/L");
	if (find >= 0) {
		data = data.mid(0, find);
	}
	ui->tableWidget->setItem(row, 2, new QTableWidgetItem(data));

	// 備考
	text = line.at(index + 5);
	data = text.mid(2);
	ui->tableWidget->setItem(row, 11, new QTableWidgetItem(data));

	// 出発場所
	text = line.at(index + 2);
	data = text.mid(1);
	ui->tableWidget->setItem(row, 12, new QTableWidgetItem(data));

	// 出発座標
	text = line.at(index + 3);
	data = text.mid(4);
	text = line.at(index + 4);
	data = "\"" + data + "," + text.mid(4) + "\"";
	ui->tableWidget->setItem(row, 13, new QTableWidgetItem(data));

	// 到着場所
	text = line.at(index + 6);
	data = text.mid(1);
	ui->tableWidget->setItem(row, 14, new QTableWidgetItem(data));

	// 到着座標
	text = line.at(index + 7);
	data = text.mid(4);
	text = line.at(index + 8);
	data = "\"" + data + "," + text.mid(4) + "\"";
	ui->tableWidget->setItem(row, 15, new QTableWidgetItem(data));

	return index + 20;
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

	if (!list.isNull()) {
		ui->tableWidget->setColumnCount(16);

		ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("出発日付"));
		ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("出発時刻"));

		ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("燃費"));
		ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("距離"));
		ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("時間"));

		ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("到着日付"));
		ui->tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("到着時刻"));
		ui->tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("走行距離"));
		ui->tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("運転時間"));
		ui->tableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("平均速度"));
		ui->tableWidget->setHorizontalHeaderItem(10, new QTableWidgetItem("平均燃費"));
		ui->tableWidget->setHorizontalHeaderItem(11, new QTableWidgetItem("備考"));
		ui->tableWidget->setHorizontalHeaderItem(12, new QTableWidgetItem("出発場所"));
		ui->tableWidget->setHorizontalHeaderItem(13, new QTableWidgetItem("出発座標"));
		ui->tableWidget->setHorizontalHeaderItem(14, new QTableWidgetItem("到着場所"));
		ui->tableWidget->setHorizontalHeaderItem(15, new QTableWidgetItem("到着座標"));

		QStringList line = list.split("\r\n");
		int count = line.count();
		int index = 0;

		while (index < count) {
			QString text = line.at(index);

			if (text.indexOf("トリップデータ") >= 0) {
				index = makeData(line, index);
			}
			else {
				index++;
			}
		}

		for (int n = 0; n < ui->tableWidget->columnCount(); ++n) {
			ui->tableWidget->resizeColumnToContents(n);
		}
	}
}

void MainWindow::saveData(QTextStream &stream)
{
	QString text;
	QString coma;

	int rowCount = ui->tableWidget->rowCount();

	for (int n = 0; n < ui->tableWidget->columnCount(); ++n) {
		text += coma + ui->tableWidget->horizontalHeaderItem(n)->data(Qt::DisplayRole).toString();
		coma = ",";
	}
	stream << text << "\n";

	for (int row = 0; row < rowCount; ++row) {
		text.clear();
		coma.clear();

		for (int n = 0; n < ui->tableWidget->columnCount(); ++n) {
			text += coma + ui->tableWidget->item(row, n)->data(Qt::DisplayRole).toString();
			coma = ",";
		}
		stream << text << "\n";
	}
}

void MainWindow::on_pushButton_save_clicked()
{
	QString list;

	QFileDialog dialog(this);
	QString fName = dialog.getSaveFileName(this, "保存", "", "*.csv");

	if (!fName.isNull()) {
		QFile file(fName);
		file.open(QIODevice::ReadWrite);
		QTextStream stream(&file);
		stream.setCodec("shift_JIS");
		saveData(stream);
		file.close();
	}
}
