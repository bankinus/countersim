#pragma once
#include <QObject>
#include <QApplication>

class Simulator_app: public QApplication {
Q_OBJECT
public:
	Simulator_app(int argc, char **argv) : QApplication(argc, argv) {};
	virtual ~Simulator_app() {};
public slots:
	void add_register();
	void open();
	void save();
	void step();
	void run();
	void stop();
};

