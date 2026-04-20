#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define MEMORY_SIZE 100
#define PAGE_SIZE 10
#define TIME_QUANTUM 3
#define MAX_TRANSACTIONS 100
#define MAX_ACCOUNTS 50
struct Account {
    int id;
    double balance;
    pthread_mutex_t mutex;
};
struct Transaction {
    int id;
    int account_id;
    char type[10];
    double amount;
    int duration; // Remaining time
};
struct MemoryPage {
    int page_id;
    struct Transaction* transaction;
};
// Global Variables
struct Account accounts[MAX_ACCOUNTS];
int account_count = 0;
struct Transaction transaction_log[MAX_TRANSACTIONS];
int transaction_count = 0;
struct MemoryPage memory_map[MEMORY_SIZE / PAGE_SIZE];
int memory_size = MEMORY_SIZE / PAGE_SIZE;
// Mutexes
pthread_mutex_t account_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t memory_mutex = PTHREAD_MUTEX_INITIALIZER;
// Function Prototypes
void create_account(int account_id, double initial_balance);
void deposit(int account_id, double amount);
void withdraw(int account_id, double amount);
void allocate_memory(struct Transaction* transaction);
void free_memory(int transaction_id);
void run_round_robin_scheduler();
void visualize_memory_heap();
void visualize_gantt_chart();
void show_history();
int main() {
    int choice;
    int account_id;
    double amount;
    // Initialize Memory
    memset(memory_map, 0, sizeof(memory_map));
    while (1) {
        printf("\nMenu:\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Show Transaction History and Reports\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter Account ID: ");
            scanf("%d", &account_id);
            printf("Enter Initial Balance: ");
            scanf("%lf", &amount);
            create_account(account_id, amount);
            break;
        case 2:
            printf("Enter Account ID: ");
            scanf("%d", &account_id);
            printf("Enter Deposit Amount: ");
            scanf("%lf", &amount);
            deposit(account_id, amount);
            break;
        case 3:
            printf("Enter Account ID: ");
            scanf("%d", &account_id);
            printf("Enter Withdrawal Amount: ");
            scanf("%lf", &amount);
            withdraw(account_id, amount);
            break;
        case 4:
            show_history();
            break;
        case 0:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice!\n");
        }
    }
    return 0;
}
// Core Functions
void create_account(int account_id, double initial_balance) {
    pthread_mutex_lock(&account_mutex);
    accounts[account_count].id = account_id;
    accounts[account_count].balance = initial_balance;
    pthread_mutex_init(&accounts[account_count].mutex, NULL);
    account_count++;
    pthread_mutex_unlock(&account_mutex);
    printf("Account %d created with balance %.2f\n", account_id, initial_balance);
    // Simulate round-robin after account creation
    run_round_robin_scheduler();
}
void deposit(int account_id, double amount) {
    pthread_mutex_lock(&account_mutex);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == account_id) {
            pthread_mutex_lock(&accounts[i].mutex);
            accounts[i].balance += amount;
            pthread_mutex_unlock(&accounts[i].mutex);
            pthread_mutex_unlock(&account_mutex);
            printf("Deposited %.2f to account %d\n", amount, account_id);
            // Simulate a transaction
            struct Transaction t = {transaction_count++, account_id, "deposit", amount, rand() % 10 + 1};
            transaction_log[transaction_count - 1] = t;
            allocate_memory(&t);
            // Simulate round-robin after deposit
            run_round_robin_scheduler();
            return;
        }
    }
    pthread_mutex_unlock(&account_mutex);
    printf("Account %d not found\n", account_id);
}
void withdraw(int account_id, double amount) {
    pthread_mutex_lock(&account_mutex);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == account_id) {
            pthread_mutex_lock(&accounts[i].mutex);
            if (accounts[i].balance >= amount) {
                accounts[i].balance -= amount;
                printf("Withdrew %.2f from account %d\n", amount, account_id);
            } else {
                printf("Insufficient balance in account %d\n", account_id);
            }
            pthread_mutex_unlock(&accounts[i].mutex);
            pthread_mutex_unlock(&account_mutex);
            // Simulate a transaction
            struct Transaction t = {transaction_count++, account_id, "withdraw", amount, rand() % 10 + 1};
            transaction_log[transaction_count - 1] = t;
            allocate_memory(&t);
            // Simulate round-robin after withdrawal
            run_round_robin_scheduler();
            return;
        }
    }
    pthread_mutex_unlock(&account_mutex);
    printf("Account %d not found\n", account_id);
}
// Memory Management
void allocate_memory(struct Transaction* transaction) {
    pthread_mutex_lock(&memory_mutex);
    for (int i = 0; i < memory_size; i++) {
        if (memory_map[i].transaction == NULL) {
            memory_map[i].page_id = i;
            memory_map[i].transaction = transaction;
            pthread_mutex_unlock(&memory_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&memory_mutex);
}
void free_memory(int transaction_id) {
    pthread_mutex_lock(&memory_mutex);
    for (int i = 0; i < memory_size; i++) {
        if (memory_map[i].transaction != NULL && memory_map[i].transaction->id == transaction_id) {
            memory_map[i].transaction = NULL;
            pthread_mutex_unlock(&memory_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&memory_mutex);
}
// Round Robin Scheduler
void run_round_robin_scheduler() {
    int time_elapsed = 0, completed_transactions = 0;
    printf("\n--- Round Robin Scheduling ---\n");
    while (completed_transactions < transaction_count) {
        for (int i = 0; i < transaction_count; i++) {
            if (transaction_log[i].duration > 0) {
                int execution_time = (transaction_log[i].duration > TIME_QUANTUM) ? TIME_QUANTUM : transaction_log[i].duration;
                printf("Transaction %d executes from %d to %d\n",
                       transaction_log[i].id, time_elapsed, time_elapsed + execution_time);
                time_elapsed += execution_time;
                transaction_log[i].duration -= execution_time;
                if (transaction_log[i].duration == 0) {
                    completed_transactions++;
                    free_memory(transaction_log[i].id);
                }
            }
        }
    }
    printf("Total time: %d units\n", time_elapsed);
}
// Visualizations
void visualize_memory_heap() {
    printf("\nMemory Heap:\n");
    for (int i = 0; i < memory_size; i++) {
        if (memory_map[i].transaction != NULL) {
            printf("Page %d -> Transaction %d\n", i, memory_map[i].transaction->id);
        } else {
            printf("Page %d -> Empty\n", i);
        }
    }
}
void visualize_gantt_chart() {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < transaction_count; i++) {
        printf("Transaction %d: Duration %d\n", transaction_log[i].id, transaction_log[i].duration);
    }
}
// Consolidated History
void show_history() {
    printf("\n--- Transaction History and Reports ---\n");
    visualize_memory_heap();
    visualize_gantt_chart();
}




