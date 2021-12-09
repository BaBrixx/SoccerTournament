/*
//  main.c
//  Soccer Tournament
//
//  Created by Bastian Brix Hansen on 3.12.2021.
//  Copyright © 2021 Bastian Brix Hansen. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Definitions */
#define FILE_PATH "kampe-2020-2021.txt"
#define MAX_WEEKDAY_SIZE 7
#define MAX_DATE_SIZE 6
#define MAX_TIME_SIZE 6
#define MAX_TEAM_NAME_LENGTH 4

/* Structs for storing data about the matches and teams */
struct {
    char weekday[MAX_WEEKDAY_SIZE];
    char date[MAX_DATE_SIZE];
    char time[MAX_TIME_SIZE];
    char teamHome[MAX_TEAM_NAME_LENGTH];
    char teamOther[MAX_TEAM_NAME_LENGTH];
    unsigned short goalsHome;
    unsigned short goalsOther;
    unsigned int spectators;
} typedef Match;

struct {
    char name[MAX_TEAM_NAME_LENGTH];
    unsigned short points;
    unsigned short goalsBy;
    unsigned short goalsConceded;
} typedef Team;

/* Predeclaring functions */
void readFile (Match **loadedMatches, int *loadedMatchesLen, Team **loadedTeams, int *loadedTeamsLen, char *filePath);
int getTeamIndex (char teamName[], Team **loadedTeams, int *loadedTeamsLen);
void printTeams(Team **loadedTeams, int *loadedTeamsLen);
int compareFunction (const void *teamA, const void *teamB);

/* Main function */
int main(void) {
    /* Declaring the arrays and length variables */
    Match *loadedMatches = NULL;
    int loadedMatchesLen = 0;
    Team *loadedTeams = NULL;
    int loadedTeamsLen = 0;

    /* Using the readFile function to load the results */
    readFile(&loadedMatches, &loadedMatchesLen, &loadedTeams, &loadedTeamsLen, FILE_PATH);
    
    printTeams(&loadedTeams, &loadedTeamsLen);

    free(loadedMatches);
    free(loadedTeams);

    return EXIT_SUCCESS;
}

/**
 * @brief This function reads and loads the data from a spefific file
 *
 * This function is used in the beginning of the main function to load the matches and teams from the FILE_PATH.
 * The function takes two pointers to the loadedMatches and loadedTeams arrays.
 * The function also takes two integer pointers to the length of the two arrays.
 * The last parameter takes a pointer to the file path
 * 
 * @param **loadedMatches
 * @param *loadedMatchesLen
 * @param **loadedTeams
 * @param *loadedTeamsLen
 * @param *filePath
 * @return void
 */
