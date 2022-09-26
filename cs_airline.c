// CS Airline
// cs_airline.c
//
// This program was written by Zean Mei (z5434651)
// on 07/05/2022
//
// Version 1.0.0 2022-04-02: Initial Release.
// Version 1.0.1 2022-04-06: Fixed style issue with help function.
//
// a booking system capable of managing a single flight with multiple stops

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manifest.h"
////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////  CONSTANTS  /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// The buffer length is used for reading a single line
#define MAX_STRING_LEN 100

#define COMMAND_PRINT_HELP "help"
#define COMMAND_PRINT_ROUTE "print"
#define COMMAND_SUBROUTE "subroute"
#define COMMAND_CHANGE_ORIGIN "change_origin"
#define COMMAND_BYPASS "bypass"
#define COMMAND_EMERGENCY "emergency"
#define COMMAND_CANCEL "cancel"
#define COMMAND_REVERSE "reverse"
#define COMMAND_ADD_PERSON "add_person"
#define COMMAND_PRINT_MANIFEST "print_manifest"
#define COMMAND_FLIGHT_STATS "stats"

// you may choose to add additional #defines here.
#define EMERGENCY_CODE "ALT0"
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  STRUCTS  //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Airports represent 'stops' along the flight path
// These are the 'nodes' of the linked list
struct airport {
    char code[MAX_STRING_LEN];
    int arrival_time;
    int departure_time;
    struct airport *next_airport;
    struct manifest *manifest;
};

// Root flight structure
// This stores the state of the current flight
// (i.e. the head and tail of the linked list)
struct flight {
    struct airport *origin_airport;
    struct airport *dest_airport;
};

// The detail of each person in the manifest
struct person {
    char name[MAX_NAME_LEN];
    double weight;
    struct person *next;
};
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  FUNCTION PROTOTYPES  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Helper Functions
void remove_newline(char input[MAX_STRING_LEN]);
void do_print_help(void);

void interpret_line(
    char buffer[MAX_STRING_LEN],
    int *val0,
    int *val1,
    char word[MAX_STRING_LEN]
);

// Stage 1 Functions
struct flight *setup_flight(void);
void do_print_airports(struct flight *flight);
void print_one_airport(struct airport *ap);
struct airport *create_airport(
    char code[MAX_STRING_LEN],
    int arrival_time,
    int departure
);

// Stage 2 Functions
void do_subroute(struct flight *flight);
void do_change_origin(struct flight *flight);

// Your functions prototypes here
void checkvalid(struct flight *new_flight);
void calc_subroute(int start, int end);
void add_origin(struct airport *new_airport, struct flight *flight);
void do_bypass(struct flight *flight);
void remove_backslashn(char input_code[MAX_STRING_LEN]);
void do_emergency(struct flight *flight);
void cancel_flight(struct flight *flight);
void do_reverse(struct flight *flight);
void do_add_person(struct flight *flight);
void do_print_manifest(struct flight *flight);
void free_airports(struct flight *flight);
void free_manifest(struct flight *flight);
////////////////////////////////////////////////////////////////////////////////
//////////////////////////  FUNCTION IMPLEMENTATIONS  //////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {

    // Stage 1.1
    struct flight *flight = setup_flight();

    // Move into and stay in operational mode until CTRL+D
    printf("Enter Command: ");
    char command_line[MAX_STRING_LEN];
    while (fgets(command_line, MAX_STRING_LEN, stdin) != NULL) {

        remove_newline(command_line);

        if (strcmp(COMMAND_PRINT_HELP, command_line) == 0) {
            // A help command we have implemented for you.
            do_print_help();
        } else if (strcmp(COMMAND_PRINT_ROUTE, command_line) == 0) {
            // print stops and information of the airports of the flight
            do_print_airports(flight);
        } else if (strcmp(COMMAND_SUBROUTE, command_line) == 0) {
            // calculate the input airport's arrival_time 
            // with the departure_time of the origin
            do_subroute(flight);
        } else if (strcmp(COMMAND_CHANGE_ORIGIN, command_line) == 0) {
            // add a new airport and link it to the new origin_airport
            do_change_origin(flight);
        } else if (strcmp(COMMAND_BYPASS, command_line) == 0) {
            // remove an airport from the flight
            do_bypass(flight);
        } else if (strcmp(COMMAND_EMERGENCY, command_line) == 0) {
            // check the emergency condition and add an emergency airport
            do_emergency(flight);
        } else if (strcmp(COMMAND_CANCEL, command_line) == 0) {
            // cancel the flight route and add a new one
            cancel_flight(flight);
            free(flight);
            flight = setup_flight();
        } else if (strcmp(COMMAND_REVERSE, command_line) == 0) {
            // reverse the flight route
            do_reverse(flight);
        } else if (strcmp(COMMAND_ADD_PERSON, command_line) == 0) {
            // add a person to the manifest
            do_add_person(flight);
        } else if (strcmp(COMMAND_PRINT_MANIFEST, command_line) == 0) {
            // print the detail of the manifest
            do_print_manifest(flight);
        }
        
            
        printf("Enter Command: ");
    }
    // freeing all the person in manifest
    free_manifest(flight);
    // freeing all the airports and flight before exit
    free_airports(flight);
    if (flight != NULL) {
        free(flight);
    }
    
    printf("Goodbye!\n");

    return 0;
}

