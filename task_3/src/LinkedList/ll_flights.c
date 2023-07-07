#include "ll_flights.h"

LL_Flights *createLinkedListFlights() {
    LL_Flights *pLLFlights = (LL_Flights *) calloc(1, sizeof(LL_Flights));
    return pLLFlights;
}

Flight *createFlight(char *sFlightID, char *sDestination, int iNumberOfSeats, int iTimeOfDeparture) {
    Flight *pFlight = (Flight *) calloc(1, sizeof(Flight));
    pFlight->szFlightID = (char *) calloc(1, strlen(sFlightID) + 1);
    strncpy(pFlight->szFlightID, sFlightID, strlen(sFlightID) + 1);
    pFlight->szDestination = (char *) calloc(1, strlen(sDestination) + 1);
    strncpy(pFlight->szDestination, sDestination, strlen(sDestination) + 1);
    pFlight->iNumberOfSeats = iNumberOfSeats;
    pFlight->iTimeOfDeparture = iTimeOfDeparture;
    return pFlight;
}

Passenger *createPassenger(int iSeatNumber, char *sName, int iAge) {
    Passenger *pPassenger = (Passenger *) calloc(1, sizeof(Passenger));
    pPassenger->szName = (char *) calloc(1, strlen(sName) + 1);
    strncpy(pPassenger->szName, sName, strlen(sName) + 1);
    pPassenger->iSeatNumber = iSeatNumber;
    pPassenger->iAge = iAge;
    return pPassenger;
}

void freeLinkedListFlight(LL_Flights *pLLFlight) {
    Flight *pTmpFlight = pLLFlight->pHead;
    while (pTmpFlight != NULL) {
        pLLFlight->pHead = pTmpFlight->pNext;
        freeFlight(pTmpFlight);
        pTmpFlight = pLLFlight->pHead;
    }

    free(pLLFlight);
}

void freeFlight(Flight *pFlight) {
    free(pFlight->szFlightID);
    free(pFlight->szDestination);

    Passenger *pTmpPassenger = pFlight->pPassengerHead;
    while (pTmpPassenger != NULL) {
        pFlight->pPassengerHead = pTmpPassenger->pNext;
        freePassenger(pTmpPassenger);
        pTmpPassenger = pFlight->pPassengerHead;
    }

    free(pFlight);
}

void freePassenger(Passenger *pPassenger) {
    free(pPassenger->szName);
    free(pPassenger);
}

void addFlightToHead(LL_Flights *pLLFlight, Flight *pFlight) {
    if (pLLFlight->pHead == NULL) {
        pLLFlight->pHead = pFlight;
        pLLFlight->pTail = pFlight;
    } else {
        pFlight->pNext = pLLFlight->pHead;
        pLLFlight->pHead->pPrev = pFlight;
        pLLFlight->pHead = pFlight;
    }
}

void addFlightToTail(LL_Flights *pLLFlight, Flight *pFlight) {
    if (pLLFlight->pTail == NULL) {
        pLLFlight->pTail = pFlight;
        pLLFlight->pHead = pFlight;
    } else {
        pFlight->pPrev = pLLFlight->pTail;
        pLLFlight->pTail->pNext = pFlight;
        pLLFlight->pTail = pFlight;
    }
}

void addPassengerToFlight(Flight *pFlight, Passenger *pPassenger) {
    Passenger *pTmpPrev = NULL;
    Passenger *pTmp = NULL;
    pPassenger->pNext = NULL;

//    if list is empty
    if (pFlight->pPassengerHead == NULL) {
        pFlight->pPassengerHead = pPassenger;
        pFlight->iNumberOfReservation++;
        return;
    }

//    if input is less than head
    if (pPassenger->iSeatNumber < pFlight->pPassengerHead->iSeatNumber) {
        pPassenger->pNext = pFlight->pPassengerHead;
        pFlight->pPassengerHead = pPassenger;
        pFlight->iNumberOfReservation++;
        return;
    }

    pTmpPrev = pFlight->pPassengerHead;
    pTmp = pFlight->pPassengerHead->pNext;

    while (pTmp != NULL) {
        if (pPassenger->iSeatNumber < pTmp->iSeatNumber) {
            pTmpPrev->pNext = pPassenger;
            pPassenger->pNext = pTmp;
            pFlight->iNumberOfReservation++;
            return;
        }

        pTmpPrev = pTmp;
        pTmp = pTmp->pNext;
    }

    pTmpPrev->pNext = pPassenger;
    pFlight->iNumberOfReservation++;
}

