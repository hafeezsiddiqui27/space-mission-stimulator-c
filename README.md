SPACE MISSION CONTROL SYSTEM
PROGRAM USER MANUAL
============================================================

1. PROGRAM OVERVIEW
-------------------
The Space Mission Control System is a menu-driven C program used to
manage spacecraft, crew members, missions and emergency reports.

The program is operated through text menus. Enter the number shown beside
the required operation. The program validates numeric input and displays
an error message when an entered value is invalid or outside its allowed
range.

2. STARTING THE PROGRAM AND ADMIN LOGIN
---------------------------------------
When the program starts, it attempts to load saved records from the four
text files in the program's working directory. It then displays the
administrator login prompt.

Default administrator PIN: 1234
Maximum login attempts: 3

At the prompt:
- Enter 1234 to log in.
- An incorrect PIN allows another attempt.
- After three incorrect PIN entries, access is denied and the program ends.

3. MAIN MENU
------------
After successful login, the program displays:

1. Mission management
2. Spacecraft and crew management
3. Emergency management
4. Show stored data summary
5. Save all data
6. Reload data from files
0. Save and exit

Enter one whole number from 0 to 6.

Option 1 opens Mission Management.
Option 2 opens Spacecraft and Crew Management.
Option 3 opens Emergency Management.
Option 4 displays the number of missions, spacecraft, crew members and
emergencies currently stored in memory.
Option 5 writes all records to the four text files.
Option 6 reloads the records from the text files and replaces the current
in-memory records with the data read from those files.
Option 0 saves all records and closes the program.

4. MISSION MANAGEMENT
---------------------
Select 1 from the main menu.

The submenu is:
1. Add mission
2. View all missions
3. Search mission
4. Update mission status/score
5. Delete mission
6. Calculate readiness scores
7. Sort missions by score
0. Return to main menu

4.1 Add mission
The program asks for:
- Mission ID: whole number from 1 to 999999; must be unique.
- Destination: non-empty text; maximum 19 characters.
- Assigned spacecraft ID: choose an ID from the registered spacecraft list.
  The spacecraft must already exist.
- Mission duration: whole number from 1 to 3650 days.
- Required fuel: numeric value from 0 to 100000.
- Required oxygen: numeric value from 0 to 100000.

A new mission starts with status PLANNED, score 0 and zero emergency
counts. Register a spacecraft before adding a mission.

4.2 View all missions
Displays the mission ID, destination, assigned spacecraft ID, duration,
fuel requirement, oxygen requirement and current status. No additional
input is required.

4.3 Search mission
Enter the mission ID (1 to 999999). If it exists, the program displays its
destination, assigned spacecraft, duration, requirements, score and
emergency counters.

4.4 Update mission status/score
Enter the existing mission ID. Select the new status:
1. Planned
2. Active
3. Completed
4. Failed

Then enter the mission score from 0 to 100.

4.5 Delete mission
Enter the ID of the mission to delete. The program removes that mission
if it exists.

4.6 Calculate readiness scores
No extra input is required. The program calculates each mission's score
using the assigned spacecraft's fuel, oxygen and condition values. The
fuel and oxygen components are capped at 100 percent, and the three
components are averaged.

4.7 Sort missions by score
No extra input is required. The program sorts missions by readiness score
from highest to lowest and displays the resulting mission list. At least
two missions are required.

5. SPACECRAFT AND CREW MANAGEMENT
----------------------------------
Select 2 from the main menu.

The submenu is:
1. Register spacecraft
2. View spacecraft
3. Update spacecraft resources
4. Assign crew member
5. View spacecraft crew
6. Delete spacecraft
7. Remove crew member
0. Return to main menu

5.1 Register spacecraft
Enter:
- Spacecraft ID: whole number from 1 to 999999; must be unique.
- Spacecraft name: non-empty text; maximum 29 characters.
- Fuel level: numeric value from 0 to 100.
- Oxygen level: numeric value from 0 to 100.
- Power level: numeric value from 0 to 100.
- Temperature: numeric value from -273 to 1000.
- Condition: numeric value from 0 to 100.

The new spacecraft starts with zero crew members. The system supports a
maximum of 20 spacecraft.

