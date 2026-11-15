#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SHIPS 20
#define MAX_CREW 6
#define MAX_EMERG 10
#define INIT_MISSIONS 4
#define PIN_ATTEMPTS 3
#define ADMIN_PIN 1234

#define MISSIONS_FILE "missions.txt"
#define SHIPS_FILE "ships.txt"
#define CREW_FILE "crew.txt"
#define EMERGENCIES_FILE "emergencies.txt"

typedef enum { M_PLANNED, M_ACTIVE, M_DONE, M_FAILED } MissionStatus;
typedef enum { E_ENGINE = 1, E_FUEL_LEAK, E_SOLAR, E_OXYGEN } EmergencyType;

typedef struct {
    int id;
    char name[30];
    int age;
    float health;
    char role[20];
} Crew;

typedef struct {
    int id;
    char name[30];
    float fuel;
    float oxygen;
    float power;
    float temp;
    float condition;
    int crewCount;
    Crew crew[MAX_CREW];
} Spacecraft;

typedef struct {
    int id;
    char destination[20];
    int spacecraftID;
    int durationDays;
    float fuelReq;
    float oxygenReq;
    float score;
    int status;
    int emergenciesHandled;
    int emergenciesTotal;
} Mission;

typedef struct {
    int type;
    int severity;
    char desc[50];
    int resolved;
} Emergency;

/* Shared global data: definitions are in main.c. */
extern Mission *missions;
extern int missionCount;
extern int missionCap;
extern Spacecraft ships[MAX_SHIPS];
extern int shipCount;
extern Emergency queue[MAX_EMERG];
extern int queueCount;

/* Member 1: input, login, menu, and persistence. */
int readInt(const char *prompt, int min, int max);
float readFloat(const char *prompt, float min, float max);
int adminLogin(void);
void mainMenu(void);
int growMissionArray(void);
int saveMissions(void);
int loadMissions(void);
int saveShips(void);
int loadShips(void);
int saveCrew(void);
int loadCrew(void);
int saveEmergencies(void);
int loadEmergencies(void);
int saveAllData(void);
int loadAllData(void);

/* Other project modules. */
void missionMenu(void);
void spacecraftMenu(void);
void emergencyMenu(void);

#endif
