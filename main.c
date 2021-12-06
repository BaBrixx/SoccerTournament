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

/* Structs for storing data about the matches and teams*/
struct {
    char weekday[MAX_WEEKDAY_SIZE];
    char date[MAX_DATE_SIZE];
    char time[MAX_TIME_SIZE];
    char teamHome[MAX_TEAM_NAME_LENGTH];
    char teamOther[MAX_TEAM_NAME_LENGTH];
    unsigned int spectators;
} typedef Match;

struct {
    char name[MAX_TEAM_NAME_LENGTH];
    unsigned short points;
    unsigned short goalsBy;
    unsigned short goalsAgainst;
} typedef Team;

void readFile (Match **loadedMatches, int *loadedMatchesLen, Team **loadedTeams, int *loadedTeamsLen, char *filePath);
int getTeamIndex (char teamName[], Team **loadedTeams, int *loadedTeamsLen);
void sanitize (char* string);

/* Main function */
int main(void) {
    Match *loadedMatches = NULL;
    int loadedMatchesLen = 0;
    Team *loadedTeams = NULL;
    int loadedTeamsLen = 0;

    readFile(&loadedMatches, &loadedMatchesLen, &loadedTeams, &loadedTeamsLen, FILE_PATH);
    
    free(loadedMatches);
    free(loadedTeams);

    return EXIT_SUCCESS;
}

/* Function for reading a given file */
void readFile (Match **loadedMatches, int *loadedMatchesLen, Team **loadedTeams, int *loadedTeamsLen, char *filePath) {
    /* Creating variables */
    FILE *fp;

    /* Creating temporary variables */
    char tempWeekDay[MAX_WEEKDAY_SIZE];
    char tempDate[MAX_DATE_SIZE];
    char tempTime[MAX_TIME_SIZE];
    char tempHomeTeam[MAX_TEAM_NAME_LENGTH];
    char tempOtherTeam[MAX_TEAM_NAME_LENGTH];
    int tempHomeTeamGoals = 0;
    int tempOtherTeamGoals = 0;
    int tempSpectators = 0;
    Team *teamHome = NULL;
    Team *teamOther = NULL;

    /* Opening and checking whether the file loaded successfully */
    fp = fopen(filePath, "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    /* Allocating memory for the first match and team*/
    *loadedMatches = (Match *) malloc(sizeof(Match));
    *loadedTeams = (Team *) malloc(sizeof(Team));

    /* While loop running through the file line by line to load the data */
    while (fscanf(fp, " %s %s %s %s - %s %d - %d %d", tempWeekDay, tempDate, tempTime, tempHomeTeam, tempOtherTeam, &tempHomeTeamGoals, &tempOtherTeamGoals, &tempSpectators) == 8) {
        printf("%d ", *loadedTeamsLen);
        int teamHomeIndex = getTeamIndex(tempHomeTeam, *loadedTeams, loadedTeamsLen);
        printf("%d ", *loadedTeamsLen);
        int teamOtherIndex = getTeamIndex(tempOtherTeam, *loadedTeams, loadedTeamsLen);
        printf("%d\n", *loadedTeamsLen);

        printf("Team 1 index: %d, Team 2 index: %d\n Team len: %d\n", teamHomeIndex, teamOtherIndex, *loadedTeamsLen);

    }

    /* Closing the file */
    fclose(fp);
}

/* Function for getting the index of the specific team. Creating it if it was not found. */
int getTeamIndex (char teamName[], Team **loadedTeams, int *loadedTeamsLen) {
    for (int i = 0; i < *loadedTeamsLen; i++) {
        if (strcmp(teamName, loadedTeams[i]->name) == 0) {
            return i;
        }
    }
    
    /* Creating the specific team */
    printf("*loadedTeamsLen += 1\n");
    *loadedTeamsLen += 1;
    printf("Reallocating memory...\n");
    *loadedTeams = (Team *) realloc(*loadedTeams, *loadedTeamsLen * sizeof(Team));
    if (loadedTeams == NULL) {
        printf("Reallocating memory failed. Ending program.");
        exit(0);
    }


    /* Setting the values */
    printf("%s, %d\n", teamName, *loadedTeamsLen);
    strcpy((*loadedTeams)[*loadedTeamsLen - 1].name, teamName);
    loadedTeams[*loadedTeamsLen - 1]->points = (unsigned short) 0;
    loadedTeams[*loadedTeamsLen - 1]->goalsBy = (unsigned short) 0;
    loadedTeams[*loadedTeamsLen - 1]->goalsAgainst = (unsigned short) 0;

    /* Returning the index of the newly created team */
    return *loadedTeamsLen - 1;
}

/**
 * @brief A function to sanitize a string
 *
 * This function removes characters that some systems cant handle.
 * The function takes a string and makes the changes directly on the string.
 *
 * @param string
 */
void sanitize(char* string){
    // Remove the newline at the end of the string, otherwise it could for example be interpreted as a delimiter for an extra category
    string[strcspn(string, "\n")] = '\0';

    // If we have a char 13 in the string, make it a end of string character.
    // This is a problem on Windows when using WSL.
    int i = 0;
    while(string[i] != '\0'){
        if (string[i] == (char) 13) string[i] = '\0';
        i++;
    }
}