// Stage 1.1
// Creates and initialises a flight by asking the user for input.
// Parameters:
//     None
// Returns:
//     Pointer to the malloc'd flight
struct flight *setup_flight(void) {

    // Create a new, empty flight
    struct flight *new_flight = malloc(sizeof(struct flight));
    new_flight->origin_airport = NULL;
    new_flight->dest_airport = NULL;

    // Read number of stops using fgets
    printf("How many stops will be enroute? ");
    char input_line[MAX_STRING_LEN] = {0};
    fgets(input_line, MAX_STRING_LEN, stdin);

    // Convert the string to a number
    int num_stops = 0;
    num_stops = atoi(input_line);
    // read strings
    int i = 1;
    int departure_time;
    int arrival_time;
    char code[MAX_STRING_LEN];
    char input_airport[MAX_STRING_LEN];
    // read input and create airport using input information
    fgets(input_airport, MAX_STRING_LEN, stdin);
    interpret_line(input_airport, &arrival_time, &departure_time, code);
    new_flight->origin_airport = create_airport(code, arrival_time, departure_time);
    struct airport *tmp = new_flight->origin_airport;
    while (i < num_stops) {
        fgets(input_airport, MAX_STRING_LEN, stdin);
        interpret_line(input_airport, &arrival_time, &departure_time, code);
        struct airport *new_airport = create_airport(code, arrival_time, departure_time);
        tmp->next_airport = new_airport;
        tmp = tmp->next_airport;
        i++;
    }
    // check if arrival time and departure time is valid
    checkvalid(new_flight);
    // link the last airport to the dest_airport
    struct airport *n = new_flight->origin_airport;
    while (n->next_airport != NULL) {
        n = n->next_airport;
    }
    new_flight->dest_airport = n;
    return new_flight;
}

// a function for checking if the time of arrival and departure is valid
void checkvalid(struct flight *new_flight) {
    struct airport *previous = new_flight->origin_airport;
    while (previous != NULL) {
        struct airport *n = previous->next_airport;
        if (n != NULL && n->arrival_time > n->departure_time) {
            printf("Departure time cannot be before the arrival time!\n");
            previous->next_airport = n->next_airport;
            free(n);
        }
        else if (n != NULL && previous->departure_time > n->arrival_time) {
            printf("New arrival time cannot be before the previous departure time\n");
            previous->next_airport = n->next_airport;
            free(n);
        }
        previous = previous->next_airport;
    }
}

// Stage 1.2
// Given a pointer to a flight struct, prints all the info about the route
// Parameters:
//     flight = the flight which contains the flight route to print
// Returns:
//     None
void do_print_airports(struct flight *flight) {
    struct airport *n = flight->origin_airport;
    printf("ROUTE:\n");
    while (n != NULL) {
        print_one_airport(n);
        n = n->next_airport;
    }
}

// Stage 1.2
// PROVIDED FUNCTION - DO NOT CHANGE
// Given a pointer to an airport struct,
// prints all the info about the airport
// Parameters:
//     ap = the airport to print
// Returns:
//     None
void print_one_airport(struct airport *ap) {
    printf(
        "%8s:    %04d -> %04d\n",
       ap->code,
       ap->arrival_time,
       ap->departure_time
    );
}

// Stage 1.1
// Given the information about a new airport,
// Uses `malloc` to create memory for it and returns a pointer to
// that memory.
// Parameters:
//     code            = the airport ICAO code
//     arrival_time    = the arrival time of the new airport
//     departure_time  = the departure time of the new airport
// Returns:
//     A pointer to the malloc'd struct airport
struct airport *create_airport(
    char code[MAX_STRING_LEN],
    int arrival_time,
    int departure_time)
{
    // Malloc new struct
    struct airport *new_airport = malloc(sizeof(struct airport));

    // initialise airport fields
    strcpy(new_airport->code, code);
    new_airport->arrival_time = arrival_time;
    new_airport->departure_time = departure_time;
    new_airport->next_airport = NULL;
    new_airport->manifest = NULL;
    return new_airport;
}

