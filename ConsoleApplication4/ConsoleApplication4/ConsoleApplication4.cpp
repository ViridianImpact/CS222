/*
Programmer: Nicholas Provencal
Purpose: Created a functioning POS for a bike shop keeping and inventory of bikes using a linked list to maintain and provide service.
I could organize the printing option for how to print by sending a different head depending on what order I wanted to be printed, this made the order enum irrelevant/redundant. 
In order to delete a specific node properly I needed to find its place in the other lists and make sure to properly organize its position before deleting it. 
By passing a pointer by reference for each of the heads I was able to maintain their relationship with every node for every list, even when transferring back and forth from the function.
*/

#include "stdafx.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>

using namespace std;

#ifndef _BIKES_H_
#define _BIKES_H_

enum status { NO_STATUS, NOT_RENTED, RENTED, ALL };
enum order { NO_ORDER, ID, MANUF };

struct Bike
{
	char manufact[25]; //name of manufacturer
	int id_num; //ID number
	status rented_code; //RENTED/NOT_RENTED
	char to_whom[25]; //to whom bike is rented
	int size; //size of engine
	float cost_per_day; //cost for every day of use
	bool deleted; //to mark bike as deleted in the list.
	Bike *prev; //pointer to the previous node in the general list
	Bike *prev_manuf; //pointer to previous node in the manufacturers list
	Bike *prev_id; //pointer to the previous node in the list by ID
	Bike *next_manuf; //pointer to next node in the
					  //manufacturers list
	Bike *next_id; //pointer to the next node
				   //in the list by ID
	Bike *next; //pointer to the next node in the general list
};

//Functions:
/*readBike:-
Purpose: reads bikes from the file, creates new Bike object, and calls functions to add bike to the
*general list, to the list sorted by id, and to the list sorted by the manufacturer.
*Parameters - handle to the input file, and head pointers to the three lists.*/
Bike * readBike(ifstream & inFile, Bike *& head, Bike *& manuf, Bike *& id);

/*addBike:-
*Purpose: adds bike to the general list
*Parameters: pointer to the first node in the list, pointer to the new node
*Returns pointed to the first node in the list.
*This function is overloaded with two other functions below
*/
Bike * addBike(Bike* head, Bike* new_bike);

/*
*addBike:
*Purpose: Overloaded function to add bike to the list, sorted by ID.
*Note, that you have only one set of nodes. You will need to implement different
*lists by different sets of pointers.
*/
Bike * addBike(Bike * head_id, Bike * new_bike, order);

/*
*addBike:
*Purpose: Overloaded function to add bike to the list, sorted by the manufacturer.
*Note that the last parameter is just a placeholder to tell compiler to call this
*overloaded function. you can pass any value there, it is not used in the function.
*/
Bike * addBike(Bike * head_manufacturer, Bike * new_bike, order, int);

/*
*printBike:-
*Purpose: Prints any list of bikes, that have not been deleted, depending on the
*status, and the head pointer passed. If status is RENTED, you print only bikes
*that are rented. If status is ALL, you print the entire list of (not deleted)
*bikes. Note, that depending on the list, you pointer assignments should be
*different.
*The last part is not applicable if you choose not to implement sorted list
*Parameters:
*Bike* - pointer to the first node of the list of bikes.
*order - needed to control pointer re-assignment. I.e. if order == ID,
* you should move along the list, sorted by ID.
*status - Determines which bikes to print from the list. I.e. rented, not
* rented, or all.
*/
void printBikes(Bike*, order, status);

/*do_transact:-
*Purpose: Reads the ID, number of days from the file, and depending on the status
*(RENTED/NOT_RENTED) either rents bike to the person (in which case you need to
* read person's name from the file, or performs return transaction. If transaction
*is rented, you should also display the total cost of the rent
*(number of days * cost of rent per day).
*in both cases, if status of bike does not correspond to the type of the
* transaction (i.e. you try to rent the bike which status is RENTED), you need to
* output an appropriate message and return.
*/
int do_transact(ifstream &, Bike * head, status);

/*
*del_id_bike:-
*Purpose: Finds the bike by the ID in the list and sets its deleted flag to true.
*Note, that you do not delete the node. You set its deleted flag to true.
*Parameters:
*fstream object to the file to read the bike's ID
*pointer to the head of the bike's list sorted by ID. If you did not implement
*the sorted list, pass there a pointer to the head of the general list.
*/
int del_id_bike(ifstream &, Bike*&, Bike*&, Bike*&);

