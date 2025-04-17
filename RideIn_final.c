#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<limits.h>
#include<time.h>

#define MAX_RIDES 100
#define MAX_USERS 100
#define MAX_VEHICLES 100
#define NAME_LEN 50
#define USERNAME_LEN 30
#define PASSWORD_LEN 30
#define DATA_FILE "rides.txt"
#define USER_FILE "users.txt"

typedef struct {
    int id;
    char driverName[NAME_LEN];
    int rideType;
    int availableRide;
} Ride;

typedef struct {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
} User;

typedef struct {
    int id;
    char registrationNumber[NAME_LEN];
    char model[NAME_LEN];
    int capacity;
} Vehicle;

Ride rides[MAX_RIDES];
User users[MAX_USERS];
int totalRides = 0;
int totalUsers = 0;


void loadRideDetails();
void saveRides();
void viewRides();
void addRide();
void updateRide();
void deleteRide();
void bookRide();
void cancelBooking();
int compareStrings(const char *str1, const char *str2);
void copyString(char *d,  char *s);
void loadUsers();
void saveUsersToFile();
int registerUser();
int loginUser();
void displayMenu();
void userMenu();
void adminMenu();
Vehicle vehicles[MAX_VEHICLES];
int totalVehicles = 0;

void loadVehicles();
void saveVehicles();
void viewVehicles();
void addVehicle();
void updateVehicle();
void deleteVehicle();

void loadRideDetails() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        printf("error");
        exit(1);
    }

    while (fscanf(file, "%d %s %d %d", &rides[totalRides].id, rides[totalRides].driverName, &rides[totalRides].rideType, &rides[totalRides].availableRide) != EOF) {
        totalRides++;

    }
    fclose(file);
}


void saveRides() {
    FILE *file = fopen(DATA_FILE, "w");
    for (int i = 0; i < totalRides; i++) {
        fprintf(file, "%d %s %d %d\n", rides[i].id, rides[i].driverName, rides[i].rideType, rides[i].availableRide);
    }
    fclose(file);
}


void viewRides() {
    printf("Available Rides:\n");
    for (int i = 0; i < totalRides; i++) {
        if (rides[i].availableRide) {
            printf("Ride ID: %d, Driver: %s, Type: ", rides[i].id, rides[i].driverName);
            if (rides[i].rideType == 1) {
                printf("Cab\n");
            }
            else if (rides[i].rideType == 2) {
                printf("Bike\n");
            }
            else if (rides[i].rideType == 3) {
                printf("Auto\n");
            }
        }
    }
}


void addRide() {
    if (totalRides >= MAX_RIDES) {
        printf("Ride limit reached.\n");
        return;
    }

    rides[totalRides].id = totalRides + 1;

    printf("Enter ride type (1 for Cab, 2 for Bike, 3 for Auto): ");
    int rideType;
    scanf("%d", &rideType);
    rides[totalRides].rideType = rideType;
    getchar();

    printf("Enter driver name: ");
    fgets(rides[totalRides].driverName, NAME_LEN, stdin);

    size_t len = strlen(rides[totalRides].driverName);
    if (len > 0 && rides[totalRides].driverName[len - 1] == '\n') {
        rides[totalRides].driverName[len - 1] = '\0';
    }

    rides[totalRides].availableRide = 1;
    totalRides++;
    saveRides();
    printf("Ride added successfully!\n");
}


void updateRide() {
    int rideId;
    printf("Enter Ride ID to update: ");
    scanf("%d", &rideId);

    if (rideId <= 0 || rideId > totalRides) {
        printf("Invalid Ride ID.\n");
        return;
    }

    getchar();
    printf("Enter new driver name: ");
    fgets(rides[rideId - 1].driverName, NAME_LEN, stdin);

    size_t len = strlen(rides[rideId - 1].driverName);
    if (len > 0 && rides[rideId - 1].driverName[len - 1] == '\n') {
        rides[rideId - 1].driverName[len - 1] = '\0';
    }

    saveRides();
    printf("Ride updated successfully!\n");
}


