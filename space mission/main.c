#include "common.h"

Mission *missions = NULL;
int missionCount = 0;
int missionCap = 0;
Spacecraft ships[MAX_SHIPS];
int shipCount = 0;
Emergency queue[MAX_EMERG];
int queueCount = 0;

static void clearInputLine(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

int readInt(const char *prompt, int min, int max)
{
    int value;
    int result;
    char extra;

    while (1) {
        printf("%s", prompt);
        result = scanf("%d%c", &value, &extra);
        if (result == EOF) return min;
        if (result != 2 || extra != '\n') {
            printf("Invalid input. Please enter a whole number.\n");
            clearInputLine();
            continue;
        }
        if (value < min || value > max) {
            printf("Out of range. Enter a value from %d to %d.\n", min, max);
            continue;
        }
        return value;
    }
}

float readFloat(const char *prompt, float min, float max)
{
    float value;
    int result;
    char extra;

    while (1) {
        printf("%s", prompt);
        result = scanf("%f%c", &value, &extra);
        if (result == EOF) return min;
        if (result != 2 || extra != '\n') {
            printf("Invalid input. Please enter a numeric value.\n");
            clearInputLine();
            continue;
        }
        if (!(value >= min && value <= max)) {
            printf("Out of range. Enter a value from %.2f to %.2f.\n", min, max);
            continue;
        }
        return value;
    }
}

int adminLogin(void)
{
    int pin;
    int attempts;

    printf("\n========== SPACE MISSION CONTROL SYSTEM ==========\n");
    printf("                 ADMIN LOGIN\n");

    for (attempts = 1; attempts <= PIN_ATTEMPTS; attempts++) {
        pin = readInt("Enter admin PIN : ", 0, 999999);
        if (pin == ADMIN_PIN) {
            printf("Login successful. Welcome, Administrator!\n");
            return 1;
        }
        printf("Incorrect PIN. Attempts remaining: %d\n", PIN_ATTEMPTS - attempts);
    }

    printf("Access denied. Too many incorrect attempts.\n");
    return 0;
}

static void showDataSummary(void)
{
    printf("\n--------------- CURRENT DATA SUMMARY ---------------\n");
    printf("Missions stored     : %d\n", missionCount);
    printf("Spacecraft stored   : %d\n", shipCount);
    printf("Crew members stored : ");
    {
        int i, totalCrew = 0;
        for (i = 0; i < shipCount; i++) totalCrew += ships[i].crewCount;
        printf("%d\n", totalCrew);
    }
    printf("Emergencies queued  : %d\n", queueCount);
    printf("----------------------------------------------------\n");
}

void mainMenu(void)
{
    int choice;

    do {
        printf("\n=============== ADMIN MAIN MENU ===============\n");
        printf("1. Mission management\n");
        printf("2. Spacecraft and crew management\n");
        printf("3. Emergency management\n");
        printf("4. Show stored data summary\n");
        printf("5. Save all data\n");
        printf("6. Reload data from files\n");
        printf("0. Save and exit\n");
        printf("===============================================\n");

        choice = readInt("Enter your choice: ", 0, 6);

        switch (choice) {
            case 1: missionMenu(); break;
            case 2: spacecraftMenu(); break;
            case 3: emergencyMenu(); break;
            case 4: showDataSummary(); break;
            case 5:
                if (saveAllData()) printf("All data saved successfully.\n");
                else printf("Some data could not be saved. Check file permissions.\n");
                break;
            case 6:
                if (loadAllData()) printf("Data loaded successfully from storage files.\n");
                else printf("One or more storage files could not be loaded.\n");
                break;
            case 0:
                if (saveAllData()) printf("Data saved. Closing Space Mission Control System.\n");
                else printf("Warning: data could not be fully saved before exit.\n");
                break;
        }
    } while (choice != 0);
}

int main(void)
{
    printf("Starting Space Mission Control System...\n");
    if (!loadAllData())
        printf("One or more data files were unavailable or invalid; starting with available data.\n");

    if (adminLogin()) mainMenu();

    free(missions);
    missions = NULL;
    missionCount = 0;
    missionCap = 0;
    printf("System shut down.\n");
    return 0;
}
