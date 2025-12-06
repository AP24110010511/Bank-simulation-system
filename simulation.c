#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define CREDENTIAL_FILE "credentials.txt"
#define SERVED_FILE "served_customers.txt"

struct Customer {
    int token;
    char name[50];
    char service[50];
};

struct Queue {
    struct Customer arr[MAX];
    int front;
    int rear;
    int nextToken;
};

void initQueue(struct Queue *q) {
    q->front = -1;
    q->rear = -1;
    q->nextToken = 1;
}

int isFull(struct Queue *q) {
    return (q->front == 0 && q->rear == MAX - 1) || (q->front == q->rear + 1);
}

int isEmpty(struct Queue *q) {
    return q->front == -1;
}

void clearBuffer() {
    while (getchar() != '\n');
}

void enqueueCustomer(struct Queue *q) {
    if (isFull(q)) {
        printf("\nQueue is FULL!\n");
        return;
    }

    struct Customer c;
    c.token = q->nextToken++;

    clearBuffer();
    printf("\nEnter Customer Name: ");
    fgets(c.name, sizeof(c.name), stdin);
    c.name[strcspn(c.name, "\n")] = 0;

    printf("Enter Service Type: ");
    fgets(c.service, sizeof(c.service), stdin);
    c.service[strcspn(c.service, "\n")] = 0;

    if (q->front == -1) q->front = 0;

    q->rear = (q->rear + 1) % MAX;
    q->arr[q->rear] = c;

    printf("\nCustomer Added.\nToken Number: %d\n", c.token);
}

void serveCustomer(struct Queue *q) {
    if (isEmpty(q)) {
        printf("\nNo customers in queue.\n");
        return;
    }

    struct Customer c = q->arr[q->front];

    if (q->front == q->rear) {
        q->front = q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX;
    }

    printf("\nServing Customer:\n");
    printf("Token: %d | Name: %s | Service: %s\n", c.token, c.name, c.service);

    FILE *f = fopen(SERVED_FILE, "a");
    if (f) {
        fprintf(f, "Token: %d | Name: %s | Service: %s\n", c.token, c.name, c.service);
        fclose(f);
    }
}

void displayQueue(struct Queue *q) {
    if (isEmpty(q)) {
        printf("\nNo customers in queue.\n");
        return;
    }

    printf("\n----- CURRENT QUEUE -----\n");
    int i = q->front;

    while (1) {
        printf("Token: %d | Name: %s | Service: %s\n",
               q->arr[i].token, q->arr[i].name, q->arr[i].service);

        if (i == q->rear) break;
        i = (i + 1) % MAX;
    }
}

void peekCustomer(struct Queue *q) {
    if (isEmpty(q)) {
        printf("\nNo customers in queue.\n");
        return;
    }

    struct Customer c = q->arr[q->front];
    printf("\nNext Customer:\nToken: %d | Name: %s | Service: %s\n",
           c.token, c.name, c.service);
}

int loginSystem() {
    FILE *file = fopen(CREDENTIAL_FILE, "r");

    if (!file) {
        file = fopen(CREDENTIAL_FILE, "w");
        fprintf(file, "admin admin123\n");
        fclose(file);
        printf("\nLogin file created.\nDefault: admin / admin123\n\n");
        file = fopen(CREDENTIAL_FILE, "r");
    }

    char savedUser[50], savedPass[50];
    fscanf(file, "%s %s", savedUser, savedPass);
    fclose(file);

    char user[50], pass[50];
    printf("\n======== LOGIN ========\n");
    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(user, savedUser) == 0 && strcmp(pass, savedPass) == 0) {
        printf("\nLogin Successful!\n");
        return 1;
    }

    printf("\nInvalid Login!\n");
    return 0;
}

int main() {
    if (!loginSystem()) {
        printf("\nExiting...\n");
        return 0;
    }

    struct Queue q;
    initQueue(&q);

    int choice;

    do {
        printf("\n===== BANK QUEUE SIMULATION (C) =====\n");
        printf("1. Add Customer (Enqueue)\n");
        printf("2. Serve Customer (Dequeue)\n");
        printf("3. Display Queue\n");
        printf("4. Peek Next Customer\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
            case 1: enqueueCustomer(&q); break;
            case 2: serveCustomer(&q); break;
            case 3: displayQueue(&q); break;
            case 4: peekCustomer(&q); break;
            case 5: printf("\nExiting...\n"); break;
            default: printf("\nInvalid Choice!\n");
        }

    } while (choice != 5);

    return 0;
}