/*
del_manuf:-
*Purpose: Finds the bike by the manufacturer's name in the list and sets its
*deleted flag to true.
*Parameters: as above.
*/
int del_manuf(ifstream &, Bike*&, Bike*&, Bike*&);

/*
deleteBikes:-
*Purpose: To traverse the general list of bikes and deletes all the nodes in
*it, releasing back the memory. It is called before your program exits.
*Note, that if you implemented any dynamically allocated memory in the node,
*you will need to release that memory first, before deleting the node itself.
*Parameters:
*pointer to the head of the general list of bikes.
*/
int deleteBikes(Bike *);

/*
*init:-
*Purpose: This function initializes the new Bike's object to all default values
*(char array should be initialized to ""
*/
void init(Bike*);

#endif /* _BIKES_H_*/

int main()
{
	Bike *head = NULL; //beginning of general list
	Bike *headID = NULL; //beginning of id list
	Bike *headMan = NULL; //beginning of manufact list
	int control = 1; //initialize control for switch statement
	ifstream inFile("prog_5_dat_file.dat"); //opens file

	if (!inFile.is_open()) //file couldn't be opened
	{
		cout << "Program could not open file!!";
		cin.get();
		return -1;
	}

	while (inFile >> control && !inFile.eof()) //creates control or ends program
	{
		switch (control)
		{
		case 1:
			readBike(inFile, head, headMan, headID);//creates linked lists
			break;
		case 2:
			printBikes(head, NO_ORDER, NO_STATUS); //print without order
			break;
		case 3:
			printBikes(headID, ID, NO_STATUS); //print in numerical order
			break;
		case 4:
			printBikes(headMan, MANUF, NO_STATUS); //print in alphabetical order
			break;
		case 5:
			printBikes(head, NO_ORDER, NOT_RENTED); // print only bike not rented
			break;
		case 6:
			printBikes(head, NO_ORDER, RENTED); // print only bike rented
			break;
		case 7:
			do_transact(inFile, head, NO_STATUS); //rent or retrun bike
			break;
		case 8:
			do_transact(inFile, head, RENTED); //rent or return bike
			break;
		case 9:
			del_id_bike(inFile, head, headID, headMan); // delete bike based on id
			break;
		case 10: 
			del_manuf(inFile, head, headID, headMan); // delete bike based on manufacturer
			break;
		default:
			break;
		}
	}

	inFile.close(); // closes file

	cin.get();

    return 0;
}

Bike * readBike(ifstream & inFile, Bike *& head,Bike *& headMan, Bike *& headID)
{
	Bike *bike = new Bike; //initializes a new object
	init(bike);

	inFile >> bike->id_num; //imports data from files
	inFile >> bike->size;
	inFile >> bike->cost_per_day;
	inFile >> bike->manufact;

	head = addBike(head, bike); //puts object into general list
	headID = addBike(headID, bike, NO_ORDER);
	headMan = addBike(headMan, bike, NO_ORDER, 0); //puts object into manufaturers list
	return head;
}

Bike * addBike(Bike * head, Bike * newBike)
{
	
	if (!head) //makes object head if none exists
	{
		head = newBike;
		return head;
	}
	else //places new object at end of list 
	{
		Bike *temp = head;
		while(temp->next != NULL)
		{
			temp = temp->next;
		}

		newBike->prev = temp;
		temp->next = newBike;
		return head;
	}
}

Bike * addBike(Bike * headID, Bike * newBike, order)
{
	int i = 0; //counter

	if (!headID) //makes the head if none exists
	{
		headID = newBike;
		return headID;
	}
	else //compares id numbers
	{
		Bike *temp = headID;
		while (temp != NULL)
		{
			if (newBike->id_num < temp->id_num) //new object is smaller
			{
				if (i == 0) //makes it the head
				{
					headID = newBike;
					newBike->next_id = temp;
					temp->prev_id = newBike;
					return headID;
				}
				else //places new object at this point
				{
					newBike->next_id = temp;
					newBike->prev_id = temp->prev_id;
					temp->prev_id->next_id = newBike;
					temp->prev_id = newBike;
					return headID;
				}
			}
			else if (temp->next_id == NULL)
			{
				temp->next_id = newBike;
				newBike->prev_id = temp;
				return headID;
			}

			temp = temp->next_id;
			i++;
		}
	}
}

