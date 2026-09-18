#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100


const char SPECIALTIES[4][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
const double BASE_FEES[4] = {1500.00, 2500.00, 4500.00, 5000.00};
const int AVG_TIMES[4] = {15, 20, 30, 30};

const char WARDS[4][20] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};
const double WARD_RATES[4] = {3000.00, 6000.00, 12000.00, 25000.00};
const int WARD_CAPS[4] = {20, 10, 10, 5};


int patient_id[MAX_PATIENTS];
char patient_name[MAX_PATIENTS][50];
int patient_age[MAX_PATIENTS];
int patient_urgency[MAX_PATIENTS];
int patient_specialty[MAX_PATIENTS];
int patient_is_admitted[MAX_PATIENTS];
int patient_ward[MAX_PATIENTS];
int patient_days[MAX_PATIENTS];
int patient_bed_num[MAX_PATIENTS];

double patient_base_fee[MAX_PATIENTS];
double patient_surcharge[MAX_PATIENTS];
double patient_ward_cost[MAX_PATIENTS];
double patient_gross[MAX_PATIENTS];
double patient_discount[MAX_PATIENTS];
double patient_final[MAX_PATIENTS];
int patient_wait_time[MAX_PATIENTS];


int bedOccupancy[4][20] = {0};
int patient_count = 0;
int specialty_queue[4] = {0};

void registerPatient() {
    if (patient_count >= MAX_PATIENTS) {
        printf("\nError: Maximum patient capacity reached!\n");
        return;
    }

    printf("\n--- Register New Patient ---\n");
    patient_id[patient_count] = patient_count + 101;

    printf("Enter Patient Name: ");
    scanf(" %[^\n]", patient_name[patient_count]);

    printf("Enter Patient Age: ");
    scanf("%d", &patient_age[patient_count]);

    printf("Select Urgency Level (1: Low, 2: Medium, 3: High, 4: Critical Emergency): ");
    scanf("%d", &patient_urgency[patient_count]);

    printf("Select Medical Specialty:\n");
    for (int i = 0; i < 4; i++) {
        printf("  %d. %s (Base Fee: LKR %.2f, Avg Time: %d mins)\n", i + 1, SPECIALTIES[i], BASE_FEES[i], AVG_TIMES[i]);
    }
    printf("Enter choice (1-4): ");
    int spec_choice;
    scanf("%d", &spec_choice);
    patient_specialty[patient_count] = spec_choice - 1;

 
    double surcharge_percent = 0.0;
    if (patient_urgency[patient_count] == 3) surcharge_percent = 0.15;
    else if (patient_urgency[patient_count] == 4) surcharge_percent = 0.30;

    patient_base_fee[patient_count] = BASE_FEES[patient_specialty[patient_count]];
    patient_surcharge[patient_count] = patient_base_fee[patient_count] * surcharge_percent;
    
    
    patient_is_admitted[patient_count] = 0;
    patient_ward[patient_count] = -1;
    patient_days[patient_count] = 0;
    patient_bed_num[patient_count] = -1;
    patient_ward_cost[patient_count] = 0.0;

    specialty_queue[patient_specialty[patient_count]]++;

    printf("\nPatient Registered Successfully! Patient ID: %d\n", patient_id[patient_count]);
    patient_count++;
}

void admitToWard() {
    if (patient_count == 0) {
        printf("\nNo patients registered yet!\n");
        return;
    }

    int id, found_index = -1;
    printf("\nEnter Patient ID to Admit: ");
    scanf("%d", &id);

    for (int i = 0; i < patient_count; i++) {
        if (patient_id[i] == id) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        printf("\nPatient ID not found!\n");
        return;
    }

    if (patient_is_admitted[found_index] == 1) {
        printf("\nPatient is already admitted to Ward %s (Bed %d).\n", WARDS[patient_ward[found_index]], patient_bed_num[found_index] + 1);
        return;
    }

    printf("\nAvailable Wards:\n");
    for (int i = 0; i < 4; i++) {
        int occupied = 0;
        for (int b = 0; b < WARD_CAPS[i]; b++) {
            if (bedOccupancy[i][b] == 1) occupied++;
        }
        printf("  %d. %s (Daily Rate: LKR %.2f, Beds: %d/%d occupied)\n", i + 1, WARDS[i], WARD_RATES[i], occupied, WARD_CAPS[i]);
    }

    printf("Select Ward Choice (1-4): ");
    int w_choice;
    scanf("%d", &w_choice);
    w_choice--;

    int assigned_bed = -1;
    for (int b = 0; b < WARD_CAPS[w_choice]; b++) {
        if (bedOccupancy[w_choice][b] == 0) {
            assigned_bed = b;
            break;
        }
    }

    if (assigned_bed == -1) {
        printf("\nSorry, Ward %s is fully occupied!\n", WARDS[w_choice]);
        return;
    }

    printf("Enter estimated number of days: ");
    int days;
    scanf("%d", &days);

    bedOccupancy[w_choice][assigned_bed] = 1;
    patient_is_admitted[found_index] = 1;
    patient_ward[found_index] = w_choice;
    patient_bed_num[found_index] = assigned_bed;
    patient_days[found_index] = days;
    patient_ward_cost[found_index] = WARD_RATES[w_choice] * days;

    printf("\nPatient %s successfully admitted to %s, Bed #%d for %d days!\n", patient_name[found_index], WARDS[w_choice], assigned_bed + 1, days);
}