int deleteFlightFromListByID(LL_Flights *pLLFlight, char *szFlightID) {
    Flight *pTmp = NULL;

    if (pLLFlight->pHead == NULL) {
        return FALSE;
    }

    pTmp = pLLFlight->pHead;

    while (pTmp != NULL) {
        if (strncmp(pTmp->szFlightID, szFlightID, strlen(szFlightID)) == 0) {

//            if there is only one element in list
            if (pTmp == pLLFlight->pHead && pTmp == pLLFlight->pTail) {
                pLLFlight->pHead = NULL;
                pLLFlight->pTail = NULL;
                freeFlight(pTmp);
                return TRUE;
            }

//            if head is match
            if (pTmp == pLLFlight->pHead) {
                pLLFlight->pHead = pTmp->pNext;
                pLLFlight->pHead->pPrev = NULL;

                freeFlight(pTmp);
                return TRUE;
            }

//            if tail is a match
            if (pTmp == pLLFlight->pTail) {
                pLLFlight->pTail = pTmp->pPrev;
                pLLFlight->pTail->pNext = NULL;

                freeFlight(pTmp);
                return TRUE;
            }

//            if middle is match
            pTmp->pPrev->pNext = pTmp->pNext;
            pTmp->pNext->pPrev = pTmp->pPrev;
            freeFlight(pTmp);
            return TRUE;
        }

        pTmp = pTmp->pNext;
    }

    return FALSE;
}

int deletePassengerFromListByName(Flight *pFlight, char *szName) {
    Passenger *pTmpPrev = NULL;
    Passenger *pTmp = NULL;

//    if the list is empty
    if (pFlight->pPassengerHead == NULL) {
        return FALSE;
    }

//    if input is less than head
    if (strncmp(pFlight->pPassengerHead->szName, szName, strlen(szName)) == 0) {
        pTmp = pFlight->pPassengerHead;
        pFlight->pPassengerHead = pFlight->pPassengerHead->pNext;

        pFlight->iNumberOfReservation--;
        freePassenger(pTmp);
        return TRUE;
    }

    pTmpPrev = pFlight->pPassengerHead;
    pTmp = pFlight->pPassengerHead->pNext;

    while (pTmp != NULL) {
//        if name hits a match
        if (strncmp(pTmp->szName, szName, strlen(szName)) == 0) {
            pTmpPrev->pNext = pTmp->pNext;

            pFlight->iNumberOfReservation--;
            freePassenger(pTmp);
            return TRUE;
        }

        pTmpPrev = pTmp;
        pTmp = pTmp->pNext;
    }

    return FALSE;
}

int deletePassengerFromListBySeatNumber(Flight *pFlight, int iSeatNumber) {
    Passenger *pTmpPrev = NULL;
    Passenger *pTmp = NULL;

//    if the list is empty
    if (pFlight->pPassengerHead == NULL) {
        return FALSE;
    }

    //    if input is less than head
    if (pFlight->pPassengerHead->iSeatNumber == iSeatNumber) {
        pTmp = pFlight->pPassengerHead;
        pFlight->pPassengerHead = pFlight->pPassengerHead->pNext;

        pFlight->iNumberOfReservation--;
        freePassenger(pTmp);
        return TRUE;
    }

    pTmpPrev = pFlight->pPassengerHead;
    pTmp = pFlight->pPassengerHead->pNext;

    while (pTmp != NULL) {
//        if seat number is match
        if (pTmp->iSeatNumber == iSeatNumber) {
            pTmpPrev->pNext = pTmp->pNext;

            pFlight->iNumberOfReservation--;
            freePassenger(pTmp);
            return TRUE;
        }

        pTmpPrev = pTmp;
        pTmp = pTmp->pNext;
    }

    return FALSE;
}

Flight *getFlightFromListByID(LL_Flights *pLLFlight, char *szFlightID) {
    Flight *pTmp = NULL;

    if (pLLFlight->pHead == NULL) {
        return NULL;
    }

    pTmp = pLLFlight->pHead;

    while (pTmp != NULL) {
        if (strncmp(pTmp->szFlightID, szFlightID, strlen(szFlightID)) == 0) {
            return pTmp;
        }

        pTmp = pTmp->pNext;
    }

    return NULL;
}

Passenger *getPassengerFromListBySeatNumber(Flight *pFlight, int iSeatNumber) {
    Passenger *pTmp = NULL;

    if (pFlight->pPassengerHead == NULL) {
        return NULL;
    }

    pTmp = pFlight->pPassengerHead;

    while (pTmp != NULL) {
        if (pTmp->iSeatNumber == iSeatNumber) {
            return pTmp;
        }

        pTmp = pTmp->pNext;
    }

    return NULL;
}

Passenger *getPassengerFromListByName(Flight *pFlight, char *szName) {
    Passenger *pTmp = NULL;

    if (pFlight->pPassengerHead == NULL) {
        return NULL;
    }

    pTmp = pFlight->pPassengerHead;

    while (pTmp != NULL) {
        if (strncmp(pTmp->szName, szName, strlen(szName)) == 0) {
            return pTmp;
        }

        pTmp = pTmp->pNext;
    }

    return NULL;
}