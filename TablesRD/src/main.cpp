#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <list>

#include "mtwist.h"
#include "Environment.h"
#include "Agent.h"
#include "TableObj.h"

using namespace std;

string Upper(string a) {
	string ret = a;
	transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
	return ret;
}

string PopReturnUpper(list<string>* a)
{
	string ret = a->front();
	ret = Upper(ret);
	a->pop_front();

	return ret;
}

string PopReturnNormal(list<string>* a)
{
	string ret = a->front();
	a->pop_front();

	return ret;
}

int StrNum(string s)
{
	int number;
	if (!(istringstream(s) >> number)) number = -1;

	return number;
}

void InterpretError()
{
	printf ("ERROR: could not interpret command.\n");
}

void EnvironmentError()
{
	printf ("ERROR: Environment not initialized.  Use 'start' first.\n");
}

void DisplaySettings()
{
	printf ("Settings on generation:\n");
	printf ("# Agents: %d\n", Environment::numAgents);
	printf ("# Tables: %d\n", Environment::numTables);
	printf ("Loner preference: %d\n", Environment::lonerPreference);
	printf ("Auto-seat on Step: ");
	if (Environment::seatOnStep) printf ("TRUE\n");
	else printf ("FALSE\n");
	printf ("Show detailed util calc: ");
	if (Environment::D_CALCUTIL) printf ("TRUE\n");
	else printf ("FALSE\n");

	printf ("Show Parse on Load: ");
	if (Environment::SHOW_PARSE) printf ("TRUE\n");
	else printf ("FALSE\n");

	printf ("Enable 1-for-1 Swaps: ");
	if (bool Environment::ALLOW_SWAPS) printf ("TRUE\n");
	else printf ("FALSE\n");

	// printf ("Default personality types: %d\n", Environment::personalityTypes);
	printf ("Table capacity: %d\n\n", Environment::tableCapacity);
}

// Wipes out existing environment
void ClearEnvironment() {
	Environment::allAgents.erase(Environment::allAgents.begin(), Environment::allAgents.begin() + Environment::allAgents.size());
	Environment::allTables.erase(Environment::allTables.begin(), Environment::allTables.begin() + Environment::allTables.size());
	Environment::agentID = 0;
	Environment::tableID = 0;
	Environment::initialized = false;

	printf ("Old environment erased.\n");
}

// Creates new environment from default settings
void InitializeEnvironment() {
	printf ("Initializing environment.\n");
	for (int j = 0; j < Environment::numAgents; j++)
		Environment::AddAgent();
	for (int j = 0; j < Environment::numTables; j++)
		Environment::AddTable();
	Environment::initialized = true;

	printf ("New environment initialized.\n");
}

void AddAgent(list<string> commands) {
	string arg = PopReturnUpper(&commands);
	int number = StrNum(arg);

	if (number < 0 || number >= Environment::personalityTypes) {
		printf ("ERROR: Personality type must range between 0 and %d\n", Environment::personalityTypes - 1);
	}
	else {
		Environment::AddAgent(number);
		printf ("New agent added with personality %d\n", number);
		if (Environment::tableID > 0 && Environment::agentID > 0 && !Environment::initialized)
		{
			Environment::initialized = true;
			printf("The environment is now considered initialized.\n");
		}
	}
}

void AddTable(list<string> commands) {
	string arg = PopReturnUpper(&commands);
	int number = StrNum(arg);

	if (number <= 0) {
		printf ("ERROR: Table capacity must be at least 1.\n");
	}
	else {
		Environment::AddTable(number);
		printf ("New table added with capacity %d\n", number);
		if (Environment::tableID > 0 && Environment::agentID > 0 && !Environment::initialized)
		{
			Environment::initialized = true;
			printf("The environment is now considered initialized.\n");
		}
	}
}

void ToggleSettings(list<string> commands) {
	string arg = PopReturnUpper(&commands);

	if (arg == "AUTOSEAT") {
		Environment::seatOnStep = !Environment::seatOnStep;
		DisplaySettings();
	}

	else if (arg == "UTILCALC") {
		Environment::D_CALCUTIL = !Environment::D_CALCUTIL;
		DisplaySettings();
	}

	else if (arg == "PARSE") {
		Environment::SHOW_PARSE = !Environment::SHOW_PARSE;
		DisplaySettings();
	}

	else
		InterpretError();
}

void SetAgents(list<string> commands) {
	string arg = PopReturnUpper(&commands);
	int number = StrNum(arg);

	if (number <= 0) {
		printf ("ERROR: Must have at least 1 agent in environment.\n");
	} else {
		Environment::numAgents = number;
		DisplaySettings();
	}
}

