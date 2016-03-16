#include <QApplication>
#include <QBoxLayout>
#include <QFileDialog>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QPalette>
#include <QMenuBar>
#include <QPalette>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "execution_visitor.h"
#include "parser.h"
#include "register.h"
#include "simulator_app.h"
#include "simulation.h"
#include "window.h"


static bool is_running;
static QTextEdit *editor;
static QLineEdit *step_field;
QLabel *console;
static Simulator_app *app;
static Simulation simulation;
static Context *context;
static Execution_visitor *exe;
static QFileDialog *open_dialog;
static QFileDialog *save_dialog;
static QVBoxLayout *registerDisplay;

class RegisterDisplayLine {
	protected:
		QFrame line;
		QHBoxLayout lineLayout;
		size_t regNum;
		QLabel regNumLabel;
		QLineEdit edit;
	public:
		RegisterDisplayLine(size_t num) : regNum(num){
			regNumLabel.setText(QString::number(num));
			edit.setText(QString::number(0));
			registerDisplay->addWidget(&line);
			line.setLayout(&lineLayout);
			lineLayout.addWidget(&regNumLabel);
			lineLayout.addWidget(&edit);
		}
		~RegisterDisplayLine() {
			registerDisplay->removeWidget(&line);
		}
		void display(unsigned long long int val) {
			edit.setText(QString::number(val));
		}
		void lock() {
			edit.setReadOnly(true);
			unsigned long long int val = QLocale().toULongLong(edit.text());
			simulation.get_register(regNum)->set_value(val);
		}
		void unlock() {
			edit.setReadOnly(false);
		}
};

static std::vector<RegisterDisplayLine*> registerDisplayLines;

static void updateRegisters() {
	size_t i=0;
	for (; i<registerDisplayLines.size();i++) {
		registerDisplayLines[i]->display(simulation.get_register(i)->get_value());
	}
	for (; i<simulation.get_registers().size();i++) {
		RegisterDisplayLine *display = new RegisterDisplayLine(i);
		registerDisplayLines.push_back(display);
		display->display(simulation.get_register(i)->get_value());
	}
}

void Simulator_app::add_register() {
	if (is_running) return;
	if (simulation.get_registers().size()<simulation.get_max_reg() || simulation.get_max_reg()==0) {
		RegisterDisplayLine *display = new RegisterDisplayLine(simulation.get_registers().size());
		registerDisplayLines.push_back(display);
		display->display(simulation.get_register(simulation.get_registers().size())->get_value());
	}
}

void Simulator_app::open() {
	if (is_running) return;
	QStringList fileNames;
	if (open_dialog->exec()) {
	    fileNames = open_dialog->selectedFiles();
	}
	if (fileNames.size()!=1) {
		//TODO error
	}
	else {
		const char *fileName = fileNames.at(0).toUtf8().constData();
		//load program
		std::ifstream filestream(fileName);
		if (!filestream.is_open()) {
			//TODO error to right console
			std::cerr << "error: could not open file:" << fileName << "\n";
			return; 
		}
		std::stringstream bufstream;
		bufstream << filestream.rdbuf();
		editor->setText(bufstream.str().c_str());
	}
}

void Simulator_app::save() {
	QStringList fileNames;
	if (save_dialog->exec()) {
	    fileNames = save_dialog->selectedFiles();
	}
	if (fileNames.size()!=1) {
		//TODO error
	}
	else {
		const char *fileName = fileNames.at(0).toUtf8().constData();
		//write program
		std::ofstream filestream(fileName);
		if (!filestream.is_open()) {
			//TODO error to right console
			std::cerr << "error: could not open file:" << fileName << "\n";
			return;
		}
		filestream << editor->toPlainText().toUtf8().constData();
	}
}

void Simulator_app::step() {
	if (!is_running){
		/*start simulation*/
		editor->setReadOnly(true);
		for (RegisterDisplayLine *line : registerDisplayLines) {
			line->lock();
		}
		exe = new Execution_visitor(simulation);
		context = Parser().parse_simulator_program(editor->toPlainText().toUtf8().constData());
		is_running=true;
	}
	unsigned long long int count = 0;
	qulonglong steps = QLocale().toULongLong(step_field->text());
	do {
		exe->step_visitc(*context);
		count++;
	} while (count < steps);
	updateRegisters();
	if (exe->get_next()==0) {
		for (RegisterDisplayLine *line : registerDisplayLines) {
			line->unlock();
		}
		editor->setReadOnly(false);
		is_running=false;
		delete exe;
		delete context;
	}
}

void Simulator_app::run() {
	if (!is_running){
		/*start simulation*/
		editor->setReadOnly(true);
		for (RegisterDisplayLine *line : registerDisplayLines) {
			line->lock();
		}
		exe = new Execution_visitor(simulation);
		context = Parser().parse_simulator_program(editor->toPlainText().toUtf8().constData());
		is_running=true;
	}
	exe->visitc(*context);
	updateRegisters();
	if (exe->get_next()==0) {
		for (RegisterDisplayLine *line : registerDisplayLines) {
			line->unlock();
		}
		editor->setReadOnly(false);
		is_running=false;
		delete exe;
		delete context;
	}
}

