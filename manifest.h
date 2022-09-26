// CS Airline Manifest Header
// manifest.h
//
// This program was written by cs1511, and
// YOU ARE NOT ALLOWED TO MODIFY THIS FILE.
//
// Version 1.0.0 2022-04-02: Initial Release.
//
// Contains function comments and prototypes for
// the manifest.


#ifndef MANIFEST_H
#define MANIFEST_H

#define MAX_NAME_LEN 1024

struct manifest {
    struct person *people;
};


// PROVIDED FUNCTION
// Creates a new, empty, manifest
// Parameters:
//     none
// Returns:
//     Returns a pointer to a malloc'd manifest
struct manifest *create_manifest();


// PROVIDED FUNCTION
// Prints the manifest
// Parameters:
//     manifest   = pointer to the manifest to print
// Returns:
//     None
void print_manifest(struct manifest *manifest);


// PROVIDED FUNCTION
// Appended a person to the end of the given manifest
// Parameters:
//     manifest    = pointer to the manifest to add to
//     name        = name of the new person
//     weight      = weight of the new person
// Returns:
//     None
void add_person(
    struct manifest *manifest,
    char name[MAX_NAME_LEN],
    double weight
);

// PROVIDED FUNCTION
// Deletes and free's all malloc'd memory of a manifest
// Parameters:
//     manifest    = pointer to the manifest to delete
// Returns:
//     None
void delete_manifest(struct manifest *manifest);

// STUDENT FUNCTION
// Gets the sum of all the people's weight on the manifest
// Parameters:
//     manifest    = pointer to the manifest to add to
// Returns:
//     The sum of the weight of all people in the given manifest
double manifest_weight(struct manifest *manifest);


// STUDENT FUNCTION
// appends manifest_2 to the end of manifest_1.
// Can assume manifest_1 and manifest_2 are not NULL.
// Parameters:
//     manifest_1      = First manifest
//     manifest_2      = Manifest to add to the first
// Returns:
//     A pointer to the joined manifest, will never return NULL.
struct manifest *join_manifest(
    struct manifest *manifest_1,
    struct manifest *manifest_2
);

#endif