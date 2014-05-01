#include <stdio.h>
#include <stdlib.h>

#include "Agent.h"
#include "Environment.h"

using namespace std;


DistMapEntry::DistMapEntry(Agent* a, int dist)
{
	this->a = a;
	this->dist = dist;

	if (Environment::D_CONSTRUCTORS)
		printf ("Adding new DistanceMap Entry for agent ID %d at distance %d.\n",
				this->a->id, this->dist);
}

Agent::Agent()
{
	this->id = Environment::NewAgentID();
	this->currentTable = NULL;

	if (Environment::ADVANCED_MODE) {
		this->personality = 5;
	}
	else {
		this->personality = Environment::RandInt(Environment::personalityTypes);
	}


	if (Environment::D_CONSTRUCTORS)
		printf ("Adding new agent, ID %d and personality %d.\n",
				this->id, this->personality);
}

Agent::Agent(int personality) {
	this->id = Environment::NewAgentID();

	if (personality < 0 || personality >= Environment::personalityTypes) {
		printf ("ERROR: Cannot add agent with personality %d.  Range must be between 0 and %d\n", personality, Environment::personalityTypes - 1);
		printf ("Adding agent with random personality type instead.\n");

		this->personality = Environment::RandInt(Environment::personalityTypes);
	}
	else this->personality = personality;
	this->currentTable = NULL;

	if (Environment::D_CONSTRUCTORS)
		printf ("Adding new agent, ID %d and personality %d.\n",
		this->id, this->personality);
}

void Agent::DisplayNeighbors() {
	if (this->neighbors.size() == 0)
		printf ("No neighbors.");
	else {
		printf ("Neighbors with ");
		for (unsigned i = 0; i < this->neighbors.size(); i++) {
			printf ("%d ", this->neighbors.at(i)->id);
		}
	}
}

void Agent::SwapWith(Agent* a)
{
	if (this->currentTable == NULL)
	{
		if (a->currentTable == NULL) {
			if (Environment::D_MOVETABLES)
				printf ("Swapping agent %d with %d does nothing.  Both are unseated.\n", this->id, a->id);

			return;
		}
		else {
			TableObj* t = a->currentTable;
			a->UnseatAgent();
			t->AddToTable(this);

			if (Environment::D_MOVETABLES)
				printf ("Unseated agent %d has swapped with agent %d at table %d.\n", this->id, a->id, this->currentTable->id);
		}
	}

	else if (a->currentTable == NULL) {
		TableObj* t = this->currentTable;
		this->UnseatAgent();
		t->AddToTable(a);
			if (Environment::D_MOVETABLES)
				printf ("Agent %d at table %d has swapped with unseated agent %d.\n", this->id, a->currentTable->id, a->id);
	}

	else if (this->currentTable->id == a->currentTable->id){
		if (Environment::D_MOVETABLES)
			printf ("Swapping agent %d with %d does nothing.  Both are at the same table.\n", this->id, a->id);

		return;
	}

	else
	{
		TableObj* t1 = this->currentTable;
		TableObj* t2 = a->currentTable;

		this->UnseatAgent();
		a->UnseatAgent();

		t1->AddToTable(a);
		t2->AddToTable(this);

		if (Environment::D_MOVETABLES)
			printf ("Swapping Agent %d is now at table %d, while agent %d is now at table %d.\n", this->id, this->currentTable->id, a->id, a->currentTable->id);
	}
}

void Agent::DisplayAgentInfo()
{
	if (!Environment::ADVANCED_MODE) {
		printf ("Agent %d, Personality %d, is", this->id, this->personality);
	}
	else {
		printf ("Agent %d.  ", this->id);
		DisplayNeighbors();
	}

	if (this->currentTable == NULL)
		printf (" Unseated.  Utility is 0.\n");
	else
	{
		printf (" At table %d.  ", this->currentTable->id);
		if (Environment::D_CALCUTIL)
			printf ("\n");
		this->DisplayAgentUtility();
	}

}

void Agent::DisplayAgentUtility()
{
	printf ("Total utility is: %d, normalized to %0.3f\n",
			this->CalcUtilWith(), this->CalcUtilNorm());
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
			int targetTable = Environment::RandInt(Environment::allTables.size());
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

	for (unsigned i = 0; i < t->AgentList.size(); i++)
	{
		int utilcalc = 0;

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
	return Environment::personalityTypes - abs(a->personality - this->personality) - Environment::lonerPreference;
}

// Returns utility of current table
int Agent::CalcUtilWith()
{
	TableObj* t = this->currentTable;

	if (t == NULL)
		return 0;
	else
		return CalcUtilWith(t);
}

// Calculates utility for current table, normalized for number of agents present
double Agent::CalcUtilNorm()
{
	if (this->currentTable == NULL || this->currentTable->numAtTable <= 1)
		return 0;
	else
		return double(CalcUtilWith()) / (this->currentTable->numAtTable - 1);
}

double Agent::CalcUtilNorm(TableObj* t)
{
	if (this->currentTable->id == t->id)
		if (t->numAtTable <= 1)
			return 0.0;
		else
			return double(CalcUtilWith(t)) / (t->numAtTable - 1);
	else
		if (t->numAtTable < 1) return 0.0;
		else return double(CalcUtilWith(t)) / (t->numAtTable);
}



void Agent::FindBetterTable()
{
	int currentUtil = CalcUtilWith();

	if (this->currentTable == NULL) {
		if (Environment::seatOnStep) this->SeatRandomly();
		if (this->currentTable == NULL) return;
	}

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
				if (Environment::D_SEARCHTABLES && comparisonUtil > currentUtil)
				{
					printf ("Agent %d can move from table %d to table %d.  ", this->id, currentTableID, t->id);
					printf ("Will increase utility by %d\n", comparisonUtil - currentUtil);
				}

				t->AddToTable(this);

				return;
			}
		}
	}
}

