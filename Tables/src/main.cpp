#include <stdio.h>

#include "mtwist.h"
#include "environment.h"
#include "Agent.h"
#include "TableObj.h"

int main()
{
	mt_seed32new(mt_seed());

	// Initialize starting ID numbers
	Environment::tableID = 0;
	Environment::agentID = 100;

	if (Environment::D_CONSTRUCTORS)
		printf ("Environment initialized with starting table ID %d and starting agent ID %d\n", Environment::tableID, Environment::agentID);

	Agent* new_agent;
	TableObj* new_table;

	for (int i = 0; i < Environment::NUM_AGENTS; i++)
	{
		new_agent = new Agent();
		Environment::AddAgent(new_agent);
	}

	for (int i = 0; i < Environment::NUM_TABLES; i++)
	{
		new_table = new TableObj();
		Environment::AddTable(new_table);
	}

	Environment::EnvOpenSeats();

	printf ("\n\nAdding agents to tables.\n");
	for (unsigned i = 0; i < Environment::allAgents.size(); i++)
	{
		Agent* agentForAssignment = Environment::allAgents.at(i);
		agentForAssignment->SeatRandomly();

		//int tableAssignment = Environment::RandInt(Environment::NUM_TABLES);
		//Environment::allTables.at(tableAssignment)->AddToTable(agentForAssignment);
	}

	printf ("\n\nDisplaying table info list.\n");
	for (unsigned i = 0; i < Environment::allTables.size(); i++)
		Environment::allTables.at(i)->DisplayTableInfo();

	// Environment::EnvOpenSeats();

	//for (unsigned i = 0; i < Environment::allAgents.size(); i++)
	//	Environment::allAgents.at(i)->DisplayAgentUtility();


	printf ("\n\n---Total Utility for All Agents: %d (%0.3f normalized)---\n\n",
			Environment::CalcSocialUtility(), Environment::CalcSocialUtilityNorm());


	for (int a = 0; a < 20; a++)
		for (unsigned i = 0; i < Environment::allAgents.size(); i++)
		{
			if (Environment::USE_NORMALIZED_UTILITY)
				Environment::allAgents.at(i)->FindBetterTableNorm();
			else
				Environment::allAgents.at(i)->FindBetterTable();
		}



	printf ("\n\n---Total Utility for All Agents: %d (%0.3f normalized)---\n\n",
				Environment::CalcSocialUtility(), Environment::CalcSocialUtilityNorm());


	printf ("\n\nDisplaying table info list.\n");
	for (unsigned i = 0; i < Environment::allTables.size(); i++)
		Environment::allTables.at(i)->DisplayTableInfo();

	printf ("\n\nRemoving agents from tables.\n");
	for (unsigned i = 0; i < Environment::allAgents.size(); i++)
	{
		//Environment::allAgents.at(i)->DisplayAgentInfo();

		Agent* a = Environment::allAgents.at(i);

		a->UnseatAgent();
		/*

		TableObj* t = a->currentTable;

		if (t == NULL)
			printf ("Agent %d is not sitting at a table!\n", a->id);
		else
		{
			t->RemoveFromTable(a);
		}*/
	}

	printf ("\n\nDisplaying table info list.\n");
	for (unsigned i = 0; i < Environment::allTables.size(); i++)
		Environment::allTables.at(i)->DisplayTableInfo();

	return 0;
}
