#pragma once
#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <time.h>

extern int id, p; 

enum {EX_SUCCESS, EX_FAILURE};


// Lecture 1 Exercise 1 : Ring Communication
// ==========================================
int exercise1_1(int argc, char* argv[]);

// Lecture 1 Exercise 2: Ring Communication w/Probe 
// ================================================
int exercise1_2(int argc, char* argv[]);

// Lecture 1 Exercise 3 : Blocking Point to Point Communication
// =============================================================
int exercise1_3(int argc, char* argv[]);


// Lecture 2 Exercise 1: Non Blocking P2P Communication
// ====================================================
int exercise2_1(int argc, char* argv[]);

int exercise2_2(int argc, char* argv[]);

// Lecture 2 Exercise 3: Non-blocking Communications with Neighbours
// ==================================================================

int exercise2_3(int argc, char* argv[]);

int exercise4_1(int argc, char* argv[]);

int exercise4_2(int argc, char* argv[]);

int exercise5_1(int argc, char* argv[]);

int exercise6_1(int argc, char* argv[]); 