Bike * addBike(Bike * headMan, Bike * newBike, order, int i)
{
	if (!headMan) //makes new object head if no head exists
	{
		headMan = newBike;
		return headMan;
	}
	else //compares the manfacturers
	{
		Bike *temp = headMan;
		
		while (temp != NULL)
		{
			if (strcmp(newBike->manufact, temp->manufact) < 0) //new object is earlier alphabetically
			{
				if (i == 0) //makes it the head
				{
					headMan = newBike;
					newBike->next_manuf = temp;
					temp->prev_manuf = newBike;
					return headMan;
				}
				else //places it inside the list at this spot
				{
					newBike->next_manuf = temp;
					temp->prev_manuf->next_manuf = newBike;
					temp->prev_manuf = newBike;
					return headMan;
				}
			}
			else if (strcmp(newBike->manufact, temp->manufact) == 0) //new object matches alphabetically
			{
				if (newBike->id_num < temp->id_num) //sorts by id by default
				{
					if (i == 0) //makes it the head
					{
						headMan = newBike;
						newBike->next_manuf = temp;
						temp->prev_manuf = newBike;
						return headMan;
					}
					else if (strcmp(newBike->manufact, temp->next_manuf->manufact) == 0)
					{
						break;
					}
					else //places it inside the list at this spot
					{
						newBike->next_manuf = temp;
						temp->prev_manuf->next_manuf = newBike;
						temp->prev_manuf = newBike;
						return headMan;
					}
				}
			}
			else if (temp->next_manuf == NULL)
			{
				temp->next_manuf = newBike;
				newBike->prev_manuf = temp;
				return headMan;
			}

			temp = temp->next_manuf;
			i++;
		}

		
	}
}

void printBikes(Bike * start, order o, status s) {
	Bike *temp = start;

	if (s == NO_STATUS) //unrented or rented bikes
	{
		if (o == NO_ORDER) //without order
		{
			while (temp != NULL)
			{
				if (!temp->deleted)
					printf("Id %d, man: %s\n", temp->id_num, temp->manufact);
				temp = temp->next;
			}

			cout << endl << "^^^ GENERAL ^^^" << endl << endl;
		}
		else if (o == ID) //numerical order
		{
			while (temp != NULL)
			{
				if (!temp->deleted)
					printf("Id %d, man: %s\n", temp->id_num, temp->manufact);
				temp = temp->next_id;
			}

			cout << endl << "^^^ SORTED BY ID ^^^" << endl << endl;
		}
		else //alphabetical order
		{
			while (temp != NULL)
			{
				if (!temp->deleted)
					printf("Id %d, man: %s\n", temp->id_num, temp->manufact);
				temp = temp->next_manuf;
			}

			cout << endl << "^^^ SORTED ALPHABETICALLY ^^^" << endl << endl;
		}
	}
	else {
		while (temp != NULL)
		{
			if (temp->rented_code == s && !temp->deleted) //prints based on status
			{
				printf("Id %d, man: %s\n", temp->id_num, temp->manufact);
			}
			temp = temp->next;
		}

		if (s == NOT_RENTED)
		{
			cout << endl << "^^^ NOT RENTED ^^^" << endl << endl;
		}
		else if (s == RENTED)
		{
			cout << endl << "^^^ RENTED ^^^" << endl << endl;
		}
	}
}

int do_transact(ifstream & inFile, Bike * head, status s)
{
	int id;
	float days;
	string str, str2;
	char name[50];
	Bike *temp = head;

	if (s == NO_STATUS) //performs rental
	{
		inFile >> id;
		inFile >> days;

		inFile >> str;
		inFile >> str2;
		strcpy_s(name, str.c_str());

		
		while (temp != NULL)
		{
			if (id == temp->id_num) // rents bike
			{
				cout << "The price of this rental will be: $" << (days)*(temp->cost_per_day) << endl;
				strcpy_s(temp->to_whom, name);
				temp->rented_code = RENTED;
				cout << "Thank you for your business!" << endl << endl;
				return 0;
			}

			temp = temp->next;
		}
	}

	if (s == RENTED) // performs return
	{
		inFile >> id;
		inFile >> days;
		
		while (temp != NULL)
		{
			if (id == temp->id_num) // returns bike
			{
				if (temp->rented_code != RENTED) // bike hasnt been rented
				{
					cout << "This Bike has not been rented yet!" << endl << endl;
					return 0;
				}
				else //bike has been rented
				{
					cout << "The price for renting this bike was: $" << (temp->cost_per_day)*(days) << endl;
					strcpy_s(temp->to_whom, "");
					temp->rented_code = NOT_RENTED;
					cout << "Thank you for your business!" << endl << endl;
					return 0;
				}
			}

			temp = temp->next;
		}
	}
}

