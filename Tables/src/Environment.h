#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <vector>
#include "Agent.h"
#include "TableObj.h"

class Environment {
public:
	static std::vector<Agent*> allAgents;	// Vector of all agents in the environment
	static std::vector<TableObj*> allTables;	// Vector of all tables in the environment

	static int tableID;
	static int agentID;

	static int CalcSocialUtility();	// Sums utility for all agents
	static double CalcSocialUtilityNorm();	// Normalized value for social utility
	static int NewAgentID();		// Returns a new agent ID number
	static int NewTableID();		// Returns a new table ID number
	static int RandInt(int x);		// Returns random integer from 0 to x

	static void AddAgent(Agent* a);
	static void AddTable(TableObj* t);

	static int EnvOpenSeats();

	static const int NUM_AGENTS = 20;
	static const int NUM_TABLES = 5;
	static const int PERSONALITY_TYPES = 10;
	static const int TABLE_CAPACITY = 10;

	static const bool USE_NORMALIZED_UTILITY = true;
	static const bool ALLOW_SWAPS = false;		// NOT implemented yet

	// Boolean switches for debug
	static const bool D_CONSTRUCTORS = false;
	static const bool D_ENVIRONMENT = false;
	static const bool D_SEARCHTABLES = true;
	static const bool D_MOVETABLES = true;
	static const bool D_CALCUTIL = false;
};

#endif /* ENVIRONMENT_H_ */
