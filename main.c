
/*
 * File:   main.c
 * Author: <your name here>
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LENGTH 100

typedef struct Vector Vector;
struct Vector
{
    float x;
    float y;
    float z;
};

typedef struct MoveInfo MoveInfo;
struct MoveInfo
{
    Vector velocity;
    float duration;
};

// prototypes for suggested student-implemented functions
int countInputFileLines(char inputFileName[]);
void getPointFromString(char string[], Vector *point);
void getMoveInfoBetweenPoints(MoveInfo *moveInfo,
                              Vector firstPoint,
                              Vector secondPoint);

/*
 * I, Robot Programming Assignment
 */
int main(int argc, char** argv)
{
    // IMPORTANT: Only add code in the section
    // indicated below. The code I've provided
    // makes your solution work with the
    // automated grader on Coursera

    char rawFileName[MAX_LENGTH];
    fgets(rawFileName, MAX_LENGTH, stdin);

    while (rawFileName[0] != 'q')
    {
        int stringEndIndex = -1;
        char *result = NULL;
        char *stringStart = &rawFileName[0];

        // handle Windows, MacOS, and Linux
        // check for carriage return
        result = strchr(rawFileName, '\r');

        if (result == NULL)
        {
            // no carriage return found, so find new line
            result = strchr(rawFileName, '\n');
        }

        stringEndIndex = result - stringStart;

        char* inputFileName = malloc(
            (stringEndIndex + 1) * sizeof(char)
        );

        strncpy(inputFileName, rawFileName, stringEndIndex);
        inputFileName[stringEndIndex] = '\0';

        // Add your code between this comment
        // and the comment below.

        // Count the number of points in the input file
        int numberOfPoints = countInputFileLines(inputFileName);

        // Open the input file
        FILE *inputFile = fopen(inputFileName, "r");

        if (inputFile != NULL && numberOfPoints >= 2)
        {
            // Allocate memory for the points
            Vector *points = malloc(
                numberOfPoints * sizeof(Vector)
            );

            // There is one fewer move than points
            MoveInfo *moveInfo = malloc(
                (numberOfPoints - 1) * sizeof(MoveInfo)
            );

            char line[MAX_LENGTH];

            // Read all points from the input file
            for (int i = 0; i < numberOfPoints; i++)
            {
                if (fgets(line, MAX_LENGTH, inputFile) != NULL)
                {
                    getPointFromString(line, &points[i]);
                }
            }

            fclose(inputFile);

            // Calculate movement information
            float totalDuration = 0.0f;

            for (int i = 0; i < numberOfPoints - 1; i++)
            {
                getMoveInfoBetweenPoints(
                    &moveInfo[i],
                    points[i],
                    points[i + 1]
                );

                totalDuration += moveInfo[i].duration;

                // Print velocity components and duration
                printf(
                    "%.2f,%.2f,%.2f,%.2f",
                    moveInfo[i].velocity.x,
                    moveInfo[i].velocity.y,
                    moveInfo[i].velocity.z,
                    moveInfo[i].duration
                );

                // Separate movement chunks with one space
                if (i < numberOfPoints - 2)
                {
                    printf(" ");
                }
            }

            // Print total duration
            printf(" %.2f\n", totalDuration);

            // Release allocated memory
            free(points);
            free(moveInfo);
        }
        else
        {
            if (inputFile != NULL)
            {
                fclose(inputFile);
            }
        }

        // Don't add or modify any code below
        // this comment except as indicated below

        free(inputFileName);
        inputFileName = NULL;

        fgets(rawFileName, MAX_LENGTH, stdin);
    }

    return 0;
}

// Add your function implementations between this
// comment and the comment below.

// Function 1: Count the number of lines in the input file
int countInputFileLines(char inputFileName[])
{
    FILE *inputFile = fopen(inputFileName, "r");

    if (inputFile == NULL)
    {
        return 0;
    }

    int count = 0;
    int character;
    int lastCharacter = '\n';

    while ((character = fgetc(inputFile)) != EOF)
    {
        if (character == '\n')
        {
            count++;
        }

        lastCharacter = character;
    }

    // Account for a final line without a newline character
    if (lastCharacter != '\n')
    {
        count++;
    }

    fclose(inputFile);

    return count;
}


// Function 2: Extract x, y, and z coordinates from a string
void getPointFromString(char string[], Vector *point)
{
    sscanf(
        string,
        "%f,%f,%f",
        &point->x,
        &point->y,
        &point->z
    );
}


// Function 3: Calculate velocity vector and duration
// between two consecutive points
void getMoveInfoBetweenPoints(
    MoveInfo *moveInfo,
    Vector firstPoint,
    Vector secondPoint
)
{
    // Calculate differences between coordinates
    float deltaX = secondPoint.x - firstPoint.x;
    float deltaY = secondPoint.y - firstPoint.y;
    float deltaZ = secondPoint.z - firstPoint.z;

    // Calculate the distance between the two points
    float distance = sqrtf(
        deltaX * deltaX +
        deltaY * deltaY +
        deltaZ * deltaZ
    );

    // Duration is equal to distance because
    // the robot moves at exactly 1 foot per second
    moveInfo->duration = distance;

    // Calculate the normalized velocity vector
    if (distance > 0.0f)
    {
        moveInfo->velocity.x = deltaX / distance;
        moveInfo->velocity.y = deltaY / distance;
        moveInfo->velocity.z = deltaZ / distance;
    }
    else
    {
        // Handle identical consecutive points
        moveInfo->velocity.x = 0.0f;
        moveInfo->velocity.y = 0.0f;
        moveInfo->velocity.z = 0.0f;
    }
}

// Don't add or modify any code below this comment.
