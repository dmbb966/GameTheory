#include <stdio.h>

#include "TableObj.h"
#include "Environment.h"

using namespace std;

TableObj::TableObj()
{
	this->id = Environment::NewTableID();
	this->capacity = Environment::tableCapacity;
	this->numAtTable = 0;

	if (Environment::D_CONSTRUCTORS)
		printf ("Adding new empty table, ID %d and capacity %d.\n",
				this->id, this->capacity);
}

void TableObj::DisplayTableInfo()
{
	printf ("Table ID %d, with %d of %d people seated.", this->id, this->numAtTable, this->capacity);

	if (this->numAtTable > 0)
	{
		int tableUtility = 0;
		for (unsigned i = 0; i < this->AgentList.size(); i++)
			tableUtility += AgentList.at(i)->CalcUtilWith(this);
		double tableUtilityNorm = double(tableUtility) / this->numAtTable;

		printf ("  Table utility: %d, normalized to %0.3f: \n", tableUtility, tableUtilityNorm);
		for (unsigned i = 0; i < this->AgentList.size(); i++)
		{
			printf ("   ");
			this->AgentList.at(i)->DisplayAgentInfo();
		}
	}
	else printf ("\n");


}

// Add newArrival to the table
void TableObj::AddToTable(Agent* newArrival)
{
	if (this->numAtTable < this->capacity)
	{
		if (newArrival->currentTable != NULL)
		{
			//if (Environment::D_MOVETABLES)
			//	printf ("Agent %d is currently seated.  Unseating agent.\n", newArrival->id);
			newArrival->UnseatAgent();
		}

		AgentList.push_back(newArrival);
		newArrival->currentTable = this;
		this->numAtTable++;

		if (Environment::D_MOVETABLES)
			printf ("Agent %d is now at table %d (%d of %d people seated)\n",
					newArrival->id, this->id, this->numAtTable, this->capacity);
	}
	else
		printf ("ERROR!  Tried to move agent %d to table %d, but table already has %d of %d people seated!\n",
					newArrival->id, this->id, this->numAtTable, this->capacity);
}

void TableObj::RemoveFromTable(Agent* departed)
{
	for (unsigned i = 0; i < this->AgentList.size(); i++)
	{
		Agent* list = this->AgentList.at(i);
		if (list->id == departed->id)
		{
			this->AgentList.erase(this->AgentList.begin() + i);
			this->numAtTable--;
			departed->currentTable = NULL;

			//if (Environment::D_MOVETABLES)
			//	printf ("Successfully removed agent %d from table %d\n", departed->id, this->id);

			return;
		}
	}
	if (Environment::D_MOVETABLES)
		printf ("ERROR: Unable to locate agent %d at table %d\n", departed->id, this->id);
}

int TableObj::OpenSeats()
{
	return this->capacity - this->numAtTable;
}


/*
// Returns total utility of all agents at the table
int TableObj::CalcUtilAtTable()
{
	int result = 0;
	int utilcalc;

	if (this->numAtTable == 0)
	{
		if (Environment::D_CALCUTIL)
			printf ("Calc util of table %d is 0 because table is empty!\n", this->id);
		return 0;
	}

	for (int i = 0; i < this->AgentList.size(); i++)
	{
		utilcalc = this->AgentList.at(i).CalcUtilWith();
		result += utilcalc;

		if (Environment::D_CALCUTIL)
			printf ("Utility of table %d: Agent %d has %d utility.\n", this->id, this->AgentList.at(i).id, utilcalc);
	}

	if (Environment::D_CALCUTIL)
		printf ("Total utility of table %d is: %d\n", this->id, result);

	return result;
}

// Add newArrival to the table
void TableObj::AddToTable(Agent newArrival)
{
	if (this->numAtTable >= this->capacity)	{
		printf ("ERROR: Trying to move agent %d to table %d when table has %d of %d people!\n",
				newArrival.id, this->id, this->numAtTable, this->capacity);
	}
	else {
		this->numAtTable ++;
		//FIXME
		//newArrival.MoveTo(this);
		//newArrival.currentTable = NULL;
		//newArrival.currentTable = *this;
		this->AgentList.push_back(newArrival);

		if (Environment::D_MOVETABLES)
			printf ("Agent %d has been moved to table %d.  Table now has %d of %d people.\n",
					newArrival.id, this->id, this->numAtTable, this->capacity);
	}
}

// Remove departed from the table
void TableObj::RemoveFromTable(int departed)
{
	Agent a;

	if (this->numAtTable == 0)
		printf ("ERROR: Trying to remove agent %d from table %d when table is empty!\n", departed, this->id);
	else
	{
		for (int i = 0; i < this->AgentList.size(); i++)
		{
			a = this->AgentList.at(i);
			if (a.id == departed)
			{
				this->AgentList.erase(this->AgentList.begin() + i);

				if (Environment::D_MOVETABLES)
					printf ("Agent %d has been found at table %d and is being removed.\n", departed, this->id);
			}
		}
	}
}
*/
