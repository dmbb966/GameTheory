#ifndef AGENT_H_
#define AGENT_H_

class TableObj;

#include <vector>
#include "TableObj.h"

class DistMapEntry {
public:
	DistMapEntry(Agent* a, int distance);

	Agent* a;
	int dist;
};

class Agent {
public:
	Agent();
	Agent(int personality);

	int id;								// Unique agent ID

	std::vector<Agent*> neighbors;	// Links to neighboring agents
	std::vector<DistMapEntry*> distMap;
	//DistanceMap* distMap;

	TableObj* currentTable;	// Where he is currently seated

	// Will remove personality
	int personality;	// Personality code - used to compute compatibility



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

	void AddLink(Agent* a);
	void RemoveLink(Agent* a);
	void CalcLinks();
	int CalcDistanceWith(Agent *target);

private:
	void DisplayAgentUtility();
	void DisplayNeighbors();
	static void RecursiveDist(std::vector<DistMapEntry*> stack, Agent* parent, Agent* target, int curLevel, int maxLevel);



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
