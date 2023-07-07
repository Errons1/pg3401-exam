#ifndef TASK_3_LL_FLIGHTS_H
#define TASK_3_LL_FLIGHTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct Passenger {
    struct Passenger *pNext;
    int iSeatNumber;
    char *szName;
    int iAge;
} Passenger;

typedef struct Flight {
    struct Flight *pNext;
    struct Flight *pPrev;
    char *szFlightID;
    char *szDestination;
    int iNumberOfSeats;
    int iNumberOfReservation;
    int iTimeOfDeparture;
    Passenger *pPassengerHead;
} Flight;

typedef struct LL_Flights {
    Flight *pHead;
    Flight *pTail;
} LL_Flights;

// --- Functions for creating and free in a safe manner ---

/* Creates LL_Flights struct in safe manner
 * return       pointer to LL_Flights struct */
LL_Flights *createLinkedListFlights();

/* Creates Flight struct in safe manner
 * return       pointer to Flight struct */
Flight *createFlight(char *sFlightID, char *sDestination, int iNumberOfSeats, int iTimeOfDeparture);

/* Creates Passenger struct in safe manner
 * return       pointer to Passenger struct */
Passenger *createPassenger(int iSeatNumber, char *sName, int iAge);

/* Frees everything associated with LL_Flight struct in safe manner */
void freeLinkedListFlight(LL_Flights *pLLFlight);

/* Frees everything associated with Flight struct in safe manner */
void freeFlight(Flight *pFlight);

/* Frees everything associated with Passenger struct in safe manner*/
void freePassenger(Passenger *pPassenger);


// --- Function to add elements into linked list ---

/* Add Flight element to head in a LL_Flight
 * para1        pointer to LL_Flights struct
 * para2        pointer to Flight element */
void addFlightToHead(LL_Flights *pLLFlight, Flight *pFlight);

/* Add Flight element to tail in a LL_Flight
 * para1        pointer to LL_Flights struct
 * para2        pointer to Flight element */
void addFlightToTail(LL_Flights *pLLFlight, Flight *pFlight);

/* Add Passenger element to Flight linked list
 * para1        pointer to Flight struct
 * para2        pointer to Passenger element */
void addPassengerToFlight(Flight *pFlight, Passenger *pPassenger);


// --- Function to search for a element and then delete it ---

/* Deletes a Flight element from LL_Flights struct by using flight ID as a search parameter
 * para1        pointer to LL_Flights struct
 * para2        pointer to Flight element
 * return       int value 1 (true) if deleted or 0 (false) on none deletion*/
int deleteFlightFromListByID(LL_Flights *pLLFlight, char *szFlightID);

/* Deletes a Passenger element from Flight linked list by using name as search parameter
 * para1        pointer to Flight struct
 * para2        pointer to Passenger element
 * return       int value 1 (true) if deleted or 0 (false) on none deletion*/
int deletePassengerFromListByName(Flight *pFlight, char *szName);

/* Deletes a Passenger element from Flight linked list by using seat number as a search parameter
 * para1        pointer to Flight struct
 * para2        pointer to Passenger element
 * return       int value 1 (true) if deleted or 0 (false) on none deletion */
int deletePassengerFromListBySeatNumber(Flight *pFlight, int iSeatNumber);


// --- Function for getting or searching if element exist ---

/* Searches for a Flight element from LL_Flights struct by using flight ID as search parameter
 * para1        pointer to LL_Flights struct
 * para2        pointer to Flight element
 * return       pointer to found Flight element or NULL */
Flight *getFlightFromListByID(LL_Flights *pLLFlight, char *szFlightID);

/* Deletes a Passenger element from Flight linked list by using seat number as search parameter
 * para1        pointer to Flight struct
 * para2        pointer to Passenger element
 * return       pointer to found Passenger element or NULL */
Passenger *getPassengerFromListBySeatNumber(Flight *pFlight, int iSeatNumber);

/* Deletes a Passenger element from Flight linked list by using name as search parameter
 * para1        pointer to Flight struct
 * para2        pointer to Passenger element
 * return       pointer to found Passenger element or NULL */
Passenger *getPassengerFromListByName(Flight *pFlight, char *szName);

#endif //TASK_3_LL_FLIGHTS_H