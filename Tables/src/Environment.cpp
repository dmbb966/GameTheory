#include "Environment.h"
#include "mtwist.h"

int Environment::agentID;
int Environment::tableID;
int Environment::numAgents;
int Environment::numTables;
int Environment::personalityTypes;
int Environment::tableCapacity;
int Environment::lonerPreference;

bool Environment::seatOnStep;
bool Environment::initialized;

bool Environment::D_CALCUTIL;
std::vector<Agent*> Environment::allAgents;
std::vector<TableObj*> Environment::allTables;

void Environment::AddAgent()
{
	Agent* a = new Agent();
	allAgents.push_back(a);

	if (D_ENVIRONMENT)
		printf ("Pushed agent %d onto all agents list. Vector size is now %d\n", a->id, allAgents.size());
}

void Environment::AddTable()
{
	TableObj* t = new TableObj();
	allTables.push_back(t);

	if (D_ENVIRONMENT)
		printf ("Pushed table %d onto all tables list.  Vector size is now %d\n", t->id, allTables.size());
}

// Sums utility for all agents
int Environment::CalcSocialUtility()
{
	int result = 0;

	for (unsigned i = 0; i < allAgents.size(); i++)
		result += allAgents.at(i)->CalcUtilWith();

	return result;
}

double Environment::CalcSocialUtilityNorm()
{
	int result = 0;

	for (unsigned i = 0; i < allAgents.size(); i++)
		result += allAgents.at(i)->CalcUtilNorm();

	return result;
}

// Returns a new agent ID number
int Environment::NewAgentID()
{
	int newID = agentID;
	agentID++;

	return newID;
}

// Returns a new table ID number
int Environment::NewTableID()
{
	int newID = tableID;
	tableID++;

	return newID;
}


// Returns random integer from 0 to (x-1)
int Environment::RandInt(int x)
{
	return mt_drand() * x;
}


// Returns total number of empty seats in the environment
int Environment::EnvOpenSeats()
{
	int result = 0;

	for (unsigned i = 0; i < allTables.size(); i++)
		result += allTables.at(i)->OpenSeats();

	if (D_ENVIRONMENT)
		printf ("Total number of empty seats in the environment: %d\n", result);

	return result;
}