void readFile (Match **loadedMatches, int *loadedMatchesLen, Team **loadedTeams, int *loadedTeamsLen, char *filePath) {
    /* Creating variables */
    FILE *fp;

    /* Creating temporary variables */
    char tempWeekDay[MAX_WEEKDAY_SIZE];
    char tempDate[MAX_DATE_SIZE];
    char tempTime[MAX_TIME_SIZE];
    char tempHomeTeam[MAX_TEAM_NAME_LENGTH];
    char tempOtherTeam[MAX_TEAM_NAME_LENGTH];
    unsigned short tempHomeTeamGoals = 0;
    unsigned short tempOtherTeamGoals = 0;
    unsigned short tempSpectators = 0;
    Team *teamHome = NULL;
    Team *teamOther = NULL;

    /* Opening and checking whether the file loaded successfully */
    fp = fopen(filePath, "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    /* Allocating memory for the first match and team */
    *loadedMatches = (Match *) malloc(sizeof(Match));
    *loadedTeams = (Team *) malloc(sizeof(Team));

    if (loadedMatches == NULL || loadedTeams == NULL) {
        printf("Allocating memory failed. Ending program.");
        exit(0);
    }

    /* While loop running through the file line by line to load the data */
    while (fscanf(fp, " %s %s %s %s - %s %hu - %hu %hu", tempWeekDay, tempDate, tempTime, tempHomeTeam, tempOtherTeam, &tempHomeTeamGoals, &tempOtherTeamGoals, &tempSpectators) == 8) {
        int teamHomeIndex = getTeamIndex(tempHomeTeam, loadedTeams, loadedTeamsLen);
        int teamOtherIndex = getTeamIndex(tempOtherTeam, loadedTeams, loadedTeamsLen);

        /* Creating the match for the array */
        *loadedMatchesLen += 1;
        *loadedMatches = (Match *) realloc(*loadedMatches, *loadedMatchesLen * sizeof(Match));
        strcpy((*loadedMatches)[*loadedMatchesLen - 1].weekday, tempWeekDay);
        strcpy((*loadedMatches)[*loadedMatchesLen - 1].date, tempDate);
        strcpy((*loadedMatches)[*loadedMatchesLen - 1].time, tempTime);
        strcpy((*loadedMatches)[*loadedMatchesLen - 1].teamHome, tempHomeTeam);
        strcpy((*loadedMatches)[*loadedMatchesLen - 1].teamOther, tempOtherTeam);
        (*loadedMatches)[*loadedMatchesLen - 1].goalsHome = (unsigned short) tempHomeTeamGoals;
        (*loadedMatches)[*loadedMatchesLen - 1].goalsOther = (unsigned short) tempOtherTeamGoals;
        (*loadedMatches)[*loadedMatchesLen - 1].spectators = (unsigned short) tempSpectators;

        /* Adding points to the teams */
        if (tempHomeTeamGoals == tempOtherTeamGoals) {
            (*loadedTeams)[teamHomeIndex].points += (unsigned short) 1;
            (*loadedTeams)[teamOtherIndex].points += (unsigned short) 1;
        }
        else if (tempHomeTeamGoals > tempOtherTeamGoals) {
            (*loadedTeams)[teamHomeIndex].points += (unsigned short) 3;
        }
        else {
            (*loadedTeams)[teamOtherIndex].points += (unsigned short) 3;
        }
        
        /* Adding goals stats */
        (*loadedTeams)[teamHomeIndex].goalsBy += tempHomeTeamGoals;
        (*loadedTeams)[teamHomeIndex].goalsConceded += tempOtherTeamGoals;

        (*loadedTeams)[teamOtherIndex].goalsBy += tempOtherTeamGoals;
        (*loadedTeams)[teamOtherIndex].goalsConceded += tempHomeTeamGoals;
    }

    /* Sorting the team array by points and goals using qsort */
    qsort(*loadedTeams, *loadedTeamsLen, sizeof(Team), compareFunction);

    /* Closing the file */
    fclose(fp);
}

/**
 * @brief Function for getting the index of the specific team. Creating it if it was not found.
 *
 * This function is used in the readFile function to get the index of the specific team in the loadedTeams array.
 * The function takes an array of the team name, a pointer to the loadedTeams array and a pointer to the loadedTeamsLen integer.
 * The function returns an integer in the form of the index of the specific team in the loadedTeams array.
 * 
 * @param teamName[]
 * @param **loadedTeams
 * @param *loadedTeamsLen
 * @return int
 */
int getTeamIndex (char teamName[], Team **loadedTeams, int *loadedTeamsLen) {
    int teamIndex = -1;
    int currTeam = 0;

    while (currTeam < *loadedTeamsLen && teamIndex == -1) {
        if (strcmp(teamName, (*loadedTeams)[currTeam].name) == 0) {
            teamIndex = currTeam;
        }
        currTeam++;
    }

    /* Creating the specific team if it doesnt exist yet */
    if (teamIndex == -1) {
        *loadedTeamsLen += 1;
        *loadedTeams = (Team *) realloc(*loadedTeams, *loadedTeamsLen * sizeof(Team));
        if (loadedTeams == NULL) {
            printf("Reallocating memory failed. Ending program.");
            exit(0);
        }

        /* Setting the values */
        strcpy((*loadedTeams)[*loadedTeamsLen - 1].name, teamName);
        (*loadedTeams)[*loadedTeamsLen - 1].points = (unsigned short) 0;
        (*loadedTeams)[*loadedTeamsLen - 1].goalsBy = (unsigned short) 0;
        (*loadedTeams)[*loadedTeamsLen - 1].goalsConceded = (unsigned short) 0;

        teamIndex = *loadedTeamsLen - 1;
    }

    /* Returning the index of the team */
    return teamIndex;
}

/**
 * @brief This function prints the teamsArray and formats it in a userfriendly way
 *
 * The function takes a pointer to the loadedTeams array and a pointer to the loadedTeamsLen integer.
 * 
 * @param teamName[]
 * @param **loadedTeams
 * @param *loadedTeamsLen
 * @return int
 */
void printTeams(Team **loadedTeams, int *loadedTeamsLen) {
    printf("|-----------------------------------------------------|\n");
    printf("| Nr | Team | Points | Goals scored | Goals conceeded |\n");
    printf("|-----------------------------------------------------|\n");
    
    for (int i = 0; i < *loadedTeamsLen; i++) {
        printf("| %2d | %4s | %6hu | %12hu | %15hu |\n", i + 1, (*loadedTeams)[i].name, (*loadedTeams)[i].points, (*loadedTeams)[i].goalsBy, (*loadedTeams)[i].goalsConceded);
    }
    printf("|-----------------------------------------------------|\n");
}

/**
 * @brief qsort compare function to determine wich team has better results
 *
 * The function takes a pointer to the two teams that should be compared.
 * 
 * @param *teamA
 * @param *teamB
 * @return int
 */
int compareFunction (const void *teamA, const void *teamB) {
    const Team *tempTeamA = (Team *) teamA;
    const Team *tempTeamB = (Team *) teamB;

    int tempPointDiff = tempTeamB->points - tempTeamA->points;

    if (tempPointDiff == 0) {
        tempPointDiff = (tempTeamB->goalsBy - tempTeamB->goalsConceded) - (tempTeamA->goalsBy - tempTeamA->goalsConceded);
    }

    return tempPointDiff;
}