// Stage 2.1
// a function for the subroute command to give information about the time of subroute
void do_subroute(struct flight *flight) {
    printf("Enter airport code: ");
    char input_code[MAX_STRING_LEN];
    fgets(input_code, MAX_STRING_LEN, stdin);
    remove_backslashn(input_code);
    int start_time = flight->origin_airport->departure_time;
    int end_time = 0;
    int flag = 0;
    struct airport *n = flight->origin_airport;
    while (n != NULL) {
        int result = strcmp(n->code, input_code);
        if (result == 0) {
            end_time = n->arrival_time;
            flag = 1;
            break;
        }
        n = n->next_airport;
    }
    if (flag == 0) {
        printf("Desired subroute destination does not exist!\n");
        return;
    }
    calc_subroute(start_time, end_time);
    return;
}

// a function that remove the \n from the input string
void remove_backslashn(char input_code[MAX_STRING_LEN]) {
    int n = 0;
    while (input_code[n] != '\0') {
        if (input_code[n] == '\n') {
            input_code[n] = '\0';
            break;
        }
        n++;
    }
}

// a function that use start_time and end_time to calculate time for subroute
void calc_subroute(int start, int end) {
    int hours = (end / 100) - (start / 100);
    int mins = (end % 100) - (start % 100);
    int minute_diff = hours * 60 + mins;
    if (minute_diff < 0) {
        minute_diff *= -1;
    }
    hours = minute_diff / 60;
    mins = minute_diff % 60;
    printf("Time for subroute: %d hrs & %d mins\n", hours, mins);
    return;
}

// Stage 2.2
// a function that adds a new airport and make it the new origin_airport
void do_change_origin(struct flight *flight) {
    printf("Enter new origin info: ");
    char info[MAX_STRING_LEN];
    fgets(info, MAX_STRING_LEN, stdin);
    int arrival_time;
    int departure_time;
    char code[MAX_STRING_LEN];
    interpret_line(info, &arrival_time, &departure_time, code);
    struct airport *new_origin = create_airport(code, arrival_time, departure_time);
    add_origin(new_origin, flight);
    return;
}

// a function that checks the new airport and add it to the origin_airport
void add_origin(struct airport *new_airport, struct flight *flight) {
    // check the arrival_time and departure_time
    if (new_airport->arrival_time > new_airport->departure_time) {
        printf("Departure time cannot be before the arrival time!\n");
        return;
    } else if (new_airport->departure_time > flight->origin_airport->arrival_time) {
        printf(
        "Departure of new origin cannot be after\
         the arrival time of the next airport!\n");
        return;
    }
    //check if the code of the new_airport is unique 
    struct airport *p = flight->origin_airport;
    while (p != NULL) {
        if (strcmp(new_airport->code, p->code) == 0) {
            printf("New airport code is not unique!\n");
            return;
        }
        p = p->next_airport;
    }
    new_airport->next_airport = flight->origin_airport;
    flight->origin_airport = new_airport;
    printf("Added: %s\n", flight->origin_airport->code);
    return;
}
    
// Stage 2.3
// a function that remove the input airport from the flight
void do_bypass(struct flight *flight) {
    printf("Enter airport code: ");
    char input_code[MAX_STRING_LEN];
    fgets(input_code, MAX_STRING_LEN, stdin);
    remove_backslashn(input_code);
    // check if the airport exist in the flight route
    struct airport *n = flight->origin_airport;
    int flag = 0;
    while (n != NULL) {
        if (strcmp(n->code, input_code) == 0) {
            flag = 1;
            break;
        }
        n = n->next_airport;
    }
    if (flag == 0) {
        printf("No airport of that code exists!\n");
        return;
    }
    // delete the input airport
    if (strcmp(flight->origin_airport->code, input_code) == 0) {
        struct airport *tmp = flight->origin_airport;
        flight->origin_airport = tmp->next_airport;
        free(tmp);
    } else if (strcmp(flight->dest_airport->code, input_code) == 0) {
        struct airport *tmp1 = flight->origin_airport;
        while (tmp1->next_airport != NULL) {
            tmp1 = tmp1->next_airport;
        }
        flight->dest_airport = tmp1;
        flight->dest_airport->next_airport = NULL;
        free(tmp1->next_airport);
    } else {
        struct airport *previous = flight->origin_airport;
        while (previous != NULL) {
            struct airport *p = previous->next_airport;
            if (p != NULL && strcmp(p->code, input_code) == 0) {
                previous->next_airport = p->next_airport;
                free(p);
                break;
            }
            previous = previous->next_airport;
        }
    }
    return;
}