void SetCapacity(list<string> commands) {
	string arg = PopReturnUpper(&commands);
	int number = StrNum(arg);

	if (number <= 0) {
		printf ("ERROR: Tables must have a capacity of at least 1.\n");
	} else {
		Environment::tableCapacity = number;
		DisplaySettings();
	}
}

void SetTables(list<string> commands) {
	string arg = PopReturnUpper(&commands);
	int number = StrNum(arg);

	if (number <= 0) {
		printf ("ERROR: Must have at least 1 table in environment.\n");
	} else {
		Environment::numTables = number;
		DisplaySettings();
	}
}

void SetLoner(list<string> commands) {
	string arg = PopReturnUpper(&commands);
	int number = StrNum(arg);

	if (number < 0 || number >= Environment::personalityTypes) {
		printf ("ERROR: Must enter number between 0 and %d\n", Environment::personalityTypes - 1);
	}
	else {
		Environment::lonerPreference = number;
		DisplaySettings();
	}
}

void EditAgent(list<string> commands) {
	Agent* a;
	string arg = PopReturnUpper(&commands);
	int number = StrNum(arg);

	if (number < 0 || number >= int(Environment::allAgents.size()))
		printf ("ERROR!  Agent # entered is out of range.  Valid range is between 0 and %lu.\n", Environment::allAgents.size() - 1);
	else {
		a = Environment::allAgents.at(number);

		arg = PopReturnUpper(&commands);
		number = StrNum(arg);

		if (number < 0 || number >= Environment::personalityTypes)
			printf ("ERROR!  Personality number entered is out of range.  Valid range is between 0 and %d.\n", Environment::personalityTypes - 1);
		else
		{
			a->personality = number;
			printf ("Agent %d personality is now set to %d.\n", a->id, a->personality);
		}
	}
}

void EditTable(list<string> commands) {
	TableObj* t;
	string arg = PopReturnUpper(&commands);

	int number = StrNum(arg);

	if (number < 0 || number >= int(Environment::allTables.size()))
		printf ("ERROR!  Table # entered is out of range.  Valid range is between 0 and %lu.\n", Environment::allTables.size() - 1);
	else {
		t = Environment::allTables.at(number);

		arg = PopReturnUpper(&commands);
		number = StrNum(arg);

		if (number <= 0)
			printf ("ERROR!  Capacity # entered is less than 1!\n");
		else if (number < t->numAtTable)
			printf ("ERROR!  Desired capacity is less than the %d agents already at the table!  Unseat extra agents first.\n", t->numAtTable);
		else {
			t->capacity = number;
			printf ("Table %d capacity is now set to %d.\n", t->id, t->capacity);
		}
	}
}

void DisplayHelp() {
	printf ("Available commands: \n");
	printf ("add agent #       - adds a new agent with personality #.\n");
	printf ("add table #       - adds a new table with capacity #.\n");
	printf ("clear             - removes all agents and tables from the environment.\n");
	printf ("display agent #   - displays info for a given agent.\n");
	printf ("display agents    - displays info for all agents.\n");
	printf ("display table #   - displays info for a given table.\n");
	printf ("display tables    - displays info for all tables.\n");
	printf ("edit agent # X    - changes personality of agent # to X.\n");
	printf ("edit table # X    - changes capacity of table # to X.\n");
	printf ("exit || quit      - ends the program.\n");
	printf ("load <filename>   - loads a particular filename as the environment.\n");
	printf ("move # to #       - moves an agent to the specified table.\n");
	printf ("reset             - same as 'start' - replaces existing environment.\n");
	printf ("seat all          - seats all agents randomly at the tables.\n");
	printf ("set agents #      - sets # of agents to spawn\n");
	printf ("set capacity #    - sets table capacity on spawn\n");
	printf ("set loner #       - sets loner preference.  Utility goes negative if \n"
			"                    #PersTypes - abs(diff in personality) exceeds this\n"
			"                    threshold, whereas sitting alone gives utility 0\n");
	printf ("set tables #      - sets # of tables to spawn\n");
	printf ("social            - shows total utility of entire system.\n");
	printf ("start             - creates tables and agents based on environment settings.\n");
	printf ("step              - each agent sequentially sees if there is a better table.\n");
	printf ("step #            - as 'step' but iterates # times.\n");
	printf ("step agent #      - agent # will move to a better table if possible.\n");
	printf ("swap # #          - swap two agents (only works if at separate tables)\n");
	printf ("toggle autoseat   - unseated agents will try to find a random seat on 'step'\n");
	printf ("toggle parse      - shows parse details when loading environment file.\n");
	printf ("toggle utilcalc   - detailed output for utility calculations.\n");
	printf ("unseat #          - removes agent # from his table.\n");
	printf ("unseat all        - removes all agents from tables.\n");
}

