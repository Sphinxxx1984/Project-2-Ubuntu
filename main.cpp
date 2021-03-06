#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "Heads.h"

using namespace std;

Record *Recordhead = NULL;

Tableop topt;
File fp;
Recordop ropt;

int cmd_match(string cmd) {
	std::regex pattern_create1("(CREATE)(\\s+)(TABLE)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(\\(.*\\))(\\s+)(TO)(\\s+)((?:[A-Za-z0-9_]*))(\\.)(txt)");
	std::regex pattern_create2("(CREATE)(\\s+)(TABLE)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\.)(txt)");
	std::regex pattern_drop("(DROP)(\\s+)(TABLE)(\\s+)((?:[A-Za-z0-9_]*))");
	std::regex pattern_tablelist("(TABLE)(\\s+)(LIST)");
	std::regex pattern_insert1("(INSERT)(\\s+)(INTO)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(VALUES)(\\s+)(\\(.*\\))");
	std::regex pattern_insert2("(INSERT)(\\s+)(INTO)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(\\(.*\\))(\\s+)(VALUES)(\\s+)(\\(.*\\))");
	std::regex pattern_delete1("(DELETE)(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(WHERE)(\\s+)((?:[^]*))");
	std::regex pattern_delete2("(DELETE)(\\s+)(\\*)(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))");
	std::regex pattern_update1("(UPDATE)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(SET)(\\s+)((?:[A-Za-z0-9_,=+]*))");
	std::regex pattern_update2("(UPDATE)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(SET)(\\s+)((?:[A-Za-z0-9_,=+]*))(\\s+)(WHERE)(\\s+)((?:[^]*))");
	std::regex pattern_select1("(SELECT)(\\s+)(\\*)(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(TO)(\\s+)((?:[A-Za-z0-9_]*))(\\.)(txt)");
	std::regex pattern_select2("(SELECT)(\\s+)(\\*)(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))");
	std::regex pattern_select3("(SELECT)(\\s+)(DISTINCT)(\\s+)((?:[^]*))(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(TO)(\\s+)((?:[A-Za-z0-9_]*))(\\.)(txt)");
	std::regex pattern_select4("(SELECT)(\\s+)(DISTINCT)(\\s+)((?:[^]*))(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))");
	std::regex pattern_select5("(SELECT)(\\s+)((?:[A-Za-z0-9_,=]*))(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(TO)(\\s+)((?:[A-Za-z0-9_]*))(\\.)(txt)");
	std::regex pattern_select6("(SELECT)(\\s+)((?:[A-Za-z0-9_,=]*))(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))");
	std::regex pattern_select7("(SELECT)(\\s+)(\\*)(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(ORDER)(\\s+)(BY)(\\s+)((?:[^]*))(\\s+)(ASC|DESC)(\\s+)(TO)(\\s+)((?:[A-Za-z0-9_]*))(\\.)(txt)");
	std::regex pattern_select8("(SELECT)(\\s+)(\\*)(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(ORDER)(\\s+)(BY)(\\s+)((?:[^]*))(\\s+)(ASC|DESC)");
	std::regex pattern_select9("(SELECT)(\\s+)(\\*)(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(WHERE)(\\s+)((?:[^]*))(\\s+)(TO)(\\s+)((?:[A-Z][a-z0-9_]*))(\\.)(txt)");
	std::regex pattern_select10("(SELECT)(\\s+)(\\*)(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(WHERE)(\\s+)((?:[^]*))");
	std::regex pattern_select11("(SELECT)(\\s+)((?:[^]*))(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(WHERE)(\\s+)((?:[^]*))(\\s+)(TO)(\\s+)((?:[A-Za-z0-9_]*))(\\.)(txt)");
	std::regex pattern_select12("(SELECT)(\\s+)((?:[^]*))(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(WHERE)(\\s+)((?:[^]*))");
	std::regex pattern_select13("(SELECT)(\\s+)(MAX|MIN|AVERAGE)(\\s+)((?:[^]*))(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\s+)(TO)(\\s+)((?:[A-Za-z0-9_]*))(\\.)(txt)");
	std::regex pattern_select14("(SELECT)(\\s+)(MAX|MIN|AVERAGE)(\\s+)((?:[^]*))(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))");
	std::regex pattern_quit("(quit)");

	int mode = 0;

	if (std::regex_match(cmd, pattern_create1))
		mode = 1;
	else if (std::regex_match(cmd, pattern_create2))
		mode = 2;
	else if (std::regex_match(cmd, pattern_drop))
		mode = 3;
	else if (std::regex_match(cmd, pattern_tablelist))
		mode = 4;
	else if (std::regex_match(cmd, pattern_insert1))
		mode = 5;
	else if (std::regex_match(cmd, pattern_insert2))
		mode = 6;
	else if (std::regex_match(cmd, pattern_delete1))
		mode = 7;
	else if (std::regex_match(cmd, pattern_delete2))
		mode = 8;
	else if (std::regex_match(cmd, pattern_update1))
		mode = 9;
	else if (std::regex_match(cmd, pattern_update2))
		mode = 10;
	else if (std::regex_match(cmd, pattern_select1))
		mode = 11;
	else if (std::regex_match(cmd, pattern_select2))
		mode = 12;
	else if (std::regex_match(cmd, pattern_select3))
		mode = 13;
	else if (std::regex_match(cmd, pattern_select4))
		mode = 14;
	else if (std::regex_match(cmd, pattern_select5))
		mode = 15;
	else if (std::regex_match(cmd, pattern_select6))
		mode = 16;
	else if (std::regex_match(cmd, pattern_select7))
		mode = 17;
	else if (std::regex_match(cmd, pattern_select8))
		mode = 18;
	else if (std::regex_match(cmd, pattern_select9))
		mode = 19;
	else if (std::regex_match(cmd, pattern_select10))
		mode = 20;
	else if (std::regex_match(cmd, pattern_select11))
		mode = 21;
	else if (std::regex_match(cmd, pattern_select12))
		mode = 22;
	else if (std::regex_match(cmd, pattern_select13))
		mode = 23;
	else if (std::regex_match(cmd, pattern_select14))
		mode = 24;
	else if (std::regex_match(cmd, pattern_quit))
		mode = -1;

	return mode;
}