int del_id_bike(ifstream & inFile, Bike *& head, Bike *& headID, Bike *& headMan)
{
	int id, i = 0;
	Bike *temp = head;
	
	inFile >> id;

	while (temp != NULL)
	{
		if (i == 0 && temp->id_num == id) //id is matched
		{
			if (temp != headID && temp != headMan) //if node is head of general only
			{
				head = temp->next;
				temp->next->prev = NULL;
				temp->next = NULL;

				if (temp->next_id != NULL) //node is tail of id
				{
					temp->next_id->prev_id = temp->prev_id;
					temp->prev_id->next_id = temp->next_id;
				}
				else
				{
					temp->prev_id->next_id = NULL;
				}

				temp->next_id = NULL;
				temp->prev_id = NULL;

				if (temp->next_manuf != NULL) // node is tail of manuf
				{
					temp->next_manuf->prev_manuf = temp->prev_manuf;
					temp->prev_manuf->next_manuf = temp->next_manuf;
				}
				else
				{
					temp->prev_manuf->next_manuf = NULL;
				}

				temp->next_manuf = NULL;
				temp->prev_manuf = NULL;

				temp->deleted = true;
				free(temp);
				return 0;
			}
			else if (temp == headID && temp != headMan) //if node is head of general and ID list
			{
				head = temp->next;
				temp->next->prev = NULL;
				temp->next = NULL;
				headID = temp->next_id;
				temp->next_id->prev_id = NULL;
				temp->next_id = NULL;

				if (temp->next_manuf != NULL) // node is tail of manuf
				{
					temp->next_manuf->prev_manuf = temp->prev_manuf;
					temp->prev_manuf->next_manuf = temp->next_manuf;
				}
				else
				{
					temp->prev_manuf->next_manuf = NULL;
				}

				temp->next_manuf = NULL;
				temp->prev_manuf = NULL;

				temp->deleted = true;
				free(temp);
				return 0;
			}
			else if (temp == headMan && temp != headID) //if node is head of general and manufact list
			{
				head = temp->next;
				temp->next->prev = NULL;
				temp->next = NULL;
				headMan = temp->next_manuf;
				temp->next_manuf->prev_manuf = NULL;
				temp->next_manuf = NULL;

				if (temp->next_id != NULL) //node is tail of id
				{
					temp->next_id->prev_id = temp->prev_id;
					temp->prev_id->next_id = temp->next_id;
				}
				else
				{
					temp->prev_id->next_id = NULL;
				}

				temp->next_id = NULL;
				temp->prev_id = NULL;

				temp->deleted = true;
				free(temp);
				return 0;
			}
			else //if node is head of all three
			{
				head = temp->next;
				temp->next->prev = NULL;
				temp->next = NULL;
				headMan = temp->next_manuf;
				temp->next_manuf->prev_manuf = NULL;
				temp->next_manuf = NULL;
				headID = temp->next_id;
				temp->next_id->prev_id = NULL;
				temp->next_id = NULL;
				free(temp);
				return 0;
			}
		}
		else if (temp->id_num == id) //if node is not head of general
		{
			if (temp != headID && temp != headMan) //node is not head
			{
				if (temp->next != NULL) //node is tail of general
				{
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
				}
				else
				{
					temp->prev->next = NULL;
				}

				temp->next = NULL;
				temp->prev = NULL;

				if (temp->next_id != NULL) //node is tail of id
				{
					temp->next_id->prev_id = temp->prev_id;
					temp->prev_id->next_id = temp->next_id;
				}
				else
				{
					temp->prev_id->next_id = NULL;
				}

				temp->next_id = NULL;
				temp->prev_id = NULL;

				if (temp->next_manuf != NULL) // node is tail of manuf
				{
					temp->next_manuf->prev_manuf = temp->prev_manuf;
					temp->prev_manuf->next_manuf = temp->next_manuf;
				}
				else
				{
					temp->prev_manuf->next_manuf = NULL;
				}

				temp->next_manuf = NULL;
				temp->prev_manuf = NULL;

				temp->deleted = true;
				free(temp);
				return 0;
			}
			else if (temp == headID && temp != headMan) //node is head of ID list
			{
				if (temp->next != NULL) //node is tail of general
				{
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
				}
				else
				{
					temp->prev->next = NULL;
				}

				temp->next = NULL;
				temp->prev = NULL;

				headID = temp->next_id;
				temp->next_id->prev_id = NULL;
				temp->next_id = NULL;

				if (temp->next_manuf != NULL) // node is tail of manuf
				{
					temp->next_manuf->prev_manuf = temp->prev_manuf;
					temp->prev_manuf->next_manuf = temp->next_manuf;
				}
				else
				{
					temp->prev_manuf->next_manuf = NULL;
				}

				temp->next_manuf = NULL;
				temp->prev_manuf = NULL;

				temp->deleted = true;
				free(temp);
				return 0;
			}
			else if (temp == headMan && temp != headID) //node is head of manufact list
			{
				if (temp->next != NULL) //node is tail of general
				{
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
				}
				else
				{
					temp->prev->next = NULL;
				}

				temp->next = NULL;
				temp->prev = NULL;

				if (temp->next_id != NULL) //node is tail of id
				{
					temp->next_id->prev_id = temp->prev_id;
					temp->prev_id->next_id = temp->next_id;
				}
				else
				{
					temp->prev_id->next_id = NULL;
				}

				temp->next_id = NULL;
				temp->prev_id = NULL;

				headMan = temp->next_manuf;
				temp->next_manuf->prev_manuf = NULL;
				temp->next_manuf = NULL;

				temp->deleted = true;
				free(temp);
				return 0;
			}
			else //node is head of both ID and manufact list
			{
				if (temp->next != NULL) //node is tail of general
				{
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
				}
				else
				{
					temp->prev->next = NULL;
				}

				temp->next = NULL;
				temp->prev = NULL;

				headID = temp->next_id;
				temp->next_id->prev_id = NULL;
				temp->next_id = NULL;
				headMan = temp->next_manuf;
				temp->next_manuf->prev_manuf = NULL;
				temp->next_manuf = NULL;
				temp->deleted = true;
				free(temp);
				return 0;
			}
		}
		
		temp = temp->next; //moves to next node
		i++;
	}
} 