void SeatAll() {
	for (unsigned i = 0; i < Environment::allAgents.size(); i++)
		Environment::allAgents.at(i)->SeatRandomly();

	printf ("All agents seated.\n");
}

void Step(int steps) {
	for (int i = 0; i < steps; i++) {
		if (steps > 1) printf ("\n --- Step %d --- \n\n", i + 1);

		for (unsigned i = 0; i < Environment::allAgents.size(); i++)
		{
			if (Environment::USE_NORMALIZED_UTILITY)
				Environment::allAgents.at(i)->FindBetterTableNorm();
			else
				Environment::allAgents.at(i)->FindBetterTable();
		}
	}
}

void StepAgent(list<string> commands) {
	string arg = PopReturnUpper(&commands);
	int number = StrNum(arg);

	if (number < 0 || number >= Environment::numAgents)
		printf ("Agent number entered out of bounds.  Agents are between 0 and %d.\n", Environment::agentID - 1);
	else
	{
		if (Environment::USE_NORMALIZED_UTILITY)
			Environment::allAgents.at(number)->FindBetterTableNorm();
		else
			Environment::allAgents.at(number)->FindBetterTable();
	}
}

void DisplayTable(list<string> commands) {
	string arg = PopReturnUpper(&commands);
	int number = StrNum(arg);

	if (number < 0 || number >= Environment::tableID)
		printf ("Table number entered was out of bounds.  Tables are between 0 and %d\n", Environment::tableID - 1);
	else
		Environment::allTables.at(number)->DisplayTableInfo();
}

void DisplayAllTables() {
	for (unsigned i = 0; i < Environment::allTables.size(); i++)
		Environment::allTables.at(i)->DisplayTableInfo();
}

void DisplayAgent(list<string> commands) {
	string arg = PopReturnUpper(&commands);
	int number = StrNum(arg);

	if (number < 0 || number >= Environment::agentID)
		printf ("Agent number entered was out of bounds.  Agents are between 0 and %d\n", Environment::agentID - 1);
	else
		Environment::allAgents.at(number)->DisplayAgentInfo();
}

void DisplayAllAgents() {
	for (unsigned i = 0; i < Environment::allAgents.size(); i++)
		Environment::allAgents.at(i)->DisplayAgentInfo();
}

void SwapAgents(list<string> commands) {
	int a1;
	int a2;
	string arg = PopReturnUpper(&commands);
	if (!(istringstream(arg) >> a1)) a1 = -1;

	if (a1 < 0 || a1 > Environment::agentID) {
		InterpretError();
	}

	arg = PopReturnUpper(&commands);
	if (!(istringstream(arg) >> a2)) a2 = -1;

	if (a2 < 0 || a2 > Environment::agentID) {
		InterpretError();
	}

	Agent* agent1 = Environment::allAgents.at(a1);
	Agent* agent2 = Environment::allAgents.at(a2);

	agent1->SwapWith(agent2);
}

void UnseatAll() {
	for (unsigned j = 0; j < Environment::allAgents.size(); j++)
		Environment::allAgents.at(j)->UnseatAgent();

	printf ("All agents have been unseated.\n");
}

void UnseatAgent(list<string> commands) {
	int a;
	string arg = PopReturnUpper(&commands);
	if (!(istringstream(arg) >> a)) a = -1;

	if (a < 0 || a >= Environment::agentID)
		InterpretError();
	else {
		Agent* g = Environment::allAgents.at(a);

		if (g->currentTable == NULL)
		{
			printf ("Agent %d is already unseated!\n", g->id);
		}

		g->UnseatAgent();
		printf ("Agent %d has been unseated.\n", g->id);
	}
}



void SeatAgent(list<string> commands, Agent* a) {
	string arg = PopReturnUpper(&commands);
	int num = StrNum(arg);

	if (num < 0 || num >= Environment::tableID)
		InterpretError();
	else {
		TableObj* t = Environment::allTables.at(num);

		if (t->numAtTable >= t->capacity) {
			printf ("ERROR: Table %d is already full!\n", t->id);
		}
		else {
			if (a->currentTable != NULL)
			{
				a->UnseatAgent();
			}

			t->AddToTable(a);

			printf ("Agent %d has been moved to table %d\n", a->id, a->currentTable->id);
		}
	}
}


