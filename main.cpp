#pragma once
#pragma execution_character_set("UTF-8")

#include "mini_search.h"
#include "test.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QTextCodec>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForLocale(codec);

	QPixmap pixmap("images\\logo.png");
	QSplashScreen screen(pixmap);
	screen.show();
#if 0  
	int delayTime = 5;
	QElapsedTimer timer;
	timer.start();
	while (timer.elapsed() < (delayTime * 1000))
	{
		app.processEvents();
	}
#endif  

#if 0  
	QDateTime n = QDateTime::currentDateTime();
	QDateTime now;
	do{
		now = QDateTime::currentDateTime();
		app.processEvents();
	} while (n.secsTo(now) <= 5);//5为需要延时的秒数  
#endif  
   
	mini_search* w = new mini_search;
	w->show();
	screen.finish(w);
	return a.exec();
}
