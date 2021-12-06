/*
//  main.c
//  Soccer Tournament
//
//  Created by Bastian Brix Hansen on 3.12.2021.
//  Copyright © 2021 Bastian Brix Hansen. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

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

void readFile (Match *loadedMatches, char *filePath);
void sanitize (char* string);

/* Main function */
int main(void) {
    Match *loadedMatches;

    readFile(loadedMatches, FILE_PATH);
    
    
    return EXIT_SUCCESS;
}

/* Function for reading a given file */
void readFile (Match *loadedMatches, char *filePath) {
    FILE *fp;

    /* Opening and checking whether the file loaded successfully */
    fp = fopen(filePath, "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    char tempWeekDay[MAX_WEEKDAY_SIZE];
    char tempDate[MAX_DATE_SIZE];
    char tempTime[MAX_TIME_SIZE];
    char tempHomeTeam[MAX_TEAM_NAME_LENGTH];
    char tempOtherTeam[MAX_TEAM_NAME_LENGTH];
    int tempHomeTeamGoals = 0;
    int tempOtherTeamGoals = 0;
    int tempSpectators = 0;

    /*fscanf(fp, "%3s %5s %5s %s - %3s %hu - %hu %u", tempWeekDay, tempDate, tempTime, tempHomeTeam, tempOtherTeam, &tempHomeTeamGoals, &tempOtherTeamGoals, &tempSpectators)*/ 

    while (fscanf(fp, " %s %s %s %s - %s %d - %d %d", tempWeekDay, tempDate, tempTime, tempHomeTeam, tempOtherTeam, &tempHomeTeamGoals, &tempOtherTeamGoals, &tempSpectators) == 8) {

        printf("Day: %s Date: %s Time: %s Team: %s Team2: %s Goals: %d Goals2 %d Seere: %d\n", tempWeekDay,
                                                       tempDate, tempTime, 
                                                       tempHomeTeam, tempOtherTeam, 
                                                       tempHomeTeamGoals, tempOtherTeamGoals, 
                                                       tempSpectators);

    }
    fclose(fp);
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