void Agent::FindBetterTableNorm()
{
	double currentUtil = CalcUtilNorm();

	if (this->currentTable == NULL) {
		if (Environment::seatOnStep) this->SeatRandomly();
		if (this->currentTable == NULL) return;
	}

	int currentTableID = this->currentTable->id;


	// Searches for any table with higher potential utility than current
	for (unsigned i = 0; i < Environment::allTables.size(); i++)
	{
		TableObj* t = Environment::allTables.at(i);
		if (t->id != this->currentTable->id && t->OpenSeats() > 0)
		{
			double comparisonUtil = CalcUtilNorm(t);

			if (Environment::D_CALCUTIL) {
				printf ("Agent %d current utility at table is %0.3f versus table %d is %0.3f\n", this->id, currentUtil, t->id, comparisonUtil);
			}

			if (comparisonUtil > currentUtil)
			{
				if (Environment::D_SEARCHTABLES && comparisonUtil > currentUtil)
				{
					printf ("Agent %d can move from table %d to table %d.  ", this->id, currentTableID, t->id);
					printf ("Will increase utility by %0.3f\n", comparisonUtil - currentUtil);
				}

				t->AddToTable(this);

				return;
			}
		}
	}
}

// Finds the distance between the parent and the target
int Agent::CalcDistanceWith(Agent* target) {
	Agent* origin = this;
	int distance = 999;

	// Checks all agents within distance map to see if entry already exists

	printf ("Searching all neighbors within distance 3.\n");

	for (unsigned i = 0; i < origin->distMap.size(); i++)
	{
		Agent* a = origin->distMap.at(i)->a;
		if (a->id == target->id) {
			printf ("Found target agent %d at distance %d\n", target->id, origin->distMap.at(i)->dist);
			return origin->distMap.at(i)->dist;
		}
	}

	printf ("Unable to find within distance 3.  Attempting backward search from target.\n");

	// Failing this, uses the target's distance map and checks for a common connection.

	for (unsigned i = 0; i < target->distMap.size(); i++)
	{
		DistMapEntry* dmA = target->distMap.at(i);
		if (dmA->dist >= 3) continue;	// Redundant since 3 from origin and 2 from target connects

		for (unsigned j = 0; j < origin->distMap.size(); j++){
			DistMapEntry* dmB = origin->distMap.at(j);

			if (dmA->a->id == dmB->a->id) {
				int newDist = dmA->dist + dmB->dist;
				printf ("Found target agent %d at distance %d.  Searching for shorter paths.\n", target->id, newDist);
				if (newDist < distance) distance = newDist;
				// Continues checking in case there is a closer neighbor
			}
		}
	}

	if (distance == 999) printf ("Did not find connection between agents %d and %d\n", origin->id, target->id);
	else printf ("Connection found between agents %d and %d at range %d\n", origin->id, target->id, distance);
	return distance;
}

void Agent::RecursiveDist(vector<DistMapEntry*> stack, Agent* parent, Agent* target, int curLevel, int maxLevel) {
	if (curLevel > maxLevel) return;

	printf ("%d -> %d, level %d of %d with %u neighbors\n", parent->id, target->id, curLevel, maxLevel, target->neighbors.size());

	for (unsigned i = 0; i < target->neighbors.size(); i++) {
		printf ("Begin %d of %d\n\n", i, target->neighbors.size());
		if (target->neighbors.at(i)->id == parent->id) {
			continue;
		}
		else {
			printf (" ID %d: ", target->neighbors.at(i)->id);

			DistMapEntry* d = new DistMapEntry(target->neighbors.at(i), curLevel);
			printf ("  Dist %d.\n", curLevel);
			stack.push_back(d);
			curLevel++;
			Agent::RecursiveDist(stack, target, target->neighbors.at(i), curLevel, maxLevel);
			printf ("Done with recursive call.\n");
		}
	}
}


void Agent::CalcLinks() {
	this->distMap.clear();
	printf ("Old distance map cleared.  Recalculating links.\n");
	Agent::RecursiveDist(this->distMap, this, this, 1, 3);
}

void Agent::AddLink(Agent* a) {
	for (unsigned i = 0; i < this->neighbors.size(); i++) {
		if (a->id == this->neighbors.at(i)->id) {
			printf ("ERROR: Agent %d is already a neighbor of agent %d!\n", a->id, this->id);
			return;
		}

		if (a->id == this->id) {
			printf ("ERROR: You cannot link an agent to itself!\n");
			return;
		}
	}

	this->neighbors.push_back(a);
	a->neighbors.push_back(this);
	printf ("Added Agent %d as a neighbor to agent %d and vice versa.\n", a->id, this->id);
}

void Agent::RemoveLink(Agent* a) {
	for (unsigned i = 0; i < this->neighbors.size(); i++) {
		if (a->id == this->neighbors.at(i)->id) {
			this->neighbors.erase(this->neighbors.begin()+i);

			printf ("Removed link between Agent %d and Agent %d\n", this->id, a->id);
			i = this->neighbors.size();
		}
	}

	for (unsigned i = 0; i < a->neighbors.size(); i++) {
		if (this->id == a->neighbors.at(i)->id) {
			a->neighbors.erase(a->neighbors.begin()+i);

			printf ("Removed link between Agent %d and Agent %d\n", a->id, this->id);
			return;
		}
	}
}
