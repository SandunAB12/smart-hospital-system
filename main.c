#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100


const char SPECIALTIES[4][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
const float BASE_FEES[4] = {1500.00, 2500.00, 4500.00, 5000.00};
const int AVG_TIME[4] = {15, 20, 30, 30};

const char WARDS[4][20] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
const float WARD_RATES[4] = {3000.00, 6000.00, 12000.00, 25000.00};

int bedOccupancy[4][20] = {0};
int queueCounts[4] = {0};


char patientNames[MAX_PATIENTS][50];
int patientAges[MAX_PATIENTS];
int urgencyLevels[MAX_PATIENTS];
int assignedSpecialty[MAX_PATIENTS];
int isAdmitted[MAX_PATIENTS];
int assignedWard[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];
float finalBills[MAX_PATIENTS];

int totalPatients = 0;

float calculateSurcharge(float baseFee, int urgency) {
    if (urgency == 2) return baseFee * 0.20;
    if (urgency == 3) return baseFee * 0.50;
    return 0.0;
}

float calculateDiscount(float grossTotal, int age) {
    if (age < 5 || age > 65) {
        return grossTotal * 0.15;
    }
    return 0.0;
}

void registerPatient() {
    if (totalPatients >= MAX_PATIENTS) {
        printf("\nSystem Full! Cannot register more patients.\n");
        return;
    }

    printf("\n=== PATIENT INTAKE & REGISTRATION ===\n");
    printf("Enter Patient Name: ");
    scanf(" %[^\n]", patientNames[totalPatients]);

    printf("Enter Patient Age: ");
    scanf("%d", &patientAges[totalPatients]);

    printf("Select Urgency Level (1 = Normal, 2 = Urgent, 3 = Critical): ");
    scanf("%d", &urgencyLevels[totalPatients]);

    printf("\n--- Specialty Selection ---\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s (LKR %.2f)\n", i + 1, SPECIALTIES[i], BASE_FEES[i]);
    }
    printf("Select Specialty ID (1-4): ");
    scanf("%d", &assignedSpecialty[totalPatients]);
    assignedSpecialty[totalPatients]--;

    queueCounts[assignedSpecialty[totalPatients]]++;

    printf("\nAdmit to Ward? (1 = Yes, 0 = No): ");
    scanf("%d", &isAdmitted[totalPatients]);

    if (isAdmitted[totalPatients] == 1) {
        printf("\n--- Ward Selection ---\n");
        for (int i = 0; i < 4; i++) {
            printf("%d. %s (LKR %.2f/day)\n", i + 1, WARDS[i], WARD_RATES[i]);
        }
        printf("Select Ward ID (1-4): ");
        scanf("%d", &assignedWard[totalPatients]);
        assignedWard[totalPatients]--;

        printf("Enter Days Admitted: ");
        scanf("%d", &daysAdmitted[totalPatients]);
    } else {
        assignedWard[totalPatients] = -1;
        daysAdmitted[totalPatients] = 0;
    }

    float baseFee = BASE_FEES[assignedSpecialty[totalPatients]];
    float surcharge = calculateSurcharge(baseFee, urgencyLevels[totalPatients]);
    float wardCost = (isAdmitted[totalPatients] == 1) ? (daysAdmitted[totalPatients] * WARD_RATES[assignedWard[totalPatients]]) : 0.0;
    float grossTotal = baseFee + surcharge + wardCost;
    float discount = calculateDiscount(grossTotal, patientAges[totalPatients]);
    finalBills[totalPatients] = grossTotal - discount;

    printf("\nPatient Registered Successfully! ID: PAT-%d\n", 1001 + totalPatients);
    totalPatients++;
}


void sortPatientsByPriority() {
    if (totalPatients <= 1) return;

    for (int i = 0; i < totalPatients - 1; i++) {
        for (int j = 0; j < totalPatients - i - 1; j++) {

            if (urgencyLevels[j] < urgencyLevels[j + 1]) {

                int tempUrgency = urgencyLevels[j];
                urgencyLevels[j] = urgencyLevels[j + 1];
                urgencyLevels[j + 1] = tempUrgency;


                char tempName[50];
                strcpy(tempName, patientNames[j]);
                strcpy(patientNames[j], patientNames[j + 1]);
                strcpy(patientNames[j + 1], tempName);


                int tempAge = patientAges[j];
                patientAges[j] = patientAges[j + 1];
                patientAges[j + 1] = tempAge;


                int tempSpec = assignedSpecialty[j];
                assignedSpecialty[j] = assignedSpecialty[j + 1];
                assignedSpecialty[j + 1] = tempSpec;


                int tempAdm = isAdmitted[j];
                isAdmitted[j] = isAdmitted[j + 1];
                isAdmitted[j + 1] = tempAdm;

                int tempWard = assignedWard[j];
                assignedWard[j] = assignedWard[j + 1];
                assignedWard[j + 1] = tempWard;

                int tempDays = daysAdmitted[j];
                daysAdmitted[j] = daysAdmitted[j + 1];
                daysAdmitted[j + 1] = tempDays;


                float tempBill = finalBills[j];
                finalBills[j] = finalBills[j + 1];
                finalBills[j + 1] = tempBill;
            }
        }
    }
}

void displaySortedQueue() {
    if (totalPatients == 0) {
        printf("\nNo patients registered yet.\n");
        return;
    }

    sortPatientsByPriority();

    printf("\n=== PRIORITY SORTED PATIENT QUEUE (Critical Cases First) ===\n");
    printf("%-20s %-5s %-15s %-22s %-12s\n", "Name", "Age", "Urgency", "Specialty", "Final Bill");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < totalPatients; i++) {
        char urgencyStr[15];
        if (urgencyLevels[i] == 3) strcpy(urgencyStr, "3 (Critical)");
        else if (urgencyLevels[i] == 2) strcpy(urgencyStr, "2 (Urgent)");
        else strcpy(urgencyStr, "1 (Normal)");

        printf("%-20s %-5d %-15s %-22s LKR %-10.2f\n",
               patientNames[i], patientAges[i], urgencyStr,
               SPECIALTIES[assignedSpecialty[i]], finalBills[i]);
    }
}

int main() {
    int choice;
    do {
        printf("\n=== SMART HOSPITAL & RESOURCE ALLOCATION SYSTEM ===\n");
        printf("1. Register Patient\n");
        printf("2. View Priority Sorted Queue\n");
        printf("3. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            registerPatient();
        } else if (choice == 2) {
            displaySortedQueue();
        }
    } while (choice != 3);

    return 0;
}
