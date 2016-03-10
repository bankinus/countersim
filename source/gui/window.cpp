#include <QApplication>
#include <QBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include "window.h"

int graphical_execution (int argc, char **argv, boost::program_options::variables_map v_map) {
	QApplication app(argc, argv);
	QScrollArea window;
	QVBoxLayout window_layout;
	window.setLayout(&window_layout);

	/*set up menu bar*/
	QMenuBar menubar(&window);
	QMenu *file_menu = menubar.addMenu("File");
	file_menu->addAction("open..");
	file_menu->addAction("save..");
	file_menu->addAction("close");
	QMenu *edit_menu = menubar.addMenu("Edit");
	edit_menu->addAction("undo");
	edit_menu->addAction("redo");
	edit_menu->addAction("clear");
	QMenu *simulator_menu = menubar.addMenu("Simulator");
	simulator_menu->addAction("step");
	simulator_menu->addAction("run");
	simulator_menu->addAction("stop");
	QMenu *help_menu = menubar.addMenu("Help");
	help_menu->addAction("about");
	window_layout.setMenuBar(&menubar);

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
	QTextEdit editor;
	text_layout.addWidget(&editor, 3);
	QScrollArea console_frame;
	text_layout.addWidget(&console_frame, 1);
	QLabel Console(&console_frame);

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

	
	window.show();
	return app.exec();
}

