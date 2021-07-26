#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#pragma warning(disable:4996)
#endif

#include <QMainWindow>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_pushButton_conv_clicked();
	void on_pushButton_save_clicked();

private:
	int makeData(QStringList &line, int index);
	void saveData(QTextStream &stream);

private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