// Stage 3.1
// a function for the emergency command 
void do_emergency(struct flight *flight) {
    printf("How long until emergency: ");
    char time[MAX_STRING_LEN];
    fgets(time, MAX_STRING_LEN, stdin);
    int hours = atoi(time);
    int departure = flight->origin_airport->departure_time;
    int emergency_time = departure + hours * 100;
    int arrival = flight->dest_airport->arrival_time;
    char emergency_code[MAX_STRING_LEN] = EMERGENCY_CODE;
    // check if the flight is safe
    if (emergency_time > arrival) {
        printf("Flight was safe!\n");
        return;
    } else {
        // the condition when hours equal 0 
        if (hours == 0) {
            flight->origin_airport->departure_time = 0;
            struct airport *n = flight->origin_airport->next_airport;
            struct airport *tmp;
            while (n != NULL) {
                tmp = n;
                n = n->next_airport;
                free(tmp);
            }
            flight->origin_airport->next_airport = NULL;
        } else {
            // condition when hours is not equal to 0
            struct airport *p = flight->origin_airport;
            while (p != NULL) {
                if (p->arrival_time >= emergency_time) {
                    strcpy(p->code, emergency_code);
                    p->arrival_time = emergency_time;
                    p->departure_time = 0;
                    flight->dest_airport = p;
                    break;
                }
                p = p->next_airport;
            }
            struct airport *n = flight->dest_airport->next_airport;
            struct airport *tmp1;
            while (n != NULL) {
                tmp1 = n;
                n = n->next_airport;
                free(tmp1);
            }
            flight->dest_airport->next_airport = NULL;
        }
    }
    return;
}        
        
// Stage 3.2
// a function for cancelling the flight and creating a new one again
void cancel_flight(struct flight *flight) {
    free_airports(flight);
    printf("Flight cancelled. Now accepting a new flight: \n");
    return;
}

// a function for freeing all the airports in a flight
void free_airports(struct flight *flight) {
    struct airport *n = flight->origin_airport;
    struct airport *tmp;
    while (n != NULL) {
        tmp = n;
        n = n->next_airport;
        free(tmp);
    }
    return;
}

// Stage 3.3
// a function that reverse the original flight route
void do_reverse(struct flight *flight) {
    struct airport *n = flight->origin_airport;
    struct airport *previous = NULL;
    struct airport *after = NULL;
    while (n != NULL) {
        after = n->next_airport;
        n->next_airport = previous;
        previous = n;
        n = after;
    }
    flight->origin_airport = previous;
    struct airport *tmp = flight->origin_airport;
    while (tmp->next_airport != NULL) {
        tmp = tmp->next_airport;
    }
    flight->dest_airport = tmp;
}

// Stage 4.1
// a function that add people to manifest 
// and link the manifest to certain airport
void do_add_person(struct flight *flight) {
    char airport_code[MAX_STRING_LEN];
    char passenger_name[MAX_NAME_LEN];
    char weight[MAX_STRING_LEN];
    printf("Enter the airport code: ");
    fgets(airport_code, MAX_STRING_LEN, stdin);
    remove_backslashn(airport_code);
    printf("Enter the passenger name: ");
    fgets(passenger_name, MAX_NAME_LEN, stdin);
    remove_backslashn(passenger_name);
    printf("Enter the passenger weight: ");
    fgets(weight, MAX_STRING_LEN, stdin);
    remove_backslashn(weight);
    double passenger_weight = atof(weight);
    // check if the airport exists
    struct airport *n = flight->origin_airport;
    int flag = 0;
    while (n != NULL) {
        if (strcmp(n->code, airport_code) == 0) {
            flag = 1;
            break;
        }
        n = n->next_airport;
    }
    if (flag == 0) {
        printf("No airport of that code exists!\n");
        return;
    }
    if (n->manifest == NULL) {
        n->manifest = create_manifest();
        add_person(n->manifest, passenger_name, passenger_weight);
    } else {
        add_person(n->manifest, passenger_name, passenger_weight);
    }
    return;
}

// Stage 4.2
// a function that print the detail of the manifest
void do_print_manifest(struct flight *flight) {
    printf("Enter the airport code: ");
    char airport_code[MAX_STRING_LEN];
    fgets(airport_code, MAX_STRING_LEN, stdin);
    remove_backslashn(airport_code);
    struct airport *n = flight->origin_airport;
    int flag = 0;
    while (n != NULL) {
        if (strcmp(n->code, airport_code) == 0) {
            flag = 1;
            break;
        }
        n = n->next_airport;
    }
    if (flag == 0) {
        printf("No airport of that code exists!\n");
        return;
    }
    struct person *p = n->manifest->people;
    printf("Manifest:\n");
    while (p != NULL) {
        printf("    %.2lf - %s\n", p->weight, p->name);
        p = p->next;
    }
    return;
}

