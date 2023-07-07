#include "program.h"

void startProgram() {
    printMainMenu();
    inputMainMenu();
}

void printMainMenu() {
    printf("\n1. Add flight to list\n");
    printf("2. Add passenger to flight\n");
    printf("3. Retrieve flight by index and print info\n");
    printf("4. Retrieve flight by time and print info\n");
    printf("5. Delete a flight\n");
    printf("6. Delete a passenger\n");
    printf("7. Change seat of a passenger\n");
    printf("8. Search the count of associated flights a passenger is by name\n");
    printf("9. Exit application\n");
}

void inputMainMenu() {
    char sInput[BUFFER_SIZE] = {0};
    int iInput = 0;
    bool bMainMenu = true;
    LL_Flights *pLLFlights = createLinkedListFlights();

    while (bMainMenu) {
        iInput = input_int(sInput, sizeof(sInput));

        switch (iInput) {
            case 1: {
                inputAddFlightToList(pLLFlights);
                printMainMenu();
                break;
            }
            case 2: {
                inputAddPassengerToFlight(pLLFlights);
                printMainMenu();
                break;
            }
            case 3: {
                inputPrintInfoAboutFlightByIndex(pLLFlights);
                printMainMenu();
                break;
            }
            case 4: {
                inputSearchFlightByDepartureTime(pLLFlights);
                printMainMenu();
                break;
            }
            case 5: {
                inputDeleteFlightFromList(pLLFlights);
                printMainMenu();
                break;
            }
            case 6: {
                inputDeletePassengerFromList(pLLFlights);
                printMainMenu();
                break;
            }
            case 7: {
                inputChangeSeatForPassenger(pLLFlights);
                printMainMenu();
                break;
            }
            case 8: {
                inputSearchAssociatedFlightByName(pLLFlights);
                printMainMenu();
                break;
            }
            case 9: {
                printf("Exiting program\n");
                bMainMenu = false;
                break;
            }
            default:
                printf("Invalid input\n");
                break;
        }
    }

    freeLinkedListFlight(pLLFlights);
}

void inputAddFlightToList(LL_Flights *pLLFlights) {
    char sInput[BUFFER_SIZE] = {0};
    int iInput = 0;
    Flight *pFlight = NULL;
    char *szFlightID = NULL;
    char *szDestination = NULL;
    int iNumberOfSeats = 0;
    int iTimeOfDeparture = 0;

//    szFlightID
    printf("Enter flight ID:\n");
    input_string(sInput, sizeof(sInput));
//    Check for duplicate flights
    while (getFlightFromListByID(pLLFlights, sInput)) {
        printf("Cant add duplicate flight ID, try again\n");
        memset(sInput, 0, sizeof(sInput));
        input_string(sInput, sizeof(sInput));
    }
    szFlightID = calloc(1, strlen(sInput) + 1);
    strcpy(szFlightID, sInput);

//    szDestination
    printf("Enter flight destination:\n");
    input_string(sInput, sizeof(sInput));
    szDestination = calloc(1, strlen(sInput) + 1);
    strcpy(szDestination, sInput);

//    iNumberOfSeats
    printf("Enter total amount of seats the plane has:\n");
    while (iInput == 0) {
        iInput = input_int(sInput, sizeof(sInput));
        if (iInput == 0) {
            printf("Invalid input, enter a number");
        } else if (iInput < 0) {
            printf("Invalid input, number needs to be more than 0\n");
            iInput = 0;
        }
    }
    iNumberOfSeats = iInput;
    iInput = 0;

//    iTimeOfDeparture
    printf("Enter time of departure:\n");
    while (iInput == 0) {
        iInput = input_int(sInput, sizeof(sInput));
        if (iInput < 0 || iInput > 2359) {
            printf("Invalid input, enter a time between 0000 - 2359\n");
        }
    }
    iTimeOfDeparture = iInput;
    iInput = 0;

    pFlight = createFlight(szFlightID, szDestination, iNumberOfSeats, iTimeOfDeparture);
    addFlightToTail(pLLFlights, pFlight);
    printf("Flight added to list!\n");

    free(szFlightID);
    free(szDestination);
}