void Simulator_app::stop() {
	if (is_running){
		/*stop simulation*/
		for (RegisterDisplayLine *line : registerDisplayLines) {
			line->unlock();
		}
		editor->setReadOnly(false);
		delete exe;
		delete context;
		is_running=false;
	}
}

int graphical_execution (int argc, char **argv, boost::program_options::variables_map v_map) {
	is_running = false;
	app = new Simulator_app(argc, argv);
	QScrollArea *window = new QScrollArea();
	QVBoxLayout *window_layout = new QVBoxLayout();
	window->setLayout(window_layout);

	/*Set up FileDialogs*/
	open_dialog = new QFileDialog(NULL, QString("Open File"), QDir::currentPath(), QString("All files (*)"));
	save_dialog = new QFileDialog(NULL, QString("Save File"), QDir::currentPath(), QString("All files (*)"));
	save_dialog->setLabelText( QFileDialog::Accept, "&Save" );

	/*create objects for global pointers*/
	editor = new QTextEdit();
	console = new QLabel();

	/*set up menu bar*/
	/*file*/
	QMenuBar *menubar = new QMenuBar(window);
	QMenu *file_menu = menubar->addMenu("&File");
	QAction *open_action = file_menu->addAction("&open..");
	QAction *save_action = file_menu->addAction("&save..");
	QAction *close_action = file_menu->addAction("&close");
	/*edit*/
	QMenu *edit_menu = menubar->addMenu("&Edit");
	QAction *undo_action = edit_menu->addAction("&undo");
	QAction *redo_action = edit_menu->addAction("&redo");
	QAction *clear_action = edit_menu->addAction("&clear");
	/*simulator*/
	QMenu *simulator_menu = menubar->addMenu("&Simulator");
	QAction *step_action = simulator_menu->addAction("&step");
	QAction *run_action = simulator_menu->addAction("&run");
	QAction *stop_action = simulator_menu->addAction("&stop");
	/*help*/
	QMenu *help_menu = menubar->addMenu("&Help");
	help_menu->addAction("&about");
	window_layout->setMenuBar(menubar);

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
	QFrame *body = new QFrame();
	QHBoxLayout *body_layout = new QHBoxLayout();
	body->setLayout(body_layout);
	window_layout->addWidget(body);

	/*text boxes*/
	QFrame *text_frame = new QFrame();
	QVBoxLayout *text_layout = new QVBoxLayout();
	body_layout->addWidget(text_frame, 3);
	text_frame->setLayout(text_layout);
	text_layout->addWidget(editor, 3);
	QScrollArea *console_frame = new QScrollArea();
	console->setParent(console_frame);
	text_layout->addWidget(console_frame, 1);
	editor->setStyleSheet(QString("QTextEdit[readOnly=\"true\"] { background-color: lightGray }"));

	/*simulator interface*/
	QFrame *simulator_interface = new QFrame();
	QVBoxLayout *simulator_layout = new QVBoxLayout();
	simulator_interface->setLayout(simulator_layout);
	body_layout->addWidget(simulator_interface, 1);

	/*set up action bar*/
	QFrame *actionbar = new QFrame();
	QHBoxLayout *actionbar_layout = new QHBoxLayout();
	actionbar->setLayout(actionbar_layout);
	step_field = new QLineEdit();
	step_field->setText(QString::number(1));
	actionbar_layout->addWidget(step_field);
	QPushButton *step_button = new QPushButton("step");
	actionbar_layout->addWidget(step_button);
	QPushButton *run_button = new QPushButton("run");
	actionbar_layout->addWidget(run_button);
	QPushButton *stop_button = new QPushButton("stop");
	actionbar_layout->addWidget(stop_button);
	simulator_layout->addWidget(actionbar);

	/*set up register frame*/
	QScrollArea *register_frame = new QScrollArea();
	registerDisplay = new QVBoxLayout();
	registerDisplay->setAlignment(Qt::AlignTop);
	register_frame->setLayout(registerDisplay);
	simulator_layout->addWidget(register_frame);
	QPushButton *add_register_button = new QPushButton("add register");
	registerDisplay->addWidget(add_register_button);

	/*connect button signals to slots*/
	QObject::connect(step_button, SIGNAL (clicked()), app, SLOT (step()));
	QObject::connect(run_button, SIGNAL (clicked()), app, SLOT (run()));
	QObject::connect(stop_button, SIGNAL (clicked()), app, SLOT (stop()));
	QObject::connect(add_register_button, SIGNAL (clicked()), app, SLOT (add_register()));

	/*check for program file to load*/
	if(v_map.count("input-file")) {
		std::stringstream bufstream;
		std::ifstream filestream(v_map["input-file"].as<std::string>().c_str());
		bufstream << filestream.rdbuf();
		editor->setText(bufstream.str().c_str());
	}
	/*check for register file to load*/
	if (v_map.count("register-file")) {
		std::ifstream reg_in_stream(v_map["register-file"].as<std::string>());
		if (!reg_in_stream.is_open()) {
			//TODO proper error stream
			std::cerr << "error: could not open file:" << v_map["register-file"].as<std::string>() << "\n";
			return 0;
		}
		for (size_t i=0; !reg_in_stream.eof(); i++){
			unsigned long long int val;
			reg_in_stream >> val;
			if (reg_in_stream.eof()) break;
			simulation.get_register(i)->set_value(val);
		}
		updateRegisters();
	}

	window->show();
	return app->exec();
}