int del_manuf(ifstream & inFile, Bike *& head, Bike *& headID, Bike *& headMan)
{
	char manufact[50];
	int i = 0;
	Bike *temp = head;

	inFile >> manufact;

	while (temp != NULL)
	{
		if (strcmp(temp->manufact, manufact) == 0) //matches manufacturer
		{
			if (i == 0)
			{
				if (temp != headID && temp != headMan) //not head of id nor manufact list
				{
					head = temp->next;
					temp->next->prev = NULL;
					temp->next - NULL;

					if (temp->next_id != NULL) //node is tail of id
					{
						temp->next_id->prev_id = temp->prev_id;
						temp->prev_id->next_id = temp->next_id;
					}
					else
					{
						temp->prev_id->next_id = NULL;
					}

					temp->next_id = NULL;
					temp->prev_id = NULL;

					if (temp->next_manuf != NULL) // node is tail of manuf
					{
						temp->next_manuf->prev_manuf = temp->prev_manuf;
						temp->prev_manuf->next_manuf = temp->next_manuf;
					}
					else
					{
						temp->prev_manuf->next_manuf = NULL;
					}

					temp->next_manuf = NULL;
					temp->prev_manuf = NULL;

					temp->deleted = true;
					free(temp);
					return 0;
				}
				else if (temp == headID && temp != headMan) //head of id but not manufact list
				{
					head = temp->next;
					temp->next->prev = NULL;
					temp->next = NULL;
					headID = temp->next_id;
					temp->next_id->prev_id = NULL;
					temp->next_id = NULL;

					if (temp->next_manuf != NULL) // node is tail of manuf
					{
						temp->next_manuf->prev_manuf = temp->prev_manuf;
						temp->prev_manuf->next_manuf = temp->next_manuf;
					}
					else
					{
						temp->prev_manuf->next_manuf = NULL;
					}

					temp->next_manuf = NULL;
					temp->prev_manuf = NULL;

					temp->deleted = true;
					free(temp);
					return 0;
				}
				else if (temp != headID && temp == headMan) //head of manufact but not id list
				{
					head = temp->next;
					temp->next->prev = NULL;
					temp->next = NULL;
					headMan = temp->next_manuf;
					temp->next_manuf->prev_manuf = NULL;
					temp->next_manuf = NULL;

					if (temp->next_id != NULL) //node is tail of id
					{
						temp->next_id->prev_id = temp->prev_id;
						temp->prev_id->next_id = temp->next_id;
					}
					else
					{
						temp->prev_id->next_id = NULL;
					}

					temp->next_id = NULL;
					temp->prev_id = NULL;

					temp->deleted = true;
					free(temp);
					return 0;
				}
				else //head of all lists
				{
					head = temp->next;
					temp->next->prev = NULL;
					temp->next = NULL;
					headMan = temp->next_manuf;
					temp->next_manuf->prev_manuf = NULL;
					temp->next_manuf = NULL;
					headID = temp->next_id;
					temp->next_id->prev_id = NULL;
					temp->next_id = NULL;
				}
			}
			else
			{
				if (temp != headID && temp != headMan) //head of none
				{
					if (temp->next != NULL) //node is tail of general
					{
						temp->prev->next = temp->next;
						temp->next->prev = temp->prev;
					}
					else
					{
						temp->prev->next = NULL;
					}

					temp->next = NULL;
					temp->prev = NULL;

					if (temp->next_id != NULL) //node is tail of id
					{
						temp->next_id->prev_id = temp->prev_id;
						temp->prev_id->next_id = temp->next_id;
					}
					else
					{
						temp->prev_id->next_id = NULL;
					}

					temp->next_id = NULL;
					temp->prev_id = NULL;

					if (temp->next_manuf != NULL) // node is tail of manuf
					{
						temp->next_manuf->prev_manuf = temp->prev_manuf;
						temp->prev_manuf->next_manuf = temp->next_manuf;
					}
					else
					{
						temp->prev_manuf->next_manuf = NULL;
					}

					temp->next_manuf = NULL;
					temp->prev_manuf = NULL;

					temp->deleted = true;
					free(temp);
					return 0;
				}
				else if (temp == headID && head != headMan) //head of id only
				{
					if (temp->next != NULL) //node is tail of general
					{
						temp->prev->next = temp->next;
						temp->next->prev = temp->prev;
					}
					else
					{
						temp->prev->next = NULL;
					}

					temp->next = NULL;
					temp->prev = NULL;

					headID = temp->next_id;
					temp->next_id->prev_id = NULL;
					temp->next_id = NULL;
					
					if (temp->next_manuf != NULL) // node is tail of manuf
					{
						temp->next_manuf->prev_manuf = temp->prev_manuf;
						temp->prev_manuf->next_manuf = temp->next_manuf;
					}
					else
					{
						temp->prev_manuf->next_manuf = NULL;
					}

					temp->next_manuf = NULL;
					temp->prev_manuf = NULL;

					temp->deleted = true;
					free(temp);
					return 0;
				}
				else if (temp != headID && temp == headMan) //head of manufact only
				{
					if (temp->next != NULL) //node is tail of general
					{
						temp->prev->next = temp->next;
						temp->next->prev = temp->prev;
					}
					else
					{
						temp->prev->next = NULL;
					}

					temp->next = NULL;
					temp->prev = NULL;

					if (temp->next_id != NULL) //node is tail of id
					{
						temp->next_id->prev_id = temp->prev_id;
						temp->prev_id->next_id = temp->next_id;
					}
					else
					{
						temp->prev_id->next_id = NULL;
					}

					temp->next_id = NULL;
					temp->prev_id = NULL;

					headMan = temp->next_manuf;
					temp->next_manuf->prev_manuf = NULL;
					temp->next_manuf = NULL;

					temp->deleted = true;
					free(temp);
					return 0;
				}
				else //head of id and manufact
				{
					if (temp->next != NULL) //node is tail of general
					{
						temp->prev->next = temp->next;
						temp->next->prev = temp->prev;
					}
					else
					{
						temp->prev->next = NULL;
					}

					temp->next = NULL;
					temp->prev = NULL;

					headID = temp->next_id;
					temp->next_id->prev_id = NULL;
					temp->next_id = NULL;

					headMan = temp->next_manuf;
					temp->next_manuf->prev_manuf = NULL;
					temp->next_manuf = NULL;

					temp->deleted = true;
					free(temp);
					return 0;
				}
			}
		}

		temp = temp->next; //moves to next node
		i++;
	}
}

void init(Bike *pBike)
{
	pBike->id_num = 0;
	pBike->cost_per_day = 0;
	strcpy_s(pBike->manufact, "");
	pBike->size = 0;
	pBike->deleted = false;
	pBike->rented_code = NOT_RENTED;
	pBike->next = NULL;
	pBike->next_id = NULL;
	pBike->next_manuf = NULL;
	pBike->prev = NULL;
	pBike->prev_id = NULL;
	pBike->prev_manuf = NULL;
	strcpy_s(pBike->to_whom, "");
}