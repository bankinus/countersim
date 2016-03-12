#include <QApplication>
#include <QBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QMenuBar>
#include <QPalette>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include "window.h"
#include <string>
#include <iostream>
#include <fstream>
#include "simulator_app.h"

static bool is_running;
static QTextEdit *editor;
QLabel *console;
Simulator_app *app;

void Simulator_app::open() {
	//TODO replace with real code
	std::cout << "open" << std::endl; //debug
}

void Simulator_app::save() {
	//TODO replace with real code
	std::cout << "save" << std::endl; //debug
}

void Simulator_app::step() {
	if (!is_running){
		/*start simulation*/
		editor->setReadOnly(true);
		//TODO
	}
	is_running=true;
	//TODO replace with real code
	std::cout << "step" << std::endl; //debug
}

void Simulator_app::run() {
	if (!is_running){
		/*start simulation*/
		editor->setReadOnly(true);
		//TODO 
	}
	is_running=true;
	//TODO replace with real code
	std::cout << "run" << std::endl;//debug
}

void Simulator_app::stop() {
	if (is_running){
		/*stop simulation*/
		editor->setReadOnly(false);
		//TODO
	}
	is_running=false;
	//TODO replace with real code
	std::cout << "stop" << std::endl;//debug
}

int graphical_execution (int argc, char **argv, boost::program_options::variables_map v_map) {
	is_running = false;
	app = new Simulator_app(argc, argv);
	QScrollArea window;
	QVBoxLayout window_layout;
	window.setLayout(&window_layout);

	/*create objects for global pointers*/
	editor = new QTextEdit();
	console = new QLabel();

	/*set up menu bar*/
	/*file*/
	QMenuBar menubar(&window);
	QMenu *file_menu = menubar.addMenu("&File");
	QAction *open_action = file_menu->addAction("&open..");
	QAction *save_action = file_menu->addAction("&save..");
	QAction *close_action = file_menu->addAction("&close");
	/*edit*/
	QMenu *edit_menu = menubar.addMenu("&Edit");
	QAction *undo_action = edit_menu->addAction("&undo");
	QAction *redo_action = edit_menu->addAction("&redo");
	QAction *clear_action = edit_menu->addAction("&clear");
	/*simulator*/
	QMenu *simulator_menu = menubar.addMenu("&Simulator");
	QAction *step_action = simulator_menu->addAction("&step");
	QAction *run_action = simulator_menu->addAction("&run");
	QAction *stop_action = simulator_menu->addAction("&stop");
	/*help*/
	QMenu *help_menu = menubar.addMenu("&Help");
	help_menu->addAction("&about");
	window_layout.setMenuBar(&menubar);

	/*connect action signals to slots*/
	/*file*/
	QObject::connect(open_action, SIGNAL (triggered()), app, SLOT (open()));
	QObject::connect(save_action, SIGNAL (triggered()), app, SLOT (save()));
	QObject::connect(close_action, SIGNAL (triggered()), app, SLOT (quit()));
	/*edit*/
	QObject::connect(undo_action, SIGNAL (triggered()), editor, SLOT (undo()));
	QObject::connect(redo_action, SIGNAL (triggered()), editor, SLOT (redo()));
	QObject::connect(clear_action, SIGNAL (triggered()), editor, SLOT (clear()));
	/*simulator*/
	QObject::connect(step_action, SIGNAL (triggered()), app, SLOT (step()));
	QObject::connect(run_action, SIGNAL (triggered()), app, SLOT (run()));
	QObject::connect(stop_action, SIGNAL (triggered()), app, SLOT (stop()));
	/*help*/

	/*set body frame*/
	QFrame body;
	QHBoxLayout body_layout;
	body.setLayout(&body_layout);
	window_layout.addWidget(&body);

	/*text boxes*/
	QFrame text_frame;
	QVBoxLayout text_layout;
	body_layout.addWidget(&text_frame, 3);
	text_frame.setLayout(&text_layout);
	text_layout.addWidget(editor, 3);
	QScrollArea console_frame;
	console->setParent(&console_frame);
	text_layout.addWidget(&console_frame, 1);
	editor->setStyleSheet(QString("QTextEdit[readOnly=\"true\"] { background-color: lightGray }"));

	/*simulator interface*/
	QFrame simulator_interface;
	QVBoxLayout simulator_layout;
	simulator_interface.setLayout(&simulator_layout);
	body_layout.addWidget(&simulator_interface, 1);

	/*set up action bar*/
	QFrame actionbar;
	QHBoxLayout actionbar_layout;
	actionbar.setLayout(&actionbar_layout);
	QPushButton step_button ("step");
	actionbar_layout.addWidget(&step_button);
	QPushButton run_button ("run");
	actionbar_layout.addWidget(&run_button);
	QPushButton stop_button ("stop");
	actionbar_layout.addWidget(&stop_button);
	simulator_layout.addWidget(&actionbar);
	QScrollArea register_frame;
	simulator_layout.addWidget(&register_frame);

	/*connect button signals to slots*/
	QObject::connect(&step_button, SIGNAL (clicked()), app, SLOT (step()));
	QObject::connect(&run_button, SIGNAL (clicked()), app, SLOT (run()));
	QObject::connect(&stop_button, SIGNAL (clicked()), app, SLOT (stop()));

	/*check for file to load*/
	if(v_map.count("input-file")) {
		std::stringstream bufstream;
		std::ifstream filestream(v_map["input-file"].as<std::string>().c_str());
		bufstream << filestream.rdbuf();
		editor->setText(bufstream.str().c_str());
	}

	window.show();
	return app->exec();
}

