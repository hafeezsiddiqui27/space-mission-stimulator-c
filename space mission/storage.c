#include "common.h"

int saveMissions(void)
{
    FILE *fp = fopen(MISSIONS_FILE, "w");
    int i;
    if (fp == NULL) return 0;
    fprintf(fp, "%d\n", missionCount);
    for (i = 0; i < missionCount; i++) {
        Mission *m = &missions[i];
        fprintf(fp, "%d|%s|%d|%d|%.2f|%.2f|%.2f|%d|%d|%d\n",
                m->id, m->destination, m->spacecraftID, m->durationDays,
                m->fuelReq, m->oxygenReq, m->score, m->status,
                m->emergenciesHandled, m->emergenciesTotal);
    }
    if (fclose(fp) != 0) return 0;
    return 1;
}

int loadMissions(void)
{
    FILE *fp = fopen(MISSIONS_FILE, "r");
    int count, i;
    if (fp == NULL) return 0;
    if (fscanf(fp, "%d", &count) != 1 || count < 0 || count > 10000) { fclose(fp); return 0; }
    if (count > 0) {
        Mission *temp = (Mission *)malloc(count * sizeof(Mission));
        if (temp == NULL) { fclose(fp); return 0; }
        for (i = 0; i < count; i++) {
            Mission *m = &temp[i];
            if (fscanf(fp, "\n%d|%19[^|]|%d|%d|%f|%f|%f|%d|%d|%d",
                       &m->id, m->destination, &m->spacecraftID, &m->durationDays,
                       &m->fuelReq, &m->oxygenReq, &m->score, &m->status,
                       &m->emergenciesHandled, &m->emergenciesTotal) != 10) {
                free(temp); fclose(fp); return 0;
            }
            if (m->status < M_PLANNED || m->status > M_FAILED) { free(temp); fclose(fp); return 0; }
        }
        free(missions);
        missions = temp;
        missionCount = count;
        missionCap = count;
    } else {
        free(missions);
        missions = NULL;
        missionCount = 0;
        missionCap = 0;
    }
    fclose(fp);
    return 1;
}

int saveShips(void)
{
    FILE *fp = fopen(SHIPS_FILE, "w");
    int i;
    if (fp == NULL) return 0;
    fprintf(fp, "%d\n", shipCount);
    for (i = 0; i < shipCount; i++) {
        Spacecraft *s = &ships[i];
        fprintf(fp, "%d|%s|%.2f|%.2f|%.2f|%.2f|%.2f\n",
                s->id, s->name, s->fuel, s->oxygen, s->power, s->temp, s->condition);
    }
    if (fclose(fp) != 0) return 0;
    return 1;
}

int loadShips(void)
{
    FILE *fp = fopen(SHIPS_FILE, "r");
    int count, i;
    if (fp == NULL) return 0;
    if (fscanf(fp, "%d", &count) != 1 || count < 0 || count > MAX_SHIPS) { fclose(fp); return 0; }
    for (i = 0; i < count; i++) {
        Spacecraft s;
        if (fscanf(fp, "\n%d|%29[^|]|%f|%f|%f|%f|%f",
                   &s.id, s.name, &s.fuel, &s.oxygen, &s.power, &s.temp, &s.condition) != 7) {
            fclose(fp); return 0;
        }
        s.crewCount = 0;
        ships[i] = s;
    }
    shipCount = count;
    fclose(fp);
    return 1;
}

int saveCrew(void)
{
    FILE *fp = fopen(CREW_FILE, "w");
    int i, j, total = 0;
    if (fp == NULL) return 0;
    for (i = 0; i < shipCount; i++) total += ships[i].crewCount;
    fprintf(fp, "%d\n", total);
    for (i = 0; i < shipCount; i++) {
        for (j = 0; j < ships[i].crewCount; j++) {
            Crew *c = &ships[i].crew[j];
            fprintf(fp, "%d|%d|%s|%d|%.2f|%s\n", ships[i].id,
                    c->id, c->name, c->age, c->health, c->role);
        }
    }
    if (fclose(fp) != 0) return 0;
    return 1;
}

int loadCrew(void)
{
    FILE *fp = fopen(CREW_FILE, "r");
    int total, i;
    if (fp == NULL) return 0;
    if (fscanf(fp, "%d", &total) != 1 || total < 0 || total > MAX_SHIPS * MAX_CREW) { fclose(fp); return 0; }
    for (i = 0; i < shipCount; i++) ships[i].crewCount = 0;
    for (i = 0; i < total; i++) {
        int shipID, j, index = -1;
        Crew c;
        if (fscanf(fp, "\n%d|%d|%29[^|]|%d|%f|%19[^\n]",
                   &shipID, &c.id, c.name, &c.age, &c.health, c.role) != 6) { fclose(fp); return 0; }
        for (j = 0; j < shipCount; j++) if (ships[j].id == shipID) { index = j; break; }
        if (index >= 0 && ships[index].crewCount < MAX_CREW)
            ships[index].crew[ships[index].crewCount++] = c;
    }
    fclose(fp);
    return 1;
}

int saveEmergencies(void)
{
    FILE *fp = fopen(EMERGENCIES_FILE, "w");
    int i;
    if (fp == NULL) return 0;
    fprintf(fp, "%d\n", queueCount);
    for (i = 0; i < queueCount; i++)
        fprintf(fp, "%d|%d|%s|%d\n", queue[i].type, queue[i].severity,
                queue[i].desc, queue[i].resolved);
    if (fclose(fp) != 0) return 0;
    return 1;
}

int loadEmergencies(void)
{
    FILE *fp = fopen(EMERGENCIES_FILE, "r");
    int count, i;
    if (fp == NULL) return 0;
    if (fscanf(fp, "%d", &count) != 1 || count < 0 || count > MAX_EMERG) { fclose(fp); return 0; }
    for (i = 0; i < count; i++) {
        Emergency e;
        if (fscanf(fp, "\n%d|%d|%49[^|]|%d", &e.type, &e.severity, e.desc, &e.resolved) != 4) { fclose(fp); return 0; }
        if (e.type < E_ENGINE || e.type > E_OXYGEN || e.severity < 1 || e.severity > 3 || (e.resolved != 0 && e.resolved != 1)) { fclose(fp); return 0; }
        queue[i] = e;
    }
    queueCount = count;
    fclose(fp);
    return 1;
}

int saveAllData(void)
{
    int a = saveMissions();
    int b = saveShips();
    int c = saveCrew();
    int d = saveEmergencies();
    return a && b && c && d;
}

int loadAllData(void)
{
    int a = loadMissions();
    int b = loadShips();
    int c = loadCrew();
    int d = loadEmergencies();
    return a && b && c && d;
}
