#include "common.h"

static void readText(const char *prompt, char *out, int size)
{
    size_t len;
    while (1) {
        printf("%s", prompt);
        if (fgets(out, size, stdin) == NULL) {
            out[0] = '\0';
            return;
        }
        len = strlen(out);
        if (len > 0 && out[len - 1] == '\n') out[len - 1] = '\0';
        else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
        }
        {
            int i;
            for (i = 0; out[i] != '\0'; i++)
                if (out[i] == '|') out[i] = '-';
        }
        if (out[0] != '\0') return;
        printf("Input cannot be empty.\n");
    }
}

static int findMission(int id)
{
    int i;
    for (i = 0; i < missionCount; i++) if (missions[i].id == id) return i;
    return -1;
}

int growMissionArray(void)
{
    int newCap = (missionCap == 0) ? INIT_MISSIONS : missionCap * 2;
    Mission *temp = (Mission *)realloc(missions, newCap * sizeof(Mission));
    if (temp == NULL) {
        printf("Error: unable to allocate memory for missions.\n");
        return 0;
    }
    missions = temp;
    missionCap = newCap;
    return 1;
}

static void addMission(void)
{
    Mission m;
    int i;
    if (missionCount == missionCap && !growMissionArray()) return;

    m.id = readInt("Enter mission ID: ", 1, 999999);
    if (findMission(m.id) != -1) {
        printf("A mission with this ID already exists.\n");
        return;
    }
    readText("Enter destination: ", m.destination, sizeof(m.destination));
    if (shipCount == 0) {
        printf("No spacecraft registered. Add a spacecraft first.\n");
        return;
    }
    printf("Registered spacecraft IDs: ");
    for (i = 0; i < shipCount; i++) printf("%d ", ships[i].id);
    printf("\n");
    m.spacecraftID = readInt("Enter assigned spacecraft ID: ", 1, 999999);
    for (i = 0; i < shipCount && ships[i].id != m.spacecraftID; i++) { }
    if (i == shipCount) {
        printf("Spacecraft ID not found. Mission was not added.\n");
        return;
    }
    m.durationDays = readInt("Enter mission duration (days): ", 1, 3650);
    m.fuelReq = readFloat("Enter required fuel (0-100000): ", 0.0f, 100000.0f);
    m.oxygenReq = readFloat("Enter required oxygen (0-100000): ", 0.0f, 100000.0f);
    m.score = 0.0f;
    m.status = M_PLANNED;
    m.emergenciesHandled = 0;
    m.emergenciesTotal = 0;
    missions[missionCount++] = m;
    printf("Mission added successfully.\n");
}

static void listMissions(void)
{
    int i;
    if (missionCount == 0) { printf("No missions available.\n"); return; }
    printf("\n%-6s %-19s %-10s %-8s %-10s %-10s %-8s\n", "ID", "Destination", "Ship ID", "Days", "Fuel", "Oxygen", "Status");
    for (i = 0; i < missionCount; i++) {
        const char *status = "PLANNED";
        if (missions[i].status == M_ACTIVE) status = "ACTIVE";
        else if (missions[i].status == M_DONE) status = "DONE";
        else if (missions[i].status == M_FAILED) status = "FAILED";
        printf("%-6d %-19s %-10d %-8d %-10.2f %-10.2f %-8s\n",
               missions[i].id, missions[i].destination, missions[i].spacecraftID,
               missions[i].durationDays, missions[i].fuelReq, missions[i].oxygenReq, status);
    }
}


static void calculateMissionScores(void)
{
    int i, j;
    if (missionCount == 0) { printf("No missions available.\n"); return; }
    for (i = 0; i < missionCount; i++) {
        int shipIndex = -1;
        float fuelScore, oxygenScore;
        for (j = 0; j < shipCount; j++)
            if (ships[j].id == missions[i].spacecraftID) { shipIndex = j; break; }
        if (shipIndex == -1) {
            missions[i].score = 0.0f;
        } else {
            if (missions[i].fuelReq <= 0.0f) fuelScore = 100.0f;
            else {
                fuelScore = ships[shipIndex].fuel / missions[i].fuelReq * 100.0f;
                if (fuelScore > 100.0f) fuelScore = 100.0f;
            }
            if (missions[i].oxygenReq <= 0.0f) oxygenScore = 100.0f;
            else {
                oxygenScore = ships[shipIndex].oxygen / missions[i].oxygenReq * 100.0f;
                if (oxygenScore > 100.0f) oxygenScore = 100.0f;
            }
            missions[i].score = (fuelScore + oxygenScore + ships[shipIndex].condition) / 3.0f;
        }
    }
    printf("Mission readiness scores calculated.\n");
    for (i = 0; i < missionCount; i++)
        printf("Mission %d (%s): %.2f/100\n", missions[i].id, missions[i].destination, missions[i].score);
}

static void sortMissionsByScore(void)
{
    int i, j;
    if (missionCount < 2) { printf("At least two missions are required to sort.\n"); return; }
    for (i = 0; i < missionCount - 1; i++) {
        for (j = 0; j < missionCount - i - 1; j++) {
            if (missions[j].score < missions[j + 1].score) {
                Mission temp = missions[j];
                missions[j] = missions[j + 1];
                missions[j + 1] = temp;
            }
        }
    }
    printf("Missions sorted by readiness score (highest first).\n");
    listMissions();
}

static void searchMission(void)
{
    int id, index;
    id = readInt("Enter mission ID to search: ", 1, 999999);
    index = findMission(id);
    if (index == -1) { printf("Mission not found.\n"); return; }
    printf("Mission ID: %d\nDestination: %s\nSpacecraft ID: %d\nDuration: %d days\nFuel required: %.2f\nOxygen required: %.2f\nScore: %.2f\nEmergencies: %d handled out of %d\n",
           missions[index].id, missions[index].destination, missions[index].spacecraftID,
           missions[index].durationDays, missions[index].fuelReq, missions[index].oxygenReq,
           missions[index].score, missions[index].emergenciesHandled, missions[index].emergenciesTotal);
}

static void updateMission(void)
{
    int id, index, status;
    id = readInt("Enter mission ID to update: ", 1, 999999);
    index = findMission(id);
    if (index == -1) { printf("Mission not found.\n"); return; }
    printf("1. Planned\n2. Active\n3. Completed\n4. Failed\n");
    status = readInt("Select new status: ", 1, 4);
    missions[index].status = status - 1;
    missions[index].score = readFloat("Enter mission score (0-100): ", 0.0f, 100.0f);
    printf("Mission updated successfully.\n");
}

static void deleteMission(void)
{
    int id, index, i;
    id = readInt("Enter mission ID to delete: ", 1, 999999);
    index = findMission(id);
    if (index == -1) { printf("Mission not found.\n"); return; }
    for (i = index; i < missionCount - 1; i++) missions[i] = missions[i + 1];
    missionCount--;
    printf("Mission deleted successfully.\n");
}

void missionMenu(void)
{
    int choice;
    do {
        printf("\n============= MISSION MANAGEMENT =============\n");
        printf("1. Add mission\n2. View all missions\n3. Search mission\n4. Update mission status/score\n5. Delete mission\n6. Calculate readiness scores\n7. Sort missions by score\n0. Return to main menu\n");
        choice = readInt("Enter your choice: ", 0, 7);
        switch (choice) {
            case 1: addMission(); break;
            case 2: listMissions(); break;
            case 3: searchMission(); break;
            case 4: updateMission(); break;
            case 5: deleteMission(); break;
            case 6: calculateMissionScores(); break;
            case 7: sortMissionsByScore(); break;
        }
    } while (choice != 0);
}
