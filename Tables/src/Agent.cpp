#include <stdio.h>
#include <stdlib.h>

#include "Agent.h"
#include "Environment.h"

using namespace std;

Agent::Agent()
{
	this->id = Environment::NewAgentID();
	this->personality = Environment::RandInt(Environment::PERSONALITY_TYPES);
	this->currentTable = NULL;

	if (Environment::D_CONSTRUCTORS)
		printf ("Adding new agent, ID %d and personality %d.\n",
				this->id, this->personality);
}

void Agent::DisplayAgentInfo()
{
	printf ("Agent %d: Personality %d.  ", this->id, this->personality);
	if (this->currentTable == NULL)
		printf ("Not currently seated at a table.\n");
	else
		printf ("Currently seated at table %d\n", this->currentTable->id);
}

void Agent::DisplayAgentUtility()
{
	printf ("Total utility for agent %d (personality %d) is: %d.  Normalized %0.3f\n",
			this->id, this->personality, this->CalcUtilWith(), this->CalcUtilNorm());
}

// Leaves the table and goes back to the ranks of the unsitted.
void Agent::UnseatAgent()
{
	if (this->currentTable == NULL)
		printf ("Tried to unseat agent %d, but he is already standing!\n", this->id);
	else
	{
		this->currentTable->RemoveFromTable(this);
	}
}

// Agent tries to sit at a random table
void Agent::SeatRandomly()
{
	if (Environment::EnvOpenSeats() <= 0)
	{
		printf ("Agent %d cannot find a seat!\n", this->id);
		return;
	}
	else
	{
		do {
			int targetTable = Environment::RandInt(Environment::NUM_TABLES);
			Environment::allTables.at(targetTable)->AddToTable(this);
		} while (this->currentTable==NULL);
	}

}

// Returns projected utility if agent with a given personality is
// added to the table
// TODO: Add personality matrix as an additional argument
int Agent::CalcUtilWith(TableObj* t)
{
	int result = 0;
	int utilcalc = 0;

	for (unsigned i = 0; i < t->AgentList.size(); i++)
	{
		Agent* a = t->AgentList.at(i);

		if (a->id == this->id) {
			if (Environment::D_CALCUTIL)
				printf ("Util calc for Agent %d: Comparison with self, no utility added.\n", this->id);
		}
		else {
			utilcalc = CalcUtilWith(a);

			if (Environment::D_CALCUTIL)
				printf ("Util calc for Agent %d (personality %d) with Agent %d (personality %d).  Utility: %d\n",
						this->id, this->personality, a->id, a->personality, utilcalc);
		}

		result += utilcalc;
	}

	if (Environment::D_CALCUTIL)
		printf ("Util calc for Agent %d: --- Total Utility is %d ---\n", this->id, result);

	return result;
}

// Returns utility of working with agent a
int Agent::CalcUtilWith(Agent* a)
{
	return Environment::PERSONALITY_TYPES - abs(a->personality - this->personality);
}

// Returns utility of current table
int Agent::CalcUtilWith()
{
	TableObj* t = this->currentTable;

	if (t == NULL)
	{
		printf ("Agent %d is not seated!\n", this->id);
		return 0;
	}
	else
		return CalcUtilWith(t);
}

// Calculates utility for current table, normalized for number of agents present
double Agent::CalcUtilNorm()
{
	return double(CalcUtilWith()) / this->currentTable->numAtTable;
}

double Agent::CalcUtilNorm(TableObj* t)
{
	return double(CalcUtilWith(t)) / t->numAtTable;
}



void Agent::FindBetterTable()
{
	int currentUtil = CalcUtilWith();
	int currentTableID = this->currentTable->id;

	// Searches for any table with higher potential utility than current
	for (unsigned i = 0; i < Environment::allTables.size(); i++)
	{
		TableObj* t = Environment::allTables.at(i);
		if (t->id != this->currentTable->id && t->OpenSeats() > 0)
		{
			int comparisonUtil = CalcUtilWith(t);

			if (comparisonUtil > currentUtil)
			{
				t->AddToTable(this);

				if (Environment::D_SEARCHTABLES && comparisonUtil > currentUtil)
				{
					printf ("Agent %d can move from table %d to table %d.  ", this->id, currentTableID, t->id);
					printf ("Will increase utility by %d\n", comparisonUtil - currentUtil);
				}

				return;
			}
		}
	}
}

void Agent::FindBetterTableNorm()
{
	double currentUtil = CalcUtilNorm();
	int currentTableID = this->currentTable->id;


	// Searches for any table with higher potential utility than current
	for (unsigned i = 0; i < Environment::allTables.size(); i++)
	{
		TableObj* t = Environment::allTables.at(i);
		if (t->id != this->currentTable->id && t->OpenSeats() > 0)
		{
			double comparisonUtil = CalcUtilNorm(t);

			if (comparisonUtil > currentUtil)
			{
				t->AddToTable(this);

				if (Environment::D_SEARCHTABLES && comparisonUtil > currentUtil)
				{
					printf ("Agent %d can move from table %d to table %d.  ", this->id, currentTableID, t->id);
					printf ("Will increase utility by %0.3f\n", comparisonUtil - currentUtil);
				}

				return;
			}
		}
	}
}