void deleteRide() {
    int rideId;
    printf("Enter Ride ID to delete: ");
    scanf("%d", &rideId);

    if (rideId <= 0 || rideId > totalRides) {
        printf("Invalid Ride ID.\n");
        return;
    }

    for (int i = rideId - 1; i < totalRides - 1; i++) {
        rides[i] = rides[i + 1];
    }

    totalRides--;
    saveRides();
    printf("Ride deleted successfully!\n");
}

void loadVehicles() {
    FILE *file = fopen("vehicles.txt", "r");
    if (file == NULL) {
        printf("Vehicle data file not found, starting fresh.\n");
        return;
    }

    while (fscanf(file, "%d %s %s %d", &vehicles[totalVehicles].id, vehicles[totalVehicles].registrationNumber, vehicles[totalVehicles].model, &vehicles[totalVehicles].capacity) != EOF) {
        totalVehicles++;
    }
    fclose(file);
}

void saveVehicles() {
    FILE *file = fopen("vehicles.txt", "w");
    for (int i = 0; i < totalVehicles; i++) {
        fprintf(file, "%d %s %s %d\n", vehicles[i].id, vehicles[i].registrationNumber, vehicles[i].model, vehicles[i].capacity);
    }
    fclose(file);
}

void viewVehicles() {
    printf("\n--- Vehicle Information ---\n");
    if (totalVehicles == 0) {
        printf("No vehicles available.\n");
        return;
    }

    for (int i = 0; i < totalVehicles; i++) {
        printf("ID: %d, Registration: %s, Model: %s, Capacity: %d\n",
               vehicles[i].id, vehicles[i].registrationNumber, vehicles[i].model, vehicles[i].capacity);
    }
}

void addVehicle() {
    if (totalVehicles >= MAX_VEHICLES) {
        printf("Vehicle limit reached.\n");
        return;
    }

    vehicles[totalVehicles].id = totalVehicles + 1;

    printf("Enter registration number: ");
    scanf("%s", vehicles[totalVehicles].registrationNumber);

    printf("Enter vehicle model: ");
    scanf("%s", vehicles[totalVehicles].model);

    printf("Enter seating capacity: ");
    scanf("%d", &vehicles[totalVehicles].capacity);

    totalVehicles++;
    saveVehicles();
    printf("Vehicle added successfully!\n");
}

void updateVehicle() {
    int vehicleId;
    printf("Enter Vehicle ID to update: ");
    scanf("%d", &vehicleId);

    if (vehicleId <= 0 || vehicleId > totalVehicles) {
        printf("Invalid Vehicle ID.\n");
        return;
    }

    printf("Enter new registration number: ");
    scanf("%s", vehicles[vehicleId - 1].registrationNumber);

    printf("Enter new model: ");
    scanf("%s", vehicles[vehicleId - 1].model);

    printf("Enter new seating capacity: ");
    scanf("%d", &vehicles[vehicleId - 1].capacity);

    saveVehicles();
    printf("Vehicle updated successfully!\n");
}

void deleteVehicle() {
    int vehicleId;
    printf("Enter Vehicle ID to delete: ");
    scanf("%d", &vehicleId);

    if (vehicleId <= 0 || vehicleId > totalVehicles) {
        printf("Invalid Vehicle ID.\n");
        return;
    }

    for (int i = vehicleId - 1; i < totalVehicles - 1; i++) {
        vehicles[i] = vehicles[i + 1];
    }

    totalVehicles--;
    saveVehicles();
    printf("Vehicle deleted successfully!\n");
}

