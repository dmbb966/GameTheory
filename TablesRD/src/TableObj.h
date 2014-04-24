#ifndef TABLEOBJ_H_
#define TABLEOBJ_H_

class Agent;

#include "Agent.h"
#include <vector>

class TableObj {
public:
	TableObj();
	TableObj(int capacity);

	int id;				// unique ID number for the table
	int capacity;		// max number of agents at a table
	int numAtTable;		// current number of agents at the table

	void DisplayTableInfo();

	int OpenSeats();

	std::vector<Agent*> AgentList;	// List of agents at the table

	// int CalcUtilAtTable();	// Returns total utility of all agents at the table

	void AddToTable(Agent* newArrival);	// Add newArrival to the table
	void RemoveFromTable(Agent* departed);	// Remove departed from the table
};

#endif /* TABLE_H_ */
