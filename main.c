#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100

const char SPECIALTIES[4][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
const float BASE_FEES[4] = {1500.00, 2500.00, 4500.00, 5000.00};
const char WARDS[4][20] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
const float WARD_RATES[4] = {3000.00, 6000.00, 12000.00, 25000.00};
const int WARD_CAPACITIES[4] = {20, 10, 10, 5};

int bedOccupancy[4][20] = {0};

char patientNames[MAX_PATIENTS][50];
int patientAges[MAX_PATIENTS];
int urgencyLevels[MAX_PATIENTS];
int assignedSpecialty[MAX_PATIENTS];
int isAdmitted[MAX_PATIENTS];
int assignedWard[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];
float finalBills[MAX_PATIENTS];

int totalPatients = 0;


void saveBedStatusToFile() {
    FILE *fp = fopen("beds_status.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for saving bed status.\n");
        return;
    }
    for (int w = 0; w < 4; w++) {
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            fprintf(fp, "%d ", bedOccupancy[w][b]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Bed statuses saved to beds_status.txt successfully.\n");
}

void loadBedStatusFromFile() {
    FILE *fp = fopen("beds_status.txt", "r");
    if (fp == NULL) {
        return; // File doesn't exist yet, proceed with default empty beds
    }
    for (int w = 0; w < 4; w++) {
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            fscanf(fp, "%d", &bedOccupancy[w][b]);
        }
    }
    fclose(fp);
    printf("Bed statuses loaded from beds_status.txt.\n");
}

float calculateSurcharge(float baseFee, int urgency) {
    if (urgency == 2) return baseFee * 0.20;
    if (urgency == 3) return baseFee * 0.50;
    return 0.0;
}

float calculateDiscount(float grossTotal, int age) {
    if (age < 5 || age > 65) return grossTotal * 0.15;
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

        int allocatedBed = -1;
        for (int b = 0; b < WARD_CAPACITIES[assignedWard[totalPatients]]; b++) {
            if (bedOccupancy[assignedWard[totalPatients]][b] == 0) {
                bedOccupancy[assignedWard[totalPatients]][b] = 1;
                allocatedBed = b + 1;
                break;
            }
        }
        if (allocatedBed != -1) {
            printf("Bed allocated successfully! Bed #%d\n", allocatedBed);
            saveBedStatusToFile();
        }

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

int main() {
    loadBedStatusFromFile();
    int choice;
    do {
        printf("\n=== SMART HOSPITAL & RESOURCE ALLOCATION SYSTEM ===\n");
        printf("1. Register Patient\n");
        printf("2. Save Bed Status to File\n");
        printf("3. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            registerPatient();
        } else if (choice == 2) {
            saveBedStatusToFile();
        }
    } while (choice != 3);

    return 0;
}
