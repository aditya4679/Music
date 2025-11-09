#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100
#define NAME_LEN     50
#define PHONE_LEN    20
#define EMAIL_LEN    50

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
} Record;

Record addressBook[MAX_RECORDS];
int recordCount = 0;

// Function prototypes
void createAddressBook();
void viewAddressBook();
void insertRecord();
void deleteRecord();
void modifyRecord();
int findRecordByName(const char *name);

int main(void) {
    int choice;

    do {
        printf("\n== Address Book Menu ==\n");
        printf("1) Create Address Book\n");
        printf("2) View Address Book\n");
        printf("3) Insert a Record\n");
        printf("4) Delete a Record\n");
        printf("5) Modify a Record\n");
        printf("6) Exit\n");
        printf("Enter your choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            // invalid input
            printf("Invalid input. Try again.\n");
            // clear input buffer
            while (getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');  // clear newline

        switch(choice) {
            case 1:
                createAddressBook();
                break;
            case 2:
                viewAddressBook();
                break;
            case 3:
                insertRecord();
                break;
            case 4:
                deleteRecord();
                break;
            case 5:
                modifyRecord();
                break;
            case 6:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please select 1-6.\n");
        }
    } while (choice != 6);

    return 0;
}

void createAddressBook() {
    recordCount = 0;
    printf("Address book created (cleared existing records).\n");
}

void viewAddressBook() {
    if (recordCount == 0) {
        printf("Address book is empty.\n");
        return;
    }
    printf("\n-- Records in Address Book --\n");
    for (int i = 0; i < recordCount; i++) {
        printf("Record %d:\n", i+1);
        printf("  Name : %s\n", addressBook[i].name);
        printf("  Phone: %s\n", addressBook[i].phone);
        printf("  Email: %s\n", addressBook[i].email);
        printf("-------------------------\n");
    }
}

void insertRecord() {
    if (recordCount >= MAX_RECORDS) {
        printf("Address book full — cannot insert new record.\n");
        return;
    }
    Record newRec;
    printf("Enter name: ");
    fgets(newRec.name, NAME_LEN, stdin);
    newRec.name[strcspn(newRec.name, "\n")] = '\0';  // remove newline

    printf("Enter phone: ");
    fgets(newRec.phone, PHONE_LEN, stdin);
    newRec.phone[strcspn(newRec.phone, "\n")] = '\0';

    printf("Enter email: ");
    fgets(newRec.email, EMAIL_LEN, stdin);
    newRec.email[strcspn(newRec.email, "\n")] = '\0';

    addressBook[recordCount++] = newRec;
    printf("Record inserted successfully.\n");
}

void deleteRecord() {
    char targetName[NAME_LEN];
    printf("Enter name of record to delete: ");
    fgets(targetName, NAME_LEN, stdin);
    targetName[strcspn(targetName, "\n")] = '\0';

    int idx = findRecordByName(targetName);
    if (idx < 0) {
        printf("Record with name \"%s\" not found.\n", targetName);
        return;
    }
    // Shift all subsequent records back by one
    for (int i = idx; i < recordCount - 1; i++) {
        addressBook[i] = addressBook[i+1];
    }
    recordCount--;
    printf("Record deleted successfully.\n");
}

void modifyRecord() {
    char targetName[NAME_LEN];
    printf("Enter name of record to modify: ");
    fgets(targetName, NAME_LEN, stdin);
    targetName[strcspn(targetName, "\n")] = '\0';

    int idx = findRecordByName(targetName);
    if (idx < 0) {
        printf("Record with name \"%s\" not found.\n", targetName);
        return;
    }
    printf("Modifying record for name \"%s\".\n", addressBook[idx].name);

    printf("Enter new phone (leave blank to keep current): ");
    char temp[PHONE_LEN];
    fgets(temp, PHONE_LEN, stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        strncpy(addressBook[idx].phone, temp, PHONE_LEN);
    }

    printf("Enter new email (leave blank to keep current): ");
    fgets(temp, EMAIL_LEN, stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        strncpy(addressBook[idx].email, temp, EMAIL_LEN);
    }

    printf("Record modified successfully.\n");
}

int findRecordByName(const char *name) {
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(addressBook[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