void inputAddPassengerToFlight(LL_Flights *pLLFlights) {
    Flight *pFlight = NULL;
    Passenger *pPassenger = NULL;
    char *szName = NULL;
    int iAge = 0;
    int iSeatNumber = 0;
    char sInputFlight[BUFFER_SIZE];
    char sInputPassenger[BUFFER_SIZE];
    int iInputPassenger = 0;

//    Check if flight exist
    printf("Enter flight ID to add passenger on\n");
    input_string(sInputFlight, sizeof(sInputFlight));
    if (getFlightFromListByID(pLLFlights, sInputFlight)) {
        pFlight = getFlightFromListByID(pLLFlights, sInputFlight);
    } else {
        printf("No flight to add passenger on\n");
        return;
    }

//    Enter name
    printf("Please enter passenger name: \n");
    input_string(sInputPassenger, sizeof(sInputPassenger));
    szName = (char *) calloc(1, strlen(sInputPassenger) + 1);
    strncpy(szName, sInputPassenger, strlen(sInputPassenger));

//    Enter age
    printf("Please enter passenger age: \n");
    while (iInputPassenger == 0) {
        iInputPassenger = input_int(sInputPassenger, sizeof(sInputPassenger));
        if (iInputPassenger < 0) {
            printf("Invalid input, enter age of more than 0 years\n");
            iInputPassenger = 0;
        }
    }
    iAge = iInputPassenger;
    iInputPassenger = 0;

//    Enter seat number
    printf("Please enter flight seat number between %d and %d\n", 1, pFlight->iNumberOfSeats);
    while (iInputPassenger == 0) {
        iInputPassenger = input_int(sInputPassenger, sizeof(sInputPassenger));
        if (iInputPassenger < 1 || iInputPassenger > pFlight->iNumberOfSeats) {
            printf("Invalid input, enter flight seat number between %d and %d\n", 1, pFlight->iNumberOfSeats);
            iInputPassenger = 0;
            continue;
        }

        if (getPassengerFromListBySeatNumber(pFlight, iInputPassenger)) {
            printf("Seat number %d is busy, try again\n", iInputPassenger);
            iInputPassenger = 0;
        }
    }

    iSeatNumber = iInputPassenger;
    iInputPassenger = 0;

    pPassenger = createPassenger(iSeatNumber, szName, iAge);
    addPassengerToFlight(pFlight, pPassenger);

    free(szName);
}

void inputDeleteFlightFromList(LL_Flights *pLLFlights) {
    char sInput[BUFFER_SIZE];

    printf("To delete a flight please enter the flight ID\n");
    input_string(sInput, sizeof(sInput));
    if (deleteFlightFromListByID(pLLFlights, sInput)) {
        printf("Flight deleted from list\n");
    } else {
        printf("No flight found on that ID\n");
    }
}

void inputDeletePassengerFromList(LL_Flights *pLLFlights) {
    Flight *pFlight = NULL;
    char sInputFlight[BUFFER_SIZE];
    char sInputPassenger[BUFFER_SIZE];

    printf("Please enter a flight ID to delete a passenger from: \n");
    input_string(sInputFlight, sizeof(sInputFlight));
    pFlight = getFlightFromListByID(pLLFlights, sInputFlight);
    if (pFlight == NULL) {
        printf("No flight found on that ID\n");
        return;
    }

    printf("Please enter Name of passenger to remove from list: \n");
    input_string(sInputPassenger, sizeof(sInputFlight));
    if (deletePassengerFromListByName(pFlight, sInputPassenger)) {
        printf("Passenger deleted from flight ID %s\n", pFlight->szFlightID);
    } else {
        printf("No passenger found with that name\n");
    }

}

void inputChangeSeatForPassenger(LL_Flights *pLLFlights) {
    Flight *pFlight = NULL;
    Passenger *pTmpPassenger = NULL;
    Passenger *pPassenger = NULL;
    char sInputFlight[BUFFER_SIZE];
    char sInputPassenger[BUFFER_SIZE];
    int iInputPassenger = 0;

    printf("Please enter the flight ID to change seat number for a passenger: \n");
    input_string(sInputFlight, sizeof(sInputFlight));
    pFlight = getFlightFromListByID(pLLFlights, sInputFlight);
    if (pFlight == NULL) {
        printf("No flight found on that ID\n");
        return;
    }

    printf("Please enter Name of passenger to change seat on: \n");
    input_string(sInputPassenger, sizeof(sInputFlight));
    pTmpPassenger = getPassengerFromListByName(pFlight, sInputPassenger);
    if (pTmpPassenger == NULL) {
        printf("No passenger found with that name\n");
        return;
    }
    pPassenger = createPassenger(pTmpPassenger->iSeatNumber, pTmpPassenger->szName, pTmpPassenger->iAge);

    printf("Please enter the new seat number between %d and %d\n", 1, pFlight->iNumberOfSeats);
    while (iInputPassenger == 0) {
        iInputPassenger = input_int(sInputPassenger, sizeof(sInputPassenger));
        if (iInputPassenger < 1 || iInputPassenger > pFlight->iNumberOfSeats) {
            printf("Invalid input, enter flight seat number between %d and %d\n", 1, pFlight->iNumberOfSeats);
            iInputPassenger = 0;
            continue;
        }

        if (getPassengerFromListBySeatNumber(pFlight, iInputPassenger) == FALSE) {
            deletePassengerFromListBySeatNumber(pFlight, pTmpPassenger->iSeatNumber);
            pPassenger->iSeatNumber = iInputPassenger;
            addPassengerToFlight(pFlight, pPassenger);
            printf("Passenger moved to new seat number %d\n", pPassenger->iSeatNumber);

        } else {
            printf("Could not move passenger from seat %d to %d\n", iInputPassenger, pPassenger->iSeatNumber);
            iInputPassenger = 0;
        }
    }


}