// Reads a data file and loads it as the environment
void LoadEnvironment(list<string> commands) {
	string arg = PopReturnNormal(&commands);
	const char *filename = arg.c_str();
	bool LoadTables = false;
	bool LoadAgents = false;
	bool isNum = false;

	printf ("File name selected: ");
	cout << arg << "\n";

	ifstream efile;
	efile.open(filename, ios::in);

	string buffer;
	list<string> fcommand;

	if (efile.is_open()) {
		int lineNumber = 0;
		while (getline(efile, buffer)) {
			lineNumber++;

			if (buffer.size() == 0) continue;
			else if (buffer.at(0) == ';') {
				if (Environment::SHOW_PARSE) {
					printf ("COMMENT Line %d: ", lineNumber);
					cout << buffer << "\n";
				}

				continue;
			}
			// Parse input
			else {
				for (unsigned cursor = 0; cursor < buffer.length();)
				{
					char c = buffer.at(cursor);

					if (c == 32)
						cursor++;
					else {
						int jumpto = buffer.find(' ', cursor);
						if (jumpto == -1) jumpto = buffer.length();

						string arg = buffer.substr(cursor, jumpto - cursor);
						fcommand.push_back(arg);

						cursor = jumpto;
					}
				}
			}

			if (Environment::SHOW_PARSE) cout << "Line " << lineNumber << ": " << buffer << "\n";
			char c = fcommand.front().at(0);
			if (c >= '0' && c <= '9') isNum = true;
			else isNum = false;

			for (unsigned i = 0; i <= fcommand.size(); i++) {

				if (isNum)
				{
					if (LoadAgents) {
						AddAgent(fcommand);
						fcommand.pop_front();
						if (fcommand.size() == 1) {
							Agent* a = Environment::allAgents.back();
							SeatAgent(fcommand, a);
							fcommand.pop_front();
						}
						continue;
					}
					else if (LoadTables) {
						AddTable(fcommand);
						fcommand.clear();
						break;
					}
				}
				string arg = PopReturnNormal(&fcommand);
				// if (SHOW_PARSE) cout << "/" << arg << "/ ";

				if (Upper(arg) == "TABLES")
				{
					if (LoadTables || LoadAgents)
					{
						printf ("ERROR reading line %d: Either Tables or Agents are already loading!\n", lineNumber);
						printf ("Aborting load and cleaning environment.\n");
						ClearEnvironment();
						return;
					}
					else if (!LoadTables)
					{
						printf ("Now loading tables from file.\n");
						LoadTables = true;
					}
				}

				else if (Upper(arg) == "ENDTABLES") {
					if (!LoadTables || LoadAgents) {
						printf ("ERROR reading line %d: Either tables are not loading or agents are already loading!\n", lineNumber);
						printf ("Aborting load and cleaning environment.\n");
						ClearEnvironment();
						return;
					} else if (LoadTables) {
						printf ("Tables are finished loading from file.\n");
						LoadTables = false;
					}
				}

				if (Upper(arg) == "AGENTS")
				{
					if (LoadTables || LoadAgents)
					{
						printf ("ERROR reading line %d: Either Tables or Agents are already loading!\n", lineNumber);
						printf ("Aborting load and cleaning environment.\n");
						ClearEnvironment();
						return;
					} else if (!LoadAgents)
					{
						printf ("Now loading agents from file.\n");
						LoadAgents = true;
					}
				}

				else if (Upper(arg) == "ENDAGENTS") {
					if (!LoadAgents || LoadTables) {
						printf ("ERROR reading line %d: Either tables are not loading or agents are already loading!\n", lineNumber);
						printf ("Aborting load and cleaning environment.\n");
						ClearEnvironment();
						return;
					} else if (LoadAgents) {
						printf ("Agents are finished loading from file.\n");
						LoadAgents = false;
					}
				}

			}
		}

	}
	else printf ("ERROR: File did not load properly.\n");
}

void MoveAgent(list<string> commands) {
	int num;
	Agent* a;
	string arg = PopReturnUpper(&commands);
	num = StrNum(arg);

	if (num < 0 || num >= Environment::agentID)
		InterpretError();
	else {
		a = Environment::allAgents.at(num);
		printf ("Got agent %d\n", a->id);
		a->DisplayAgentInfo();

		arg = PopReturnUpper(&commands);

		if (arg != "TO") {
			InterpretError();
		}
		else {
			SeatAgent(commands, a);
		}
	}
}

void CalcSocial() {
	printf ("---Total Utility for All Agents: %d (%0.3f normalized)---",
		Environment::CalcSocialUtility(), Environment::CalcSocialUtilityNorm());
}

