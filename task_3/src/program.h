#ifndef TASK_3_PROGRAM_H
#define TASK_3_PROGRAM_H

#include "../lib/input/input.h"
#include "LinkedList/ll_flights.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

/* Starts the program by printing the menu and make terminal ready for input*/
void startProgram();

void printMainMenu();

void inputMainMenu();

void inputAddFlightToList(LL_Flights *pLLFlights);

void inputAddPassengerToFlight(LL_Flights *pLLFlights);

void inputDeleteFlightFromList(LL_Flights *pLLFlights);

void inputDeletePassengerFromList(LL_Flights *pLLFlights);

void inputChangeSeatForPassenger(LL_Flights *pLLFlights);

void inputSearchAssociatedFlightByName(LL_Flights *pLLFlights);

void inputPrintInfoAboutFlightByIndex(LL_Flights *pLLFlights);

void inputSearchFlightByDepartureTime(LL_Flights *pLLFlights);

#endif //TASK_3_PROGRAM_H
