#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <list>

#include "mtwist.h"
#include "environment.h"
#include "Agent.h"
#include "TableObj.h"

using namespace std;

string PopReturnUpper(list<string>* a)
{
	string ret = a->front();
	a->pop_front();
	transform(ret.begin(), ret.end(), ret.begin(), ::toupper);

	return ret;
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
	// printf ("Default personality types: %d\n", Environment::personalityTypes);
	printf ("Table capacity: %d\n\n", Environment::tableCapacity);
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

	if (Environment::D_CONSTRUCTORS)
		printf ("Environment initialized with starting table ID %d and starting agent ID %d\n", Environment::tableID, Environment::agentID);

	string buffer;
	bool runProgram = true;

	printf ("Welcome to the Switch-Tables Simulator.\n\n");

	DisplaySettings();

	printf ("Type 'start' to initialize environment with the above conditions.\n");
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
		string arg = PopReturnUpper(&commands);

		if (arg == "EXIT" || arg == "QUIT") {
			runProgram = false;
		}

		else if (arg == "START" || arg == "RESET") {
			if (Environment::initialized) {
				// Wipes out existing environment
				Environment::allAgents.erase(Environment::allAgents.begin(), Environment::allAgents.begin() + Environment::allAgents.size());
				Environment::allTables.erase(Environment::allTables.begin(), Environment::allTables.begin() + Environment::allTables.size());
				Environment::agentID = 0;
				Environment::tableID = 0;

				printf ("Old environment erased.\n");
			}

			for (int j = 0; j < Environment::numAgents; j++)
				Environment::AddAgent();
			for (int j = 0; j < Environment::numTables; j++)
				Environment::AddTable();
			Environment::initialized = true;

			printf ("New environment initialized.\n");
		}

		else if (arg == "TOGGLE" && argc == 2) {
			arg = PopReturnUpper(&commands);

			if (arg == "AUTOSEAT") {
				Environment::seatOnStep = !Environment::seatOnStep;
				DisplaySettings();
			}

			else if (arg == "UTILCALC") {
				Environment::D_CALCUTIL = !Environment::D_CALCUTIL;
				DisplaySettings();
			}

			else
				EnvironmentError();
		}

		else if (arg == "SET" && argc == 3) {
			arg = PopReturnUpper(&commands);

			if (arg == "AGENTS") {
				arg = PopReturnUpper(&commands);
				int number;
				if (!(istringstream(arg) >> number)) number = -1;

				if (number <= 0) {
					EnvironmentError();
				} else {
					Environment::numAgents = number;
					DisplaySettings();
				}
			}

			if (arg == "CAPACITY") {
				arg = PopReturnUpper(&commands);
				int number;
				if (!(istringstream(arg) >> number)) number = -1;

				if (number <= 0) {
					EnvironmentError();
				} else {
					Environment::tableCapacity = number;
					DisplaySettings();
				}
			}

			if (arg == "TABLES") {
				arg = PopReturnUpper(&commands);
				int number;
				if (!(istringstream(arg) >> number)) number = -1;

				if (number <= 0) {
					EnvironmentError();
				} else {
					Environment::numTables = number;
					DisplaySettings();
				}
			}

			if (arg == "LONER") {
				arg = PopReturnUpper(&commands);
				int number;
				if (!(istringstream(arg) >> number)) number = -1;

				if (number < 0 || number >= Environment::personalityTypes) {
					printf ("ERROR: Must enter number between 0 and %d\n", Environment::personalityTypes - 1);
				}
				else {
					Environment::lonerPreference = number;
					DisplaySettings();
				}
			}
		}

		else if (arg == "EDIT" && argc == 4) {
			arg = PopReturnUpper(&commands);

			if (arg == "AGENT") {
				Agent* a;

				arg = PopReturnUpper(&commands);
				int number;
				if (!(istringstream(arg) >> number)) number = -1;

				if (number < 0 || number >= int(Environment::allAgents.size())){
					printf ("ERROR!  Agent # entered is out of range.  Valid range is between 0 and %d.\n", Environment::allAgents.size() - 1);

				}
				else
				{
					a = Environment::allAgents.at(number);

					arg = PopReturnUpper(&commands);
					if (!(istringstream(arg) >> number)) number = -1;


					if (number < 0 || number >= Environment::personalityTypes) {
						printf ("ERROR!  Personality number entered is out of range.  Valid range is between 0 and %d.\n", Environment::personalityTypes - 1);
					}
					else
					{
						a->personality = number;
						printf ("Agent %d personality is now set to %d.\n", a->id, a->personality);
					}
				}
			}

			if (arg == "TABLE") {
				TableObj* t;

				arg = PopReturnUpper(&commands);
				int number;
				if (!(istringstream(arg) >> number)) number = -1;

				if (number < 0 || number >= int(Environment::allTables.size())) {
					printf ("ERROR!  Table # entered is out of range.  Valid range is between 0 and %d.\n", Environment::allTables.size() - 1);
				}
				else
				{
					t = Environment::allTables.at(number);

					arg = PopReturnUpper(&commands);
					if (!(istringstream(arg) >> number)) number = -1;

					if (number <= 0) {
						printf ("ERROR!  Capacity # entered is less than 1!\n");
					}
					else if (number < t->numAtTable) {
						printf ("ERROR!  Desired capacity is less than the %d agents already at the table!  Unseat extra agents first.\n", t->numAtTable);
					}
					else {
						t->capacity = number;
						printf ("Table %d capacity is now set to %d.\n", t->id, t->capacity);
					}
				}
			}
		}

		else if (arg == "HELP") {
			printf ("Available commands: \n");
			printf ("display agent #   - displays info for a given agent.\n");
			printf ("display agents    - displays info for all agents.\n");
			printf ("display table #   - displays info for a given table.\n");
			printf ("display tables    - displays info for all tables.\n");
			printf ("edit agent # X    - changes personality of agent # to X.\n");
			printf ("edit table # X    - changes capacity of table # to X.\n");
			printf ("exit || quit      - ends the program.\n");
			printf ("move # to #       - moves an agent to the specified table.\n");
			printf ("reset             - same as 'start' - replaces existing environment.\n");
			printf ("seat all          - seats all agents randomly at the tables.\n");
			printf ("set agents #      - sets # of agents to spawn\n");
			printf ("set capacity #    - sets table capacity on spawn\n");
			printf ("set loner #       - sets loner preference.  Utility goes negative if #PersTypes - abs(diff in personality)\n"
					"                    exceeds this threshold, whereas sitting alone gives utility 0\n");
			printf ("set tables #      - sets # of tables to spawn\n");
			printf ("social            - shows total utility of entire system.\n");
			printf ("start             - creates tables and agents based on environment settings.\n");
			printf ("step              - each agent sequentially sees if he can move to a better table.\n");
			printf ("step #            - as 'step' but iterates # times.\n");
			printf ("step agent #      - agent # will move to a better table if possible.\n");
			printf ("swap # #          - swap two agents (only works if at separate tables)\n");
			printf ("toggle autoseat   - unseated agents will try to find a random seat on 'step'\n");
			printf ("toggle utilcalc   - detailed output for utility calculations.\n");
			printf ("unseat #          - removes agent # from his table.\n");
			printf ("unseat all        - removes all agents from tables.\n");


		}

		// ------------------ All commands below require initialization
		else if (!Environment::initialized)
		{
			InterpretError();

		}

		else if (arg == "SEAT" && argc == 2) {
			arg = PopReturnUpper(&commands);

			if (arg == "ALL") {
				for (unsigned i = 0; i < Environment::allAgents.size(); i++)
					Environment::allAgents.at(i)->SeatRandomly();

				printf ("All agents seated.\n");
			}

			else
				InterpretError();
		}

		else if (arg == "STEP") {
			// "Step" all agents one time
			if (argc == 1) {
				for (unsigned i = 0; i < Environment::allAgents.size(); i++)
				{
					if (Environment::USE_NORMALIZED_UTILITY)
						Environment::allAgents.at(i)->FindBetterTableNorm();
					else
						Environment::allAgents.at(i)->FindBetterTable();
				}
			}

			else {
				arg = PopReturnUpper(&commands);

				// "Step Agent #" command - lets agent # make a move
				if (arg == "AGENT" && argc == 3)
				{
					arg = PopReturnUpper(&commands);

					int number;
					if (!(istringstream(arg) >> number)) number = -1;

					if (number < 0 || number >= Environment::numAgents)
					{
						printf ("Agent number entered out of bounds.  Agents are between 0 and %d.\n", Environment::agentID - 1);

					}
					else
					{
						if (Environment::USE_NORMALIZED_UTILITY)
							Environment::allAgents.at(number)->FindBetterTableNorm();
						else
							Environment::allAgents.at(number)->FindBetterTable();
					}

				}

				// Step # command - iterates step # times
				else if (argc == 2)
				{
					arg = PopReturnUpper(&commands);

					int number;
					if (!(istringstream(arg) >> number)) number = -1;

					for (int j = 0; j < number; j++)
					{
						printf ("\n --- Step %d --- \n\n", j + 1);
						for (unsigned k = 0; k < Environment::allAgents.size(); k++)
							Environment::allAgents.at(k)->FindBetterTableNorm();

					}
				}

				else {
					InterpretError();
				}

			}

		}

		else if (arg == "DISPLAY") {
			arg = PopReturnUpper(&commands);

			if (arg == "TABLE" && argc == 3) {
				arg = PopReturnUpper(&commands);

				int number;
				if (!(istringstream(arg) >> number)) number = -1;

				if (number < 0 || number >= Environment::tableID)
					printf ("Table number entered was out of bounds.  Tables are between 0 and %d\n", Environment::tableID - 1);
				else
					Environment::allTables.at(number)->DisplayTableInfo();
			}

			else if (arg == "TABLES" && argc == 2) {
				for (unsigned i = 0; i < Environment::allTables.size(); i++)
					Environment::allTables.at(i)->DisplayTableInfo();
			}

			else if (arg == "AGENT" && argc == 3) {
				arg = PopReturnUpper(&commands);

				int number;
				if (!(istringstream(arg) >> number)) number = -1;

				if (number < 0 || number >= Environment::agentID)
					printf ("Agent number entered was out of bounds.  Agents are between 0 and %d\n", Environment::agentID - 1);
				else
					Environment::allAgents.at(number)->DisplayAgentInfo();
			}

			else if (arg == "AGENTS" && argc == 2) {
				for (unsigned i = 0; i < Environment::allAgents.size(); i++)
					Environment::allAgents.at(i)->DisplayAgentInfo();
			}
		}


		// SWAP AGENT # #
		else if (arg == "SWAP" && argc == 3)	{
			int a1;
			int a2;
			arg = PopReturnUpper(&commands);
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

		// UNSEAT #
		else if (arg == "UNSEAT" && argc == 2) {
			arg = PopReturnUpper(&commands);

			if (arg == "ALL") {
				for (unsigned j = 0; j < Environment::allAgents.size(); j++)
					Environment::allAgents.at(j)->UnseatAgent();

				printf ("All agents have been unseated.\n");
			}

			else {
				int a;
				if (!(istringstream(arg) >> a)) a = -1;

				if (a < 0 || a > Environment::agentID) {
					InterpretError();
				}

				Agent* g = Environment::allAgents.at(a);

				if (g->currentTable == NULL)
				{
					printf ("Agent %d is already unseated!\n", g->id);
				}

				g->UnseatAgent();
				printf ("Agent %d has been unseated.\n", g->id);
			}
		}

		// MOVE # to #
		else if (arg == "MOVE" && argc == 4) {
			int num;
			Agent* a;
			TableObj* t;

			arg = PopReturnUpper(&commands);
			if (!(istringstream(arg) >> num)) num = -1;

			if (num < 0 || num > Environment::agentID) {
				InterpretError();
			}
			a = Environment::allAgents.at(num);

			arg = PopReturnUpper(&commands);
			if (arg != "TO") {
				InterpretError();
			}

			arg = PopReturnUpper(&commands);
			if (!(istringstream(arg) >> num)) num = -1;

			if (num < 0 || num > Environment::tableID) {
				InterpretError();
			}
			t = Environment::allTables.at(num);

			if (t->numAtTable >= t->capacity) {
				printf ("ERROR: Table %d is already full!\n", t->id);
			}
			else {
				if (a->currentTable != NULL)
					a->UnseatAgent();

				t->AddToTable(a);

				printf ("Agent %d has been moved to table %d\n", a->id, a->currentTable->id);
			}
		}

		else if (arg == "SOCIAL" && argc == 1)
		{
			printf ("---Total Utility for All Agents: %d (%0.3f normalized)---",
					Environment::CalcSocialUtility(), Environment::CalcSocialUtilityNorm());
		}

		else {
			InterpretError();
		}

	}
}
