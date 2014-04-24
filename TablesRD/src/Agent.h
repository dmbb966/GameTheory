#ifndef AGENT_H_
#define AGENT_H_

class TableObj;

#include "TableObj.h"

class Agent {
public:
	Agent();
	Agent(int personality);

	int id;				// Unique agent ID
	int personality;	// Personality code - used to compute compatibility

	TableObj* currentTable;	// Where he is currently seated

	void DisplayAgentInfo();
	void UnseatAgent();
	void SeatRandomly();

	double CalcUtilNorm();				// Calculates utility for current table, normalized for number of agents present
	double CalcUtilNorm(TableObj* t);
	int CalcUtilWith();				// Calculates utility for current table
	int CalcUtilWith(Agent* a);		// Calculates individual utility with another agent
	int CalcUtilWith(TableObj* t);	// Calcualtes agent's utility at table t

	void FindBetterTable();
	void FindBetterTableNorm();

	void SwapWith(Agent* a);		// Swaps places with agent a

private:
	void DisplayAgentUtility();



	// TODO: Implement personality matrix for some randomization to compatibility



	// TableObj* SearchTables();			// Searches tables for one with greater utility
	// void MoveTo(TableObj* destination);	// Move to the designated table

	// Returns projected utility if agent with a given personality is
	// at table t, or computes individual utility with agent a
	// TODO: Add personality matrix as an additional argument
	// int CalcUtilWith(TableObj* t);
	// int CalcUtilWith(Agent* a);
	// int CalcUtilWith();				// Calculates for current table

	// TODO: Implement Swap Tables and Search Swap
};



#endif /* AGENT_H_ */