void displayPatients() {
    if (patient_count == 0) {
        printf("\nNo patients to display.\n");
        return;
    }

    printf("\n=================================== REGISTERED PATIENTS LIST ===================================\n");
    printf("%-6s | %-20s | %-4s | %-10s | %-22s | %-15s | %-8s\n", 
           "ID", "Name", "Age", "Urgency", "Specialty", "Ward Status", "Bed No");
    printf("------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < patient_count; i++) {
        char urgency_str[15];
        if (patient_urgency[i] == 1) strcpy(urgency_str, "Low");
        else if (patient_urgency[i] == 2) strcpy(urgency_str, "Medium");
        else if (patient_urgency[i] == 3) strcpy(urgency_str, "High");
        else strcpy(urgency_str, "Critical");

        char ward_info[20];
        char bed_info[10];

        if (patient_is_admitted[i] == 1) {
            strcpy(ward_info, WARDS[patient_ward[i]]);
            sprintf(bed_info, "%d", patient_bed_num[i] + 1);
        } else {
            strcpy(ward_info, "Not Admitted");
            strcpy(bed_info, "N/A");
        }

        printf("%-6d | %-20s | %-4d | %-10s | %-22s | %-15s | %-8s\n",
               patient_id[i], patient_name[i], patient_age[i], urgency_str,
               SPECIALTIES[patient_specialty[i]], ward_info, bed_info);
    }
    printf("================================================================================================\n");
}

void generateBillingAndReports() {
    if (patient_count == 0) {
        printf("\nNo data available to generate reports.\n");
        return;
    }

    double total_revenue = 0.0;

    printf("\n================================ SYSTEM BILLING & FINANCIAL REPORT ================================\n");
    for (int i = 0; i < patient_count; i++) {
        patient_gross[i] = patient_base_fee[i] + patient_surcharge[i] + patient_ward_cost[i];
        
        // Age discount calculation (Senior Citizen > 60: 10% discount)
        if (patient_age[i] >= 60) {
            patient_discount[i] = patient_gross[i] * 0.10;
        } else {
            patient_discount[i] = 0.0;
        }

        patient_final[i] = patient_gross[i] - patient_discount[i];
        total_revenue += patient_final[i];

        printf("\nPatient ID: %d | Name: %s | Age: %d\n", patient_id[i], patient_name[i], patient_age[i]);
        printf("  Base Fee       : LKR %.2f\n", patient_base_fee[i]);
        printf("  Urgency Charge : LKR %.2f\n", patient_surcharge[i]);
        printf("  Ward Cost      : LKR %.2f\n", patient_ward_cost[i]);
        printf("  Gross Total    : LKR %.2f\n", patient_gross[i]);
        printf("  Discount       : LKR %.2f\n", patient_discount[i]);
        printf("  Final Total    : LKR %.2f\n", patient_final[i]);
        printf("------------------------------------------------------------------------------------------------\n");
    }

    printf("\n--- QUEUE & ESTIMATED WAIT TIMES ---\n");
    for (int s = 0; s < 4; s++) {
        int estimated_wait = specialty_queue[s] * AVG_TIMES[s];
        printf("Specialty: %-22s | Patients In Queue: %d | Total Est. Wait Time: %d mins\n", 
               SPECIALTIES[s], specialty_queue[s], estimated_wait);
    }

    printf("\n================================================================================================\n");
    printf("TOTAL HOSPITAL REVENUE GENERATED: LKR %.2f\n", total_revenue);
    printf("================================================================================================\n");
}

void displayMenu() {
    printf("\n=== SMART HOSPITAL SYSTEM ===\n");
    printf("1. Register New Patient\n");
    printf("2. Admit Patient to Ward\n");
    printf("3. Display All Patients\n");
    printf("4. Generate Billing & Reports\n");
    printf("5. Exit\n");
    printf("Enter choice: ");
}

int main() {
    int choice = 0;
    
    while (choice != 5) {
        displayMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Try again.\n");
            while (getchar() != '\n'); 
            continue;
        }

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                admitToWard();
                break;
            case 3:
                displayPatients();
                break;
            case 4:
                generateBillingAndReports();
                break;
            case 5:
                printf("\nExiting System...\n");
                break;
            default:
                printf("\nInvalid choice. Please select 1-5.\n");
        }
    }

    return 0;
}