void vehicleMenu() {
    int choice;
    while (1) {
        printf("\n--- Vehicle Information Menu ---\n");
        printf("1. View Vehicles\n");
        printf("2. Add Vehicle\n");
        printf("3. Update Vehicle\n");
        printf("4. Delete Vehicle\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            viewVehicles();
        } else if (choice == 2) {
            addVehicle();
        } else if (choice == 3) {
            updateVehicle();
        } else if (choice == 4) {
            deleteVehicle();
        } else if (choice == 5) {
            printf("Exiting Vehicle Menu...\n");
            return;
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }
}

void bookRide() {
    int rideId;
    printf("Enter Ride ID to book: ");
    scanf("%d", &rideId);

    if (rideId > 0 && rideId <= totalRides && rides[rideId - 1].availableRide) {
        rides[rideId - 1].availableRide = 0;
        saveRides();
        printf("Ride booked successfully! Driver: %s, Type: ", rides[rideId - 1].driverName);
        if (rides[rideId - 1].rideType == 1) {
            printf("Cab\n");
        }
        else if (rides[rideId - 1].rideType == 2) {
            printf("Bike\n");
        }
        else if (rides[rideId - 1].rideType == 3) {
            printf("Auto\n");
        }
        printf("Your ride is on the way!\n");
    }
     else {
        printf("Invalid ride ID or ride is already booked!\n");
    }
}


void cancelBooking() {
    int rideId;
    printf("Enter Ride ID to cancel booking: ");
    scanf("%d", &rideId);

    if (rideId > 0 && rideId <= totalRides && !rides[rideId - 1].availableRide) {
        rides[rideId - 1].availableRide = 1;
        saveRides();
        printf("Booking cancelled successfully! Driver: %s\n", rides[rideId - 1].driverName);
    }
     else {
        printf("Invalid ride ID or no booking found!\n");
    }
}


int compareStrings(const char *str1, const char *str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    return str1[i] == str2[i];
}


void copyString(char *d,  char *s) {
    int i = 0;
    while (s[i] != '\0') {
        d[i] = s[i];
        i++;
    }
    d[i] = '\0';
}


void loadUsers() {
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
       printf("error");
       exit(1);
    }

    while (fscanf(file, "%s %s", users[totalUsers].username, users[totalUsers].password) != EOF) {
        totalUsers++;
    }
    fclose(file);
}


void saveUsersToFile() {
    FILE *file = fopen(USER_FILE, "w");
    for (int i = 0; i < totalUsers; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }
    fclose(file);
}


int registerUser() {
    if (totalUsers >= MAX_USERS) {
        printf("User limit reached.\n");
        return 0;
    }

    printf("Enter username: ");
    char username[USERNAME_LEN];
    scanf("%s", username);
    copyString(users[totalUsers].username, username);

    printf("Enter password: ");
    char password[PASSWORD_LEN];
    scanf("%s", password);
    copyString(users[totalUsers].password, password);

    totalUsers++;
    saveUsersToFile();
    printf("Registration successful!\n");
    return 1;
}


int loginUser() {
    char username[USERNAME_LEN], password[PASSWORD_LEN];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < totalUsers; i++) {
        if (compareStrings(users[i].username, username) && compareStrings(users[i].password, password)) {
            printf("Login successful!\n");
            return 1;
        }
    }
    printf("Invalid username or password!\n");
    return 0;
}


void displayMenu() {
    printf("\n--- Ride Booking System ---\n");
    printf("1. View Rides\n");
    printf("2. Book Ride\n");
    printf("3. Cancel Booking\n");
    printf("4. Exit\n");
}


void userMenu() {
    int choice;
    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            viewRides();
        }
        else if (choice == 2) {
            bookRide();
        }
        else if (choice == 3) {
            cancelBooking();
        }
        else if (choice == 4) {
            printf("Exiting...\n");
            return;
        }
        else {
            printf("Invalid choice! Please try again.\n");
        }
    }
}


void adminMenu() {
    int choice;
    while (1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. View Rides\n");
        printf("2. Add Ride\n");
        printf("3. Update Ride\n");
        printf("4. Delete Ride\n");
        printf("5. Vehicle Information\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            viewRides();
        } else if (choice == 2) {
            addRide();
        } else if (choice == 3) {
            updateRide();
        } else if (choice == 4) {
            deleteRide();
        } else if (choice == 5) {
            vehicleMenu();
        } else if (choice == 6) {
            printf("Exiting...\n");
            return;
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }
}

int main() {
    int login = 0;
    int admin = 0;

    loadRideDetails();
    loadUsers();

    while (1) {
        printf("\n--- Welcome to RideIn ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            registerUser();
        }
        else if (choice == 2) {
            if (loginUser()) {
                login = 1;
                printf("Are you an admin? (1 for Yes, 0 for No): ");
                scanf("%d", &admin);
                if (admin) {
                    adminMenu();
                }
                else {
                    userMenu();
                }
            }
        }
        else if (choice == 3) {
            printf("Exiting...\n");
            printf("Thanks for using RideIn\n");
            return 0;
        }
        else {
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

