#include "common.h"

static void readText(const char *prompt, char *out, int size)
{
    size_t len;
    while (1) {
        printf("%s", prompt);
        if (fgets(out, size, stdin) == NULL) { out[0] = '\0'; return; }
        len = strlen(out);
        if (len > 0 && out[len - 1] == '\n') out[len - 1] = '\0';
        else { int ch; while ((ch = getchar()) != '\n' && ch != EOF) { } }
        {
            int i;
            for (i = 0; out[i] != '\0'; i++)
                if (out[i] == '|') out[i] = '-';
        }
        if (out[0] != '\0') return;
        printf("Input cannot be empty.\n");
    }
}

static int findShip(int id)
{
    int i;
    for (i = 0; i < shipCount; i++) if (ships[i].id == id) return i;
    return -1;
}

static void addSpacecraft(void)
{
    Spacecraft s;
    if (shipCount >= MAX_SHIPS) { printf("Maximum spacecraft limit reached.\n"); return; }
    s.id = readInt("Enter spacecraft ID: ", 1, 999999);
    if (findShip(s.id) != -1) { printf("This spacecraft ID already exists.\n"); return; }
    readText("Enter spacecraft name: ", s.name, sizeof(s.name));
    s.fuel = readFloat("Enter fuel level (0-100): ", 0.0f, 100.0f);
    s.oxygen = readFloat("Enter oxygen level (0-100): ", 0.0f, 100.0f);
    s.power = readFloat("Enter power level (0-100): ", 0.0f, 100.0f);
    s.temp = readFloat("Enter temperature (-273-1000): ", -273.0f, 1000.0f);
    s.condition = readFloat("Enter condition (0-100): ", 0.0f, 100.0f);
    s.crewCount = 0;
    ships[shipCount++] = s;
    printf("Spacecraft registered successfully.\n");
}

static void listSpacecraft(void)
{
    int i;
    if (shipCount == 0) { printf("No spacecraft registered.\n"); return; }
    printf("\n%-6s %-24s %-8s %-8s %-8s %-10s %-10s %-6s\n", "ID", "Name", "Fuel", "Oxygen", "Power", "Temp", "Condition", "Crew");
    for (i = 0; i < shipCount; i++)
        printf("%-6d %-24s %-8.1f %-8.1f %-8.1f %-10.1f %-10.1f %-6d\n",
               ships[i].id, ships[i].name, ships[i].fuel, ships[i].oxygen,
               ships[i].power, ships[i].temp, ships[i].condition, ships[i].crewCount);
}

static void addCrew(void)
{
    int id, index;
    Crew c;
    id = readInt("Enter spacecraft ID for crew assignment: ", 1, 999999);
    index = findShip(id);
    if (index == -1) { printf("Spacecraft not found.\n"); return; }
    if (ships[index].crewCount >= MAX_CREW) { printf("This spacecraft already has the maximum of %d crew members.\n", MAX_CREW); return; }
    c.id = readInt("Enter crew member ID: ", 1, 999999);
    {
        int i, j;
        for (i = 0; i < shipCount; i++)
            for (j = 0; j < ships[i].crewCount; j++)
                if (ships[i].crew[j].id == c.id) { printf("This crew ID already exists.\n"); return; }
    }
    readText("Enter crew member name: ", c.name, sizeof(c.name));
    c.age = readInt("Enter age (18-100): ", 18, 100);
    c.health = readFloat("Enter health (0-100): ", 0.0f, 100.0f);
    readText("Enter crew role: ", c.role, sizeof(c.role));
    ships[index].crew[ships[index].crewCount++] = c;
    printf("Crew member assigned successfully.\n");
}

static void viewCrew(void)
{
    int id, index, i;
    id = readInt("Enter spacecraft ID: ", 1, 999999);
    index = findShip(id);
    if (index == -1) { printf("Spacecraft not found.\n"); return; }
    if (ships[index].crewCount == 0) { printf("No crew assigned to this spacecraft.\n"); return; }
    printf("\nCrew assigned to %s (ID %d):\n", ships[index].name, ships[index].id);
    for (i = 0; i < ships[index].crewCount; i++)
        printf("ID: %d | Name: %s | Age: %d | Health: %.1f | Role: %s\n",
               ships[index].crew[i].id, ships[index].crew[i].name,
               ships[index].crew[i].age, ships[index].crew[i].health,
               ships[index].crew[i].role);
}

static void updateSpacecraft(void)
{
    int id, index;
    id = readInt("Enter spacecraft ID to update: ", 1, 999999);
    index = findShip(id);
    if (index == -1) { printf("Spacecraft not found.\n"); return; }
    ships[index].fuel = readFloat("New fuel level (0-100): ", 0.0f, 100.0f);
    ships[index].oxygen = readFloat("New oxygen level (0-100): ", 0.0f, 100.0f);
    ships[index].power = readFloat("New power level (0-100): ", 0.0f, 100.0f);
    ships[index].temp = readFloat("New temperature (-273-1000): ", -273.0f, 1000.0f);
    ships[index].condition = readFloat("New condition (0-100): ", 0.0f, 100.0f);
    printf("Spacecraft resources and condition updated.\n");
}


static void deleteSpacecraft(void)
{
    int id, index, i;
    id = readInt("Enter spacecraft ID to delete: ", 1, 999999);
    index = findShip(id);
    if (index == -1) { printf("Spacecraft not found.\n"); return; }
    for (i = 0; i < missionCount; i++) {
        if (missions[i].spacecraftID == id) {
            printf("Cannot delete: this spacecraft is assigned to mission %d.\n", missions[i].id);
            return;
        }
    }
    for (i = index; i < shipCount - 1; i++) ships[i] = ships[i + 1];
    shipCount--;
    printf("Spacecraft and its assigned crew records deleted successfully.\n");
}

static void removeCrew(void)
{
    int shipID, crewID, index, i;
    shipID = readInt("Enter spacecraft ID: ", 1, 999999);
    index = findShip(shipID);
    if (index == -1) { printf("Spacecraft not found.\n"); return; }
    crewID = readInt("Enter crew member ID to remove: ", 1, 999999);
    for (i = 0; i < ships[index].crewCount; i++)
        if (ships[index].crew[i].id == crewID) break;
    if (i == ships[index].crewCount) { printf("Crew member not found on this spacecraft.\n"); return; }
    for (; i < ships[index].crewCount - 1; i++) ships[index].crew[i] = ships[index].crew[i + 1];
    ships[index].crewCount--;
    printf("Crew member removed successfully.\n");
}

void spacecraftMenu(void)
{
    int choice;
    do {
        printf("\n========== SPACECRAFT & CREW MANAGEMENT ==========\n");
        printf("1. Register spacecraft\n2. View spacecraft\n3. Update spacecraft resources\n4. Assign crew member\n5. View spacecraft crew\n6. Delete spacecraft\n7. Remove crew member\n0. Return to main menu\n");
        choice = readInt("Enter your choice: ", 0, 7);
        switch (choice) {
            case 1: addSpacecraft(); break;
            case 2: listSpacecraft(); break;
            case 3: updateSpacecraft(); break;
            case 4: addCrew(); break;
            case 5: viewCrew(); break;
            case 6: deleteSpacecraft(); break;
            case 7: removeCrew(); break;
        }
    } while (choice != 0);
}
