#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Account structure
struct Account {
    int accNo;
    char name[50];
    float balance;
    int pin;
};

// Transaction structure
struct Transaction {
    int accNo;
    char type[10]; // Deposit / Withdraw
    float amount;
};

// Create Account
void createAccount() {
    FILE *fp = fopen("bank.dat", "ab");
    struct Account a;

    printf("Enter Account Number: ");
    scanf("%d", &a.accNo);
    printf("Enter Name: ");
    scanf("%s", a.name);
    printf("Set PIN: ");
    scanf("%d", &a.pin);
    printf("Enter Initial Balance: ");
    scanf("%f", &a.balance);

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    printf("Account Created Successfully!\n");
}

// Verify PIN
int verifyPIN(struct Account a) {
    int pin;
    printf("Enter PIN: ");
    scanf("%d", &pin);
    return (pin == a.pin);
}

// Save Transaction
void saveTransaction(int accNo, char type[], float amount) {
    FILE *fp = fopen("trans.dat", "ab");
    struct Transaction t;

    t.accNo = accNo;
    strcpy(t.type, type);
    t.amount = amount;

    fwrite(&t, sizeof(t), 1, fp);
    fclose(fp);
}

// Deposit Money
void depositMoney() {
    FILE *fp = fopen("bank.dat", "rb+");
    struct Account a;
    int accNo;
    float amount;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while(fread(&a, sizeof(a), 1, fp)) {
        if(a.accNo == accNo) {
            if(!verifyPIN(a)) {
                printf("Wrong PIN!\n");
                fclose(fp);
                return;
            }

            printf("Enter Amount: ");
            scanf("%f", &amount);

            a.balance += amount;
            fseek(fp, -sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);

            saveTransaction(accNo, "Deposit", amount);
            printf("Deposit Successful!\n");
            fclose(fp);
            return;
        }
    }

    printf("Account Not Found!\n");
    fclose(fp);
}

// Withdraw Money
void withdrawMoney() {
    FILE *fp = fopen("bank.dat", "rb+");
    struct Account a;
    int accNo;
    float amount;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while(fread(&a, sizeof(a), 1, fp)) {
        if(a.accNo == accNo) {
            if(!verifyPIN(a)) {
                printf("Wrong PIN!\n");
                fclose(fp);
                return;
            }

            printf("Enter Amount: ");
            scanf("%f", &amount);

            if(a.balance >= amount) {
                a.balance -= amount;
                fseek(fp, -sizeof(a), SEEK_CUR);
                fwrite(&a, sizeof(a), 1, fp);

                saveTransaction(accNo, "Withdraw", amount);
                printf("Withdraw Successful!\n");
            } else {
                printf("Insufficient Balance!\n");
            }
            fclose(fp);
            return;
        }
    }

    printf("Account Not Found!\n");
    fclose(fp);
}

// Check Balance
void checkBalance() {
    FILE *fp = fopen("bank.dat", "rb");
    struct Account a;
    int accNo;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    while(fread(&a, sizeof(a), 1, fp)) {
        if(a.accNo == accNo) {
            if(!verifyPIN(a)) {
                printf("Wrong PIN!\n");
                fclose(fp);
                return;
            }

            printf("Balance: %.2f\n", a.balance);
            fclose(fp);
            return;
        }
    }

    printf("Account Not Found!\n");
    fclose(fp);
}

// Transaction History
void transactionHistory() {
    FILE *fp = fopen("trans.dat", "rb");
    struct Transaction t;
    int accNo, found = 0;

    printf("Enter Account Number: ");
    scanf("%d", &accNo);

    printf("\n--- Transaction History ---\n");
    while(fread(&t, sizeof(t), 1, fp)) {
        if(t.accNo == accNo) {
            printf("%s: %.2f\n", t.type, t.amount);
            found = 1;
        }
    }

    if(!found) printf("No Transactions Found!\n");
    fclose(fp);
}

// Main Menu
int main() {
    int choice;

    do {
        printf("\n===== BANK SYSTEM (ADVANCED) =====\n");
        printf("1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Check Balance\n");
        printf("5. Transaction History\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: transactionHistory(); break;
        }

    } while(choice != 6);

    return 0;
}