#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Agent.h"
#include "TableObj.h"
#include <vector>

class Environment {
public:
	static std::vector<Agent*> allAgents;	// Vector of all agents in the environment
	static std::vector<TableObj*> allTables;	// Vector of all tables in the environment

	static int tableID;
	static int agentID;

	static int numAgents;
	static int numTables;
	static int personalityTypes;
	static int tableCapacity;
	static int lonerPreference;

	static bool seatOnStep;
	static bool initialized;

	static bool D_CALCUTIL;

	static int CalcSocialUtility();	// Sums utility for all agents
	static double CalcSocialUtilityNorm();	// Normalized value for social utility
	static int NewAgentID();		// Returns a new agent ID number
	static int NewTableID();		// Returns a new table ID number
	static int RandInt(int x);		// Returns random integer from 0 to x

	static void AddAgent();
	static void AddAgent(int personality);
	static void AddTable();
	static void AddTable(int capacity);

	static int EnvOpenSeats();

	static void Initialize();

	static const bool USE_NORMALIZED_UTILITY = true;
	static const bool ALLOW_SWAPS = false;		// NOT implemented yet

	// Boolean switches for debug
	static const bool D_CONSTRUCTORS = false;
	static const bool D_ENVIRONMENT = false;
	static const bool D_SEARCHTABLES = true;
	static const bool D_MOVETABLES = true;
};

#endif /* ENVIRONMENT_H_ */