int main()
{
	mt_seed32new(mt_seed());

	// Initialize starting ID numbers
	Environment::tableID = 0;
	Environment::agentID = 0;
	Environment::numAgents = 20;
	Environment::numTables = 3;
	Environment::personalityTypes = 10;
	Environment::tableCapacity = 8;
	Environment::lonerPreference = 0;

	Environment::seatOnStep = false;
	Environment::initialized = false;
	Environment::D_CALCUTIL = false;
	Environment::SHOW_PARSE = false;

	if (Environment::D_CONSTRUCTORS)
		printf ("Environment initialized with starting table ID %d and starting agent ID %d\n", Environment::tableID, Environment::agentID);

	string buffer;
	bool runProgram = true;

	printf ("Welcome to the Switch-Tables Simulator.\n\n");

	DisplaySettings();

	printf ("Type 'start' to initialize environment with the above conditions.\n");
	printf ("Otherwise, you must add at least one table and one agent to begin.\n");
	printf ("Type 'seat all' to randomly seat agents at tables.\n");
	printf ("Type 'step' to progress the simulation.\n\n");
	printf ("Type 'help' for all commands.\n");

	// Command line input
	while (runProgram)
	{
		list<string> commands;
		printf ("\n>");

		getline (cin, buffer);


		// Parses input
		for (unsigned cursor = 0; cursor < buffer.length();)
		{
			char c = buffer.at(cursor);

			if (c == 32)
				cursor++;
			else {
				int jumpto = buffer.find(' ', cursor);
				if (jumpto == -1) jumpto = buffer.length();

				string arg = buffer.substr(cursor, jumpto - cursor);
				commands.push_back(arg);

				cursor = jumpto;
			}
		}

		unsigned argc = commands.size();
		if (argc == 0) continue;
		string arg = PopReturnUpper(&commands);

		if (arg == "EXIT" || arg == "QUIT") {
			runProgram = false;
		}

		else if (arg == "CLEAR") ClearEnvironment();

		else if (arg == "START" || arg == "RESET") {
			if (Environment::initialized) ClearEnvironment();
			InitializeEnvironment();
		}

		else if (arg == "ADD" && argc == 3) {
			arg = PopReturnUpper(&commands);

			if (arg == "AGENT") AddAgent(commands);
			else if (arg == "TABLE") AddTable(commands);
			else InterpretError();
		}

		else if (arg == "TOGGLE" && argc == 2) ToggleSettings(commands);

		else if (arg == "SET" && argc == 3) {
			arg = PopReturnUpper(&commands);

			if (arg == "AGENTS") SetAgents(commands);
			else if (arg == "CAPACITY") SetCapacity(commands);
			else if (arg == "TABLES") SetTables(commands);
			else if (arg == "LONER") SetLoner(commands);
		}

		else if (arg == "EDIT" && argc == 4) {
			arg = PopReturnUpper(&commands);

			if (arg == "AGENT") EditAgent(commands);
			else if (arg == "TABLE") EditTable(commands);
		}

		else if (arg == "HELP") DisplayHelp();

		else if (arg == "LOAD" && argc == 2) {
			if (Environment::initialized) {
				printf ("ERROR: Environment already initialized!  You must clear the current environment first.\n");
			} else
				LoadEnvironment(commands);
		}

		// ------------------ All commands below require initialization
		else if (!Environment::initialized)
			EnvironmentError();

		else if (arg == "SEAT" && argc == 2) {
			arg = PopReturnUpper(&commands);

			if (arg == "ALL") SeatAll();
			else InterpretError();
		}

		else if (arg == "STEP") {
			// "Step" all agents one time
			if (argc == 1) Step(1);
			else {
				arg = PopReturnUpper(&commands);

				if (arg == "AGENT" && argc == 3) StepAgent(commands);
				else if (argc == 2)	{
					arg = PopReturnUpper(&commands);
					int number = StrNum(arg);
					Step(number);
				} else InterpretError();
			}
		}

		else if (arg == "DISPLAY") {
			arg = PopReturnUpper(&commands);

			if (arg == "TABLE" && argc == 3) DisplayTable(commands);
			else if (arg == "TABLES" && argc == 2) DisplayAllTables();
			else if (arg == "AGENT" && argc == 3) DisplayAgent(commands);
			else if (arg == "AGENTS" && argc == 2) DisplayAllAgents();
		}

		else if (arg == "SWAP" && argc == 3) SwapAgents(commands);

		else if (arg == "UNSEAT" && argc == 2) {
			arg = PopReturnUpper(&commands);

			if (arg == "ALL") UnseatAll();
			else UnseatAgent(commands);
		}

		else if (arg == "MOVE" && argc == 4) MoveAgent(commands);

		else if (arg == "SOCIAL" && argc == 1) CalcSocial();
		else InterpretError();
	}
}