void inputSearchAssociatedFlightByName(LL_Flights *pLLFlights) {
    Flight *pTmp = NULL;
    char sInput[BUFFER_SIZE];
    char *szName = NULL;
    int iCount = 0;

    printf("Please enter the name of the person to count associated flights with: \n");
    input_string(sInput, sizeof(sInput));
    szName = (char *) calloc(1, strlen(sInput) + 1);
    strncpy(szName, sInput, strlen(sInput));

    while (1) {
        if (pLLFlights->pHead == NULL) {
            break;
        }

        pTmp = pLLFlights->pHead;

        while (pTmp != NULL) {
            if (getPassengerFromListByName(pTmp, szName) != NULL) {
                iCount++;
            }

            pTmp = pTmp->pNext;
        }

        break;
    }

    printf("%s is associated with %d flights\n", sInput, iCount);
    free(szName);
}

void inputPrintInfoAboutFlightByIndex(LL_Flights *pLLFlights) {
    Flight *pFlight = NULL;
    Passenger *pPassenger = NULL;
    char sInput[BUFFER_SIZE];
    int iElement = 0;

    if (pLLFlights->pHead == NULL) {
        printf("There is no flights in the list\n");
        return;
    }

    printf("Please enter the index of flight you want extended information about: \n");
    while (iElement == 0) {
        iElement = input_int(sInput, sizeof(sInput));
        if (iElement <= 0) {
            printf("Invalid input, enter a number positive number starting from 1\n");
            iElement = 0;
        }
    }

    if (pLLFlights->pHead == NULL) {
        printf("No flight to in list to print information about\n");
        return;
    }

    pFlight = pLLFlights->pHead;

    for (int i = 1; i != iElement; ++i) {
        pFlight = pFlight->pNext;

        if (pFlight == NULL) {
            printf("The index you picked is out of bound, max is %d\n", i);
            return;
        }
    }

    printf("Flight ID: %s\n", pFlight->szFlightID);
    printf("Flight destination: %s\n", pFlight->szDestination);
    printf("Total number of seats: %d\n", pFlight->iNumberOfSeats);
    printf("Total number of seat reservations: %d\n", pFlight->iNumberOfReservation);
    printf("Time of departure: %04d\n", pFlight->iTimeOfDeparture);

    if (pFlight->pPassengerHead == NULL) {
        printf("Flight have no passenger on it\n");
        return;
    }

    pPassenger = pFlight->pPassengerHead;
    while (pPassenger != NULL) {
        printf("Name: %s\t\tAge: %03d\t\tSeat number: %02d\n",
               pPassenger->szName, pPassenger->iAge, pPassenger->iSeatNumber);
        pPassenger = pPassenger->pNext;
    }
}

void inputSearchFlightByDepartureTime(LL_Flights *pLLFlights) {
    Flight *pFlight = NULL;
    char sInput[BUFFER_SIZE];
    int iTime = 0;
    int iCount = 0;

    printf("Please enter the departure time you want to search with\n");
    while (iTime == 0) {
        iTime = input_int(sInput, sizeof(sInput));
        if (iTime < 0 || iTime > 2359) {
            printf("Invalid input, enter a time between 0000 and 2359\n");
            iTime = 0;
        }
    }

    if (pLLFlights->pHead == NULL) {
        printf("There is no flights in the list\n");
        return;
    }


    pFlight = pLLFlights->pHead;

    for (int i = 1; pFlight != NULL; ++i) {
        if (pFlight->iTimeOfDeparture == iTime) {
            printf("Flight ID %s matches departure time of %04d and have index %d\n",
                   pFlight->szFlightID, iTime, i);
            iCount++;
        }

        if (iCount == 0) {
            printf("No flights have departure of that time\n");
        }

        pFlight = pFlight->pNext;
    }

}