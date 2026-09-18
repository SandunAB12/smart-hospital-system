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

int main() {
    printf("Smart Hospital System Starting...\n");
    return 0;
}