// a function that free the manifest and all the people
void free_manifest(struct flight *flight) {
    struct airport *n = flight->origin_airport;
    while (n != NULL) {
        if (n->manifest != NULL) {
            struct person *p = n->manifest->people;
            while (p != NULL) {
                struct person *tmp = p;
                p = p->next;
                free(tmp);
            }
            free(n->manifest);
        }
        n = n->next_airport;
    }
    return;
}
         

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  HELPER FUNCTIONS  //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Helper Function
// You likely do not need to change this function.
//
// Given a raw string will remove and first newline it sees.
// The newline character wil be replaced with a null terminator ('\0')
// Parameters:
//     flight  = the flight to move people along of
// Returns:
//     None
void remove_newline(char input[MAX_STRING_LEN]) {

    // Find the newline or end of string
    int index = 0;
    while (input[index] != '\n' && input[index] != '\0') {
        index++;
    }
    // Goto the last position in the array and replace with '\0'
    // Note: will have no effect if already at null terminator
    input[index] = '\0';
}


// Helper Function
// You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
//
// Given a raw string in the following foramt: [integer0] [integer1] [string]
// This function will extract the relevant values into the given pointer variables.
// The function will also remove any newline characters.
//
// For example, if given: "0135 0545 YSSY"
// The function will put the integer values
//     135 into the val1 pointer
//     545 into the val2 pointer
// And will copy a null terminated string
//     "YSSY" into the 'word' array
//
// If you are interested, `strtok` is a function which takes a string,
// and splits it up into before and after a "token" (the second argument)
//
// Parameters:
//     buffer  = A null terminated string in the following format
//               [integer0] [integer1] [string]
//     val0    = A pointer to where [integer0] should be stored
//     val1    = A pointer to where [integer1] should be stored
//     word    = An array for the [string] to be copied into
// Returns:
//     None
void interpret_line(
    char buffer[MAX_STRING_LEN],
    int *val0,
    int *val1,
    char word[MAX_STRING_LEN]
) {

    // Remove extra newline
    remove_newline(buffer);

    // Extract value 1 as int
    char *val0_str = strtok(buffer, " ");
    if (val0_str != NULL) {
        *val0 = atoi(val0_str);
    } else {
        *val0 = -1;
    }

    // Extract value 2 as int
    char *val1_str = strtok(NULL, " ");
    if (val1_str != NULL) {
        *val1 = atoi(val1_str);
    } else {
        *val1 = -1;
    }

    char *word_str = strtok(NULL, " ");
    if (word_str != NULL) {
        // Extract word
        strcpy(word, word_str);
    }

    if (val0_str == NULL || val1_str == NULL || word_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly interpret line: %s.\n", buffer);
    }

}

void do_print_help(void) {
    printf("+-----------------+-------------------------------------------+\n");
    printf("|Command Name     | How to Use                                |\n");
    printf("+=================+===========================================+\n");
    printf("|PRINT ROUTE      | Enter command: print                      |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|SUBROUTE         | Enter command: subroute                   |\n");
    printf("|                 | Enter airport code: [AIRPORT CODE]        |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|CHANGE ORIGIN    | Enter command: change_origin              |\n");
    printf("|                 | Enter new origin info: [ARRIVAL TIME]...  |\n");
    printf("|                 |     ... [DEPARTURE TIME] [AIRPORT CODE]   |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|BYPASS           | Enter command: bypass                     |\n");
    printf("|                 | Enter airport code: [AIRPORT CODE]        |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|EMERGENCY        | Enter command: emergency                  |\n");
    printf("|                 | How long until emergency: [TIME IN HOURS] |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|CANCEL           | Enter command: cancel                     |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|REVERSE          | Enter command: reverse                    |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|                 | Enter command: add_person                 |\n");
    printf("|ADD PERSON       | Enter the airport code: [AIRPORT CODE]    |\n");
    printf("|                 | Enter the passenger name: [NAME]          |\n");
    printf("|                 | Enter the passenger weight: [WEIGHT]      |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|PRINT MANIFEST   | Enter command: print_manifest             |\n");
    printf("|                 | Enter the airport code: [AIRPORT CODE]    |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|FLIGHT STATISTICS| Enter command: stats                      |\n");
    printf("+-----------------+-------------------------------------------+\n");
}