5.2 View spacecraft
Displays all registered spacecraft and their ID, name, fuel, oxygen,
power, temperature, condition and crew count. No extra input is needed.

5.3 Update spacecraft resources
Enter an existing spacecraft ID. Then enter its new:
- Fuel level: 0 to 100.
- Oxygen level: 0 to 100.
- Power level: 0 to 100.
- Temperature: -273 to 1000.
- Condition: 0 to 100.

The spacecraft ID and name remain unchanged.

5.4 Assign crew member
First enter the ID of an existing spacecraft. Then enter:
- Crew member ID: whole number from 1 to 999999; must be unique across
  all spacecraft.
- Crew member name: non-empty text; maximum 29 characters.
- Age: whole number from 18 to 100.
- Health: numeric value from 0 to 100.
- Crew role: non-empty text; maximum 19 characters.

A spacecraft can have at most 6 crew members. The program rejects an
unknown spacecraft, a duplicate crew ID or an assignment after the
spacecraft reaches its crew limit.

5.5 View spacecraft crew
Enter the spacecraft ID. The program displays the crew members assigned
to that spacecraft, including ID, name, age, health and role.

5.6 Delete spacecraft
Enter the spacecraft ID. A spacecraft cannot be deleted if an existing
mission is assigned to it. If deletion is allowed, the spacecraft and its
crew records are removed.

5.7 Remove crew member
Enter the spacecraft ID, then the crew member ID. The crew member must
belong to the selected spacecraft.

6. EMERGENCY MANAGEMENT
-----------------------
Select 3 from the main menu.

The submenu is:
1. Report emergency
2. View emergency queue
3. Resolve emergency
4. Remove resolved emergencies
0. Return to main menu

6.1 Report emergency
Select the emergency type:
1. Engine failure
2. Fuel leak
3. Solar panel failure
4. Oxygen system failure

Enter severity:
1. Low
2. Medium
3. High

Then enter a description. If the description is left empty, the program
uses the selected emergency type as the description. The queue can store
up to 10 emergency records.

6.2 View emergency queue
Displays each queued emergency, its type, severity, pending/resolved
status and description. No extra input is needed.

6.3 Resolve emergency
The program displays the queue. Enter the emergency number shown in the
list. The selected emergency is marked as resolved.

6.4 Remove resolved emergencies
No extra input is needed. All emergency records already marked as resolved
are removed from the queue.

7. INPUT VALIDATION AND GENERAL RULES
-------------------------------------
- Menu choices and integer fields require whole numbers.
- Decimal fields require numeric values.
- Numeric inputs are checked against the ranges shown in the prompts.
- Empty spacecraft, crew and mission text fields are not accepted.
- Duplicate spacecraft IDs, crew IDs and mission IDs are rejected.
- A mission must be linked to a registered spacecraft.
- A spacecraft assigned to an existing mission cannot be deleted.
- The maximum limits are 20 spacecraft, 6 crew members per spacecraft and
  10 emergency records.
- Mission records use dynamic memory allocation. The mission array grows
  when its capacity is reached.
- The vertical bar character (|) is reserved as the storage-file separator.
  Text fields containing this character are changed to a hyphen.

8. DATA FILES AND SAVING
------------------------
The program uses four readable plain-text files in its working directory:

missions.txt       Mission records.
ships.txt          Spacecraft records.
crew.txt           Crew records and their spacecraft IDs.
emergencies.txt    Emergency type, severity, description and resolution.

The first line of each file contains a record count. The following lines
contain pipe-separated record fields. The program creates or updates these
files when Save all data or Save and exit is selected.

Keep all four files in the program's working directory. If the files are
missing or invalid, the program may report that the complete dataset could
not be loaded. Use Save all data after making changes that you want to keep.

9. SUGGESTED NORMAL OPERATING SEQUENCE
--------------------------------------
1. Start the program and enter administrator PIN 1234.
2. Register a spacecraft.
3. Assign one or more crew members to the spacecraft.
4. Add a mission and select the registered spacecraft ID.
5. View missions and calculate readiness scores.
6. Update mission status/score or sort missions when needed.
7. Report emergencies and resolve them when handled.
8. Review the stored data summary.
9. Select Save all data or Save and exit.

END OF PROGRAM USER MANUAL
============================================================