bool cmd_todo(string cmd,int mode) {
	if (mode == 0) {
		cout << "Fail to match a command!Please retry!" << endl;
		mode = 0;
		return false;
	}
	bool quitflag = false;
	while (mode != 0) {
		if (mode == -1) {
			quitflag = true;
			break;
		}

		else if (mode == 1) {
			std::size_t ch1 = cmd.find("(", 0);
			std::size_t ch2 = cmd.find(")", 0);
			string title = cmd.substr(ch1 + 1, ch2 - ch1 - 1);
			//cout << title << endl;
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			char tablename[30] = { '\0' };
			char filename[30] = { '\0' };
			for (int i = 0, tablepos = cmd.find("TABLE") + 6; cmd_array[tablepos] != ' '&&i < 200; i++, tablepos++)
				tablename[i] = cmd_array[tablepos];
			//cout << tablename << endl;
			for (int j = 0, filepos = cmd.find("TO") + 3; cmd_array[filepos] != ' '&&cmd_array[filepos] != '\0'&&cmd_array[filepos] != '\r'&&j < 200; j++, filepos++)
				filename[j] = cmd_array[filepos];
			//cout << filename << endl;

			char test[40] = { '\0' };
			strcpy(test, tablename);
			strcat(test, ".txt");

			if (strcmp(test, filename) != 0) {
				mode = 0;
				cout << "Warning!Tablename doesn't match with filename!Please retry!" << endl;
				break;
			}
			
			if (strcmp(tablename, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}

			if (Recordhead != NULL) {
				for (Record *p = Recordhead; p != NULL; p = p->next) {
					char ptablename[30] = { '\0' };
					strcpy(ptablename, p->tablename.c_str());
					if (strcmp(ptablename, tablename) == 0) {
						cout << "Invalid Tablename!Please Retry!" << endl;
						mode = 0;
						break;
					}
				}
				if (mode == 0)
					break;
			}

			Table *Tablehead = new Table;
			Tablehead = NULL;
			ropt.addTable(Recordhead, tablename);
			topt.newTable(Tablehead, title);
			topt.showTable_all(Tablehead);
			fp.writeTable(Tablehead, filename);
			fp.writeRecord(Recordhead);
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 2) {
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			char tablename[30] = { '\0' };
			char filename[30] = { '\0' };
			for (int i = 0, tablepos = cmd.find("TABLE") + 6; cmd_array[tablepos] != ' '&&i < 200; i++, tablepos++)
				tablename[i] = cmd_array[tablepos];
			//cout << tablename << endl;
			for (int j = 0, filepos = cmd.find("FROM") + 5; cmd_array[filepos] != ' '&&cmd_array[filepos] != '\0'&&cmd_array[filepos] != '\r'&&j < 200; j++, filepos++)
				filename[j] = cmd_array[filepos];
			//cout << filename << endl;

			char test[40] = { '\0' };
			strcpy(test, tablename);
			strcat(test, ".txt");
			if (strcmp(test, filename) != 0) {
				mode = 0;
				cout << "Warning!Tablename doesn't match with filename!Please retry!" << endl;
				break;
			}
			if (strcmp(tablename, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}

			if (Recordhead != NULL) {
				for (Record *p = Recordhead; p != NULL; p = p->next) {
					char ptablename[40] = { '\0' };
					strcpy(ptablename, p->tablename.c_str());
					if (strcmp(ptablename, tablename) == 0) {
						cout << "Invalid Tablename!Please Retry!" << endl;
						mode = 0;
						break;
					}
				}
				if (mode == 0)
					break;
			}

			Table *Tablehead = new Table;
			Tablehead = NULL;
			ropt.addTable(Recordhead, tablename);
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_all(Tablehead);
				fp.writeRecord(Recordhead);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 3) {
			char tablename_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("TABLE") + 6; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			string tablename = tablename_array;
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			ropt.discardTable(Recordhead, tablename);
			fp.writeRecord(Recordhead);
			mode = 0;
			break;
		}

		else if (mode == 4) {
			ropt.listTable(Recordhead);
			mode = 0;
			break;
		}

		else if (mode == 5) {
			std::size_t ch1 = cmd.find("(", 0);
			std::size_t ch2 = cmd.find(")", 0);
			string content = cmd.substr(ch1 + 1, ch2 - ch1 - 1);
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			char tablename[30] = { '\0' };
			char filename[30] = { '\0' };
			for (int i = 0, tablepos = cmd.find("INTO") + 5; cmd_array[tablepos] != ' '&&i < 200; i++, tablepos++)
				tablename[i] = cmd_array[tablepos];
			strcpy(filename, tablename);
			strcat(filename, ".txt");
			if (strcmp(tablename, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.insertintoTable(Tablehead, content);
				topt.showTable_all(Tablehead);
				fp.writeTable(Tablehead, filename);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 6) {
			std::size_t ch1 = cmd.find("(", 0);
			std::size_t ch2 = cmd.find(")", 0);
			string titles = cmd.substr(ch1 + 1, ch2 - ch1 - 1);
			//cout << titles << endl;
			std::size_t ch3 = cmd.find("(", ch2 + 1);
			std::size_t ch4 = cmd.find(")", ch2 + 1);
			string content = cmd.substr(ch3 + 1, ch4 - ch3 - 1);
			//cout << content << endl;
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			char tablename[30] = { '\0' };
			char filename[30] = { '\0' };
			for (int i = 0, tablepos = cmd.find("INTO") + 5; cmd_array[tablepos] != ' '&&i < 200; i++, tablepos++)
				tablename[i] = cmd_array[tablepos];
			strcpy(filename, tablename);
			strcat(filename, ".txt");
			if (strcmp(tablename, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.insertintoTable_columns(Tablehead, titles, content);
				topt.showTable_all(Tablehead);
				fp.writeTable(Tablehead, filename);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 7) {
			char tablename_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			char limit_array[30] = { '\0' };
			char value_array[30] = { '\0' };
			strcpy(cmd_array, cmd.c_str());

			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			int type = 0;
			if (cmd.find("<=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -1;
			}
			else if (cmd.find(">=") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 1;
			}

			else if (cmd.find("!=") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 3;
			}

			else if (cmd.find("=") != string::npos) {
				for (int i = 0, valuepos = cmd.rfind("=") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 0;
			}

			else if (cmd.find("<") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -2;
			}

			else if (cmd.find(">") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 2;
			}

			for (int i = 0, limitpos = cmd.find("WHERE") + 6; cmd_array[limitpos] != ' '&&cmd_array[limitpos] != '='&&cmd_array[limitpos] != '<'&&cmd_array[limitpos] != '>'&&i < 200; i++, limitpos++)
				limit_array[i] = cmd_array[limitpos];
			string limit = limit_array;
			string value = value_array;
			//cout << limit << " " << value << endl;
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.deleteTable(Tablehead, limit, value, type);
				topt.showTable_all(Tablehead);
				fp.writeTable(Tablehead, filename);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 8) {
			char tablename_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.deleteTable_all(Tablehead);
				topt.showTable_all(Tablehead);
				fp.writeTable(Tablehead, filename);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 9) {
			char tablename_array[30] = { '\0' };
			char cmd_array[1024] = { '\0' };
			char raw_array[1024] = { '\0' };
			strcpy(cmd_array, cmd.c_str());

			for (int i = 0, tablepos = cmd.find("UPDATE") + 7; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 1024; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			for (int i = 0, rawpos = cmd.find("SET") + 4; cmd_array[rawpos] != '\0'&&cmd_array[rawpos] != '\r'&&i < 1024; i++, rawpos++)
				raw_array[i] = cmd_array[rawpos];
			stringstream raw(raw_array);
			string limit;
			string value;
			string token;
			string cell;

			while (getline(raw, token, '='))
			{
				limit += token;
				limit += ',';
				getline(raw, cell, ','); \
					value += cell;
				value += ',';
			}

			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.updateTable_all(Tablehead, limit, value);
				topt.showTable_all(Tablehead);
				fp.writeTable(Tablehead, filename);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 10) {
			char tablename_array[30] = { '\0' };
			char cmd_array[1024] = { '\0' };
			char raw_array[1024] = { '\0' };
			char limit_array[30] = { '\0' };
			char value_array[30] = { '\0' };
			strcpy(cmd_array, cmd.c_str());

			for (int i = 0, tablepos = cmd.find("UPDATE") + 7; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 1024; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			for (int i = 0, rawpos = cmd.find("SET") + 4; cmd_array[rawpos] != ' '&&i < 1024; i++, rawpos++)
				raw_array[i] = cmd_array[rawpos];

			for (int i = 0, limitpos = cmd.find("WHERE") + 6; cmd_array[limitpos] != ' '&&cmd_array[limitpos] != '='&&cmd_array[limitpos] != '<'&&cmd_array[limitpos] != '>'&&i < 1024; i++, limitpos++)
				limit_array[i] = cmd_array[limitpos];
			int type = 0;
			if (cmd.find("<=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -1;
			}
			else if (cmd.find(">=") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 1;
			}

			else if (cmd.find("!=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("!=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 3;
			}

			else if (cmd.find("=") != string::npos) {
				for (int i = 0, valuepos = cmd.rfind("=") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 0;
			}

			else if (cmd.find("<") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -2;
			}

			else if (cmd.find(">") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 2;
			}

			stringstream raw(raw_array);
			string limit;
			string value;
			string token;
			string cell;
			string add_limit = limit_array;
			string add_value = value_array;

			while (getline(raw, token, '='))
			{
				limit += token;
				limit += ',';
				getline(raw, cell, ','); \
					value += cell;
				value += ',';
			}

			//cout << add_limit << endl << add_value << endl;
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.updateTable(Tablehead, limit, value, add_limit, add_value, type);
				topt.showTable_all(Tablehead);
				fp.writeTable(Tablehead, filename);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 11) {
			char tablename_array[30] = { '\0' };
			char resultfile_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, resultpos = cmd.find("TO") + 3; cmd_array[resultpos] != ' '&&cmd_array[resultpos] != '\0'&&cmd_array[resultpos] != '\r'&&i < 200; i++, resultpos++)
				resultfile_array[i] = cmd_array[resultpos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(filename, resultfile_array) == 0) {
				cout << "Invalid file name" << endl;
				mode = 0;
				break;
			}
			if (strcmp(resultfile_array, "Record.txt") == 0) {
				mode = 0;
				cout << "INVALID RESULTFILENAME!" << endl;
				break;
			}
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_all(Tablehead);
				topt.showTable_all(Tablehead, resultfile_array);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 12) {
			char tablename_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_all(Tablehead);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 13) {
			char tablename_array[30] = { '\0' };
			char title_array[30] = { '\0' };
			char resultfile_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, titlepos = cmd.find("DISTINCT") + 9; cmd_array[titlepos] != ' '&&cmd_array[titlepos] != '\0'&&cmd_array[titlepos] != '\r'&&i < 200; i++, titlepos++)
				title_array[i] = cmd_array[titlepos];
			string limit = title_array;
			for (int i = 0, resultpos = cmd.find("TO") + 3; cmd_array[resultpos] != ' '&&cmd_array[resultpos] != '\0'&&cmd_array[resultpos] != '\r'&&i < 200; i++, resultpos++)
				resultfile_array[i] = cmd_array[resultpos];

			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(filename, resultfile_array) == 0) {
				cout << "Invalid file name" << endl;
				mode = 0;
				break;
			}
			if (strcmp(resultfile_array, "Record.txt") == 0) {
				mode = 0;
				cout << "INVALID RESULTFILENAME!" << endl;
				break;
			}
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_distinct(Tablehead, limit);
				topt.showTable_distinct(Tablehead, limit, resultfile_array);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 14) {
			char tablename_array[30] = { '\0' };
			char title_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, titlepos = cmd.find("DISTINCT") + 9; cmd_array[titlepos] != ' '&&cmd_array[titlepos] != '\0'&&cmd_array[titlepos] != '\r'&&i < 200; i++, titlepos++)
				title_array[i] = cmd_array[titlepos];

			string limit = title_array;
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_distinct(Tablehead, limit);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 15) {
			char tablename_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			char titles_array[100] = { '\0' };
			char resultfile_array[30] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, titlepos = cmd.find("SELECT") + 7; cmd_array[titlepos] != ' '&&i < 200; i++, titlepos++)
				titles_array[i] = cmd_array[titlepos];
			string titles = titles_array;
			for (int i = 0, resultpos = cmd.find("TO") + 3; cmd_array[resultpos] != ' '&&cmd_array[resultpos] != '\0'&&cmd_array[resultpos] != '\r'&&i < 200; i++, resultpos++)
				resultfile_array[i] = cmd_array[resultpos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(filename, resultfile_array) == 0) {
				cout << "Invalid file name" << endl;
				mode = 0;
				break;
			}
			if (strcmp(resultfile_array, "Record.txt") == 0) {
				mode = 0;
				cout << "INVALID RESULTFILENAME!" << endl;
				break;
			}
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_columns(Tablehead, titles);
				topt.showTable_columns(Tablehead, titles, resultfile_array);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 16) {
			char tablename_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			char titles_array[100] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, titlepos = cmd.find("SELECT") + 7; cmd_array[titlepos] != ' '&&i < 200; i++, titlepos++)
				titles_array[i] = cmd_array[titlepos];

			string titles = titles_array;
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_columns(Tablehead, titles);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 17) {
			char tablename_array[30] = { '\0' };
			char condition_array[30] = { '\0' };
			char resultfile_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, conditionpos = cmd.find("BY") + 3; cmd_array[conditionpos] != ' '&&cmd_array[conditionpos] != '\0'&&cmd_array[conditionpos] != '\r'&&i < 200; i++, conditionpos++)
				condition_array[i] = cmd_array[conditionpos];
			string condition = condition_array;
			for (int i = 0, resultpos = cmd.find("TO") + 3; cmd_array[resultpos] != ' '&&cmd_array[resultpos] != '\0'&&cmd_array[resultpos] != '\r'&&i < 200; i++, resultpos++)
				resultfile_array[i] = cmd_array[resultpos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(filename, resultfile_array) == 0) {
				cout << "Invalid file name" << endl;
				mode = 0;
				break;
			}
			if (strcmp(resultfile_array, "Record.txt") == 0) {
				mode = 0;
				cout << "INVALID RESULTFILENAME!" << endl;
				break;
			}
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			int type = -1;
			if (cmd.find("ASC") != string::npos)
				type = 1;
			else
				type = 0;

			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_sort(Tablehead, condition, type);
				topt.showTable_sort(Tablehead, condition, type, resultfile_array);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 18) {
			char tablename_array[30] = { '\0' };
			char condition_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, conditionpos = cmd.find("BY") + 3; cmd_array[conditionpos] != ' '&&cmd_array[conditionpos] != '\0'&&cmd_array[conditionpos] != '\r'&&i < 200; i++, conditionpos++)
				condition_array[i] = cmd_array[conditionpos];
			string condition = condition_array;
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			int type = -1;
			if (cmd.find("ASC") != string::npos)
				type = 1;
			else
				type = 0;

			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_sort(Tablehead, condition, type);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 19) {
			char tablename_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			char limit_array[30] = { '\0' };
			char value_array[30] = { '\0' };
			char resultfile_array[30] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, limitpos = cmd.find("WHERE") + 6; cmd_array[limitpos] != ' '&&cmd_array[limitpos] != '='&&cmd_array[limitpos] != '<'&&cmd_array[limitpos] != '>'&&i < 1024; i++, limitpos++)
				limit_array[i] = cmd_array[limitpos];
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			if (strcmp(resultfile_array, "Record.txt") == 0) {
				mode = 0;
				cout << "INVALID RESULTFILENAME!" << endl;
				break;
			}
			int type = 0;
			if (cmd.find("<=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -1;
			}
			else if (cmd.find(">=") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 1;
			}
			else if (cmd.find("!=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("!=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 3;
			}
			else if (cmd.find("=") != string::npos) {
				for (int i = 0, valuepos = cmd.rfind("=") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 0;
			}

			else if (cmd.find("<") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -2;
			}

			else if (cmd.find(">") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 2;
			}
			string limit = limit_array;
			string value = value_array;
			//cout << limit << " " << value << endl;
			for (int i = 0, resultpos = cmd.find("TO") + 3; cmd_array[resultpos] != ' '&&cmd_array[resultpos] != '\0'&&cmd_array[resultpos] != '\r'&&i < 200; i++, resultpos++)
				resultfile_array[i] = cmd_array[resultpos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(filename, resultfile_array) == 0) {
				cout << "Invalid file name" << endl;
				mode = 0;
				break;
			}
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_all_limited(Tablehead, limit, value, type);
				topt.showTable_all_limited(Tablehead, limit, value, type, resultfile_array);
			}	
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 20) {
			char tablename_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			char limit_array[30] = { '\0' };
			char value_array[30] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, limitpos = cmd.find("WHERE") + 6; cmd_array[limitpos] != ' '&&cmd_array[limitpos] != '='&&cmd_array[limitpos] != '<'&&cmd_array[limitpos] != '>'&&i < 1024; i++, limitpos++)
				limit_array[i] = cmd_array[limitpos];
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			int type = 0;
			if (cmd.find("<=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -1;
			}
			else if (cmd.find(">=") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 1;
			}
			else if (cmd.find("!=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("!=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 3;
			}
			else if (cmd.find("=") != string::npos) {
				for (int i = 0, valuepos = cmd.rfind("=") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 0;
			}

			else if (cmd.find("<") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -2;
			}

			else if (cmd.find(">") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 2;
			}
			string limit = limit_array;
			string value = value_array;
			//cout << limit << " " << value << endl;
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_all_limited(Tablehead, limit, value, type);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 21) {
			char tablename_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			char titles_array[100] = { '\0' };
			char limit_array[30] = { '\0' };
			char value_array[30] = { '\0' };
			char resultfile_array[30] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, titlepos = cmd.find("SELECT") + 7; cmd_array[titlepos] != ' '&&i < 200; i++, titlepos++)
				titles_array[i] = cmd_array[titlepos];
			string titles = titles_array;
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			if (strcmp(resultfile_array, "Record.txt") == 0) {
				mode = 0;
				cout << "INVALID RESULTFILENAME!" << endl;
				break;
			}
			for (int i = 0, limitpos = cmd.find("WHERE") + 6; cmd_array[limitpos] != ' '&&cmd_array[limitpos] != '='&&cmd_array[limitpos] != '<'&&cmd_array[limitpos] != '>'&&i < 1024; i++, limitpos++)
				limit_array[i] = cmd_array[limitpos];
			int type = 0;
			if (cmd.find("<=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -1;
			}
			else if (cmd.find(">=") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 1;
			}
			else if (cmd.find("!=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("!=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 3;
			}
			else if (cmd.find("=") != string::npos) {
				for (int i = 0, valuepos = cmd.rfind("=") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 0;
			}

			else if (cmd.find("<") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -2;
			}

			else if (cmd.find(">") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 2;
			}
			string limit = limit_array;
			string value = value_array;
			//cout << limit << " " << value << endl;
			for (int i = 0, resultpos = cmd.find("TO") + 3; cmd_array[resultpos] != ' '&&cmd_array[resultpos] != '\0'&&cmd_array[resultpos] != '\r'&&i < 200; i++, resultpos++)
				resultfile_array[i] = cmd_array[resultpos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(filename, resultfile_array) == 0) {
				cout << "Invalid file name" << endl;
				mode = 0;
				break;
			}

			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_columns_limited(Tablehead, titles, limit, value, type);
				topt.showTable_columns_limited(Tablehead, titles, limit, value, type, resultfile_array);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 22) {
			char tablename_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			char titles_array[100] = { '\0' };
			char limit_array[30] = { '\0' };
			char value_array[30] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, titlepos = cmd.find("SELECT") + 7; cmd_array[titlepos] != ' '&&i < 200; i++, titlepos++)
				titles_array[i] = cmd_array[titlepos];
			string titles = titles_array;
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			for (int i = 0, limitpos = cmd.find("WHERE") + 6; cmd_array[limitpos] != ' '&&cmd_array[limitpos] != '='&&cmd_array[limitpos] != '<'&&cmd_array[limitpos] != '>'&&i < 1024; i++, limitpos++)
				limit_array[i] = cmd_array[limitpos];
			int type = 0;
			if (cmd.find("<=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -1;
			}
			else if (cmd.find(">=") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 1;
			}
			else if (cmd.find("!=") != string::npos) {
				for (int i = 0, valuepos = cmd.find("!=") + 2; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 3;
			}
			else if (cmd.find("=") != string::npos) {
				for (int i = 0, valuepos = cmd.rfind("=") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 0;
			}

			else if (cmd.find("<") != string::npos) {
				for (int i = 0, valuepos = cmd.find("<") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = -2;
			}

			else if (cmd.find(">") != string::npos) {
				for (int i = 0, valuepos = cmd.find(">") + 1; cmd_array[valuepos] != ' '&&cmd_array[valuepos] != '\0'&&cmd_array[valuepos] != '\r'&&i < 200; i++, valuepos++)
					value_array[i] = cmd_array[valuepos];
				type = 2;
			}
			string limit = limit_array;
			string value = value_array;
			//cout << limit << " " << value << endl;

			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_columns_limited(Tablehead, titles, limit, value, type);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 23) {
			char tablename_array[30] = { '\0' };
			char condition_array[30] = { '\0' };
			char resultfile_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			for (int i = 0, resultpos = cmd.find("TO") + 3; cmd_array[resultpos] != ' '&&cmd_array[resultpos] != '\0'&&cmd_array[resultpos] != '\r'&&i < 200; i++, resultpos++)
				resultfile_array[i] = cmd_array[resultpos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(filename, resultfile_array) == 0) {
				cout << "Invalid file name" << endl;
				mode = 0;
				break;
			}
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			if (strcmp(resultfile_array, "Record.txt") == 0) {
				mode = 0;
				cout << "INVALID RESULTFILENAME!" << endl;
				break;
			}
			int type = -1;
			if (cmd.find("MAX") != string::npos) {
				type = 1;
				for (int i = 0, conditionpos = cmd.find("MAX") + 4; cmd_array[conditionpos] != ' '&&cmd_array[conditionpos] != '\0'&&cmd_array[conditionpos] != '\r'&&i < 200; i++, conditionpos++)
					condition_array[i] = cmd_array[conditionpos];
			}
			else if (cmd.find("MIN") != string::npos) {
				type = 2;
				for (int i = 0, conditionpos = cmd.find("MIN") + 4; cmd_array[conditionpos] != ' '&&cmd_array[conditionpos] != '\0'&&cmd_array[conditionpos] != '\r'&&i < 200; i++, conditionpos++)
					condition_array[i] = cmd_array[conditionpos];
			}
			else if (cmd.find("AVERAGE") != string::npos) {
				type = 3;
				for (int i = 0, conditionpos = cmd.find("AVERAGE") + 8; cmd_array[conditionpos] != ' '&&cmd_array[conditionpos] != '\0'&&cmd_array[conditionpos] != '\r'&&i < 200; i++, conditionpos++)
					condition_array[i] = cmd_array[conditionpos];
			}
			string condition = condition_array;
			Table *Tablehead = new Table;
			Tablehead = NULL;
			if (fp.readTable(Tablehead, filename)) {
				topt.showTable_maxminave(Tablehead, condition, type);
				topt.showTable_maxminave(Tablehead, condition, type, resultfile_array);
			}
			delete Tablehead;
			mode = 0;
			break;
		}

		else if (mode == 24) {
			char tablename_array[30] = { '\0' };
			char condition_array[30] = { '\0' };
			char resultfile_array[30] = { '\0' };
			char cmd_array[200] = { '\0' };
			strcpy(cmd_array, cmd.c_str());
			for (int i = 0, tablepos = cmd.find("FROM") + 5; cmd_array[tablepos] != ' '&&cmd_array[tablepos] != '\0'&&cmd_array[tablepos] != '\r'&&i < 200; i++, tablepos++)
				tablename_array[i] = cmd_array[tablepos];
			char filename[50] = { '\0' };
			strcpy(filename, tablename_array);
			strcat(filename, ".txt");
			if (strcmp(tablename_array, "Record") == 0) {
				mode = 0;
				cout << "INVALID TABLENAME!" << endl;
				break;
			}
			int type = -1;
			if (cmd.find("MAX") != string::npos) {
				type = 1;
				for (int i = 0, conditionpos = cmd.find("MAX") + 4; cmd_array[conditionpos] != ' '&&cmd_array[conditionpos] != '\0'&&cmd_array[conditionpos] != '\r'&&i < 200; i++, conditionpos++)
					condition_array[i] = cmd_array[conditionpos];
			}
			else if (cmd.find("MIN") != string::npos) {
				type = 2;
				for (int i = 0, conditionpos = cmd.find("MIN") + 4; cmd_array[conditionpos] != ' '&&cmd_array[conditionpos] != '\0'&&cmd_array[conditionpos] != '\r'&&i < 200; i++, conditionpos++)
					condition_array[i] = cmd_array[conditionpos];
			}
			else if (cmd.find("AVERAGE") != string::npos) {
				type = 3;
				for (int i = 0, conditionpos = cmd.find("AVERAGE") + 8; cmd_array[conditionpos] != ' '&&cmd_array[conditionpos] != '\0'&&cmd_array[conditionpos] != '\r'&&i < 200; i++, conditionpos++)
					condition_array[i] = cmd_array[conditionpos];
			}
			string condition = condition_array;

			Table *Tablehead = new Table;
			Tablehead = NULL;
			if(fp.readTable(Tablehead, filename))
				topt.showTable_maxminave(Tablehead, condition, type);
			delete Tablehead;
			mode = 0;
			break;
		}
	}
	if (quitflag)
		return true;
	else 
		return false;
}

void dointxt(string cmd) {
	char filename[30] = { '\0' };
	char cmd_array[200]= { '\0' };
	strcpy(cmd_array, cmd.c_str());
	for (int j = 0, filepos = cmd.find("FROM") + 5; cmd_array[filepos] != ' '&&cmd_array[filepos] != '\0'&&cmd_array[filepos] != '\r'&&j < 200; j++, filepos++)
		filename[j] = cmd_array[filepos];
	//cout << filename << endl;
	string test = filename;
	if (test.find("todo_") != 0) {
		cout << "Not a todo FILE!" << endl;
		return;
	}
	std::ifstream fin(filename, std::ios::in);
	if (!fin) {
		cout << "File open error!\n";
		return;
	}

	char buf[1024] = { '\0' };
	while (fin.getline(buf, sizeof(buf), '\n')) {
		string todo = buf;
		cout <<"cmd:["<<todo <<"]"<< endl;
		int mode = cmd_match(todo);
		if (cmd_todo(todo, mode))
			break;
	}
	fin.clear();
	fin.close();
}

void maketodo(string cmd) {
	char filename[30] = { '\0' };
	char cmd_array[200] = { '\0' };
	strcpy(cmd_array, cmd.c_str());
	for (int j = 0, filepos = cmd.find("TO") + 3; cmd_array[filepos] != ' '&&cmd_array[filepos] != '\0'&&cmd_array[filepos] != '\r'&&j < 200; j++, filepos++)
		filename[j] = cmd_array[filepos];
	//cout << filename << endl;
	string test = filename;
	if (test.find("todo_") != 0) {
		cout << "Not a todo FILE!" << endl;
		return;
	}
	FILE* fp;
	fp = fopen(filename, "w");
	fclose(fp);
	ofstream fout(filename);
	if (!fout) {
		cout << "File open error!\n";
		return;
	}
	cout << "Type in the number of commands:";
	int num;
	cin >> num;
	cin.ignore(1024, '\n');
	while (num!=0) {
		string line;
		getline(cin, line);
		fout << line << endl;
		num--;
	}
	fout.clear();
	fout.close();
}
void SQL_menu() {
	cout << "~$ ";
	string enter;
	std::getline(cin, enter);
	if (enter != "mySQL")
		return;
	std::string cmd;
	std::regex pattern_todo("(DO)(\\s+)(FROM)(\\s+)((?:[A-Za-z0-9_]*))(\\.)(txt)");
	std::regex pattern_maketodo("(MAKE)(\\s+)(TO)(\\s+)((?:[A-Za-z0-9_]*))(\\.)(txt)");
	while (1) {
		cout << "(mysql)==> ";
		std::getline(cin, cmd);
		//cout << cmd << endl;
		if (std::regex_match(cmd, pattern_todo)) {
			dointxt(cmd);
		}

		else if (std::regex_match(cmd, pattern_maketodo)) {
			maketodo(cmd);
		}

		else {
			int mode = 0;
			mode = cmd_match(cmd);
			if (cmd_todo(cmd, mode))
				break;
		}
	}
	cout << "~$" << endl;
}

int main() {
	fp.readRecord(Recordhead);
	SQL_menu();
	return 0;
}
