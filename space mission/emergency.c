#include "common.h"

static const char *emergencyName(int type)
{
    switch (type) {
        case E_ENGINE: return "Engine failure";
        case E_FUEL_LEAK: return "Fuel leak";
        case E_SOLAR: return "Solar panel failure";
        case E_OXYGEN: return "Oxygen system failure";
        default: return "Unknown";
    }
}

static void addEmergency(void)
{
    Emergency e;
    int type;
    if (queueCount >= MAX_EMERG) { printf("Emergency queue is full. Resolve an emergency first.\n"); return; }
    printf("\n1. Engine failure\n2. Fuel leak\n3. Solar panel failure\n4. Oxygen system failure\n");
    type = readInt("Select emergency type: ", 1, 4);
    e.type = type;
    e.severity = readInt("Enter severity (1=Low, 2=Medium, 3=High): ", 1, 3);
    printf("Enter emergency description: ");
    if (fgets(e.desc, sizeof(e.desc), stdin) == NULL) e.desc[0] = '\0';
    else {
        size_t len = strlen(e.desc);
        if (len > 0 && e.desc[len - 1] == '\n') e.desc[len - 1] = '\0';
        else { int ch; while ((ch = getchar()) != '\n' && ch != EOF) { } }
    }
    if (e.desc[0] == '\0') strcpy(e.desc, emergencyName(type));
    {
        int i;
        for (i = 0; e.desc[i] != '\0'; i++)
            if (e.desc[i] == '|') e.desc[i] = '-';
    }
    e.resolved = 0;
    queue[queueCount++] = e;
    printf("Emergency added to the queue.\n");
}

static void listEmergencies(void)
{
    int i;
    if (queueCount == 0) { printf("No emergencies in the queue.\n"); return; }
    printf("\n%-4s %-25s %-10s %-10s %s\n", "No.", "Type", "Severity", "Status", "Description");
    for (i = 0; i < queueCount; i++)
        printf("%-4d %-25s %-10s %-10s %s\n", i + 1, emergencyName(queue[i].type),
               queue[i].severity == 1 ? "Low" : queue[i].severity == 2 ? "Medium" : "High",
               queue[i].resolved ? "Resolved" : "Pending", queue[i].desc);
}

static void resolveEmergency(void)
{
    int number;
    if (queueCount == 0) { printf("There are no emergencies to resolve.\n"); return; }
    listEmergencies();
    number = readInt("Enter emergency number to resolve: ", 1, queueCount);
    if (queue[number - 1].resolved) printf("This emergency is already resolved.\n");
    else {
        queue[number - 1].resolved = 1;
        printf("Emergency marked as resolved.\n");
    }
}

static void removeResolved(void)
{
    int i, j, removed = 0;
    for (i = 0; i < queueCount; ) {
        if (queue[i].resolved) {
            for (j = i; j < queueCount - 1; j++) queue[j] = queue[j + 1];
            queueCount--;
            removed++;
        } else i++;
    }
    printf("%d resolved emergency record(s) removed.\n", removed);
}

void emergencyMenu(void)
{
    int choice;
    do {
        printf("\n============= EMERGENCY MANAGEMENT =============\n");
        printf("1. Report emergency\n2. View emergency queue\n3. Resolve emergency\n4. Remove resolved emergencies\n0. Return to main menu\n");
        choice = readInt("Enter your choice: ", 0, 4);
        switch (choice) {
            case 1: addEmergency(); break;
            case 2: listEmergencies(); break;
            case 3: resolveEmergency(); break;
            case 4: removeResolved(); break;
        }
    } while (choice != 0);
}
