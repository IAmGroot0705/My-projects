#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define DATA_FILE "students.txt"

typedef struct {
    int id;
    char name[NAME_LEN];
    int marks;
} Student;

static void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len == 0) {
        return;
    }
    if (str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

static void load_students(Student students[], int *count) {
    FILE *file = fopen(DATA_FILE, "r");
    char line[128];

    *count = 0;
    if (!file) {
        return;
    }

    while (fgets(line, sizeof(line), file) && *count < MAX_STUDENTS) {
        char *token;
        char *rest = line;
        Student s;

        token = strtok(rest, "|");
        if (!token) {
            continue;
        }
        s.id = atoi(token);

        token = strtok(NULL, "|");
        if (!token) {
            continue;
        }
        strncpy(s.name, token, NAME_LEN - 1);
        s.name[NAME_LEN - 1] = '\0';

        token = strtok(NULL, "|");
        if (!token) {
            continue;
        }
        s.marks = atoi(token);

        trim_newline(s.name);
        students[*count] = s;
        (*count)++;
    }

    fclose(file);
}

static void save_students(const Student students[], int count) {
    FILE *file = fopen(DATA_FILE, "w");
    int i;

    if (!file) {
        printf("Unable to open %s for saving.\n", DATA_FILE);
        return;
    }

    for (i = 0; i < count; i++) {
        fprintf(file, "%d|%s|%d\n", students[i].id, students[i].name, students[i].marks);
    }

    fclose(file);
}

static int find_student_index_by_id(const Student students[], int count, int id) {
    int i;
    for (i = 0; i < count; i++) {
        if (students[i].id == id) {
            return i;
        }
    }
    return -1;
}

static void add_student(Student students[], int *count) {
    Student s;
    char buffer[NAME_LEN];

    if (*count >= MAX_STUDENTS) {
        printf("Student list is full.\n");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &s.id);
    getchar();

    if (find_student_index_by_id(students, *count, s.id) != -1) {
        printf("Student with ID %d already exists.\n", s.id);
        return;
    }

    printf("Enter Student Name: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return;
    }
    trim_newline(buffer);
    strncpy(s.name, buffer, NAME_LEN - 1);
    s.name[NAME_LEN - 1] = '\0';

    printf("Enter Marks: ");
    scanf("%d", &s.marks);
    getchar();

    students[*count] = s;
    (*count)++;
    printf("Student added successfully.\n");
}

static void delete_student(Student students[], int *count) {
    int id;
    int index;
    int i;

    printf("Enter Student ID to delete: ");
    scanf("%d", &id);
    getchar();

    index = find_student_index_by_id(students, *count, id);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    for (i = index; i < *count - 1; i++) {
        students[i] = students[i + 1];
    }
    (*count)--;
    printf("Student deleted successfully.\n");
}

static void search_student(const Student students[], int count) {
    int id;
    int index;

    printf("Enter Student ID to search: ");
    scanf("%d", &id);
    getchar();

    index = find_student_index_by_id(students, count, id);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("Student found: ID: %d, Name: %s, Marks: %d\n",
           students[index].id, students[index].name, students[index].marks);
}

static void update_marks(Student students[], int count) {
    int id;
    int index;
    int marks;

    printf("Enter Student ID to update marks: ");
    scanf("%d", &id);
    getchar();

    index = find_student_index_by_id(students, count, id);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("Enter new marks: ");
    scanf("%d", &marks);
    getchar();

    students[index].marks = marks;
    printf("Marks updated successfully.\n");
}

static void display_students(const Student students[], int count) {
    int i;
    if (count == 0) {
        printf("No student records found.\n");
        return;
    }

    printf("\n%-10s %-30s %-10s\n", "ID", "Name", "Marks");
    printf("------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%-10d %-30s %-10d\n", students[i].id, students[i].name, students[i].marks);
    }
}

static void sort_by_id(Student students[], int count) {
    int i;
    int j;
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (students[j].id > students[j + 1].id) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    printf("Students sorted by ID.\n");
}

static void sort_by_name(Student students[], int count) {
    int i;
    int j;
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (strcmp(students[j].name, students[j + 1].name) > 0) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    printf("Students sorted by Name.\n");
}

static void print_menu(void) {
    printf("\n==== Student Record Management System ====\n");
    printf("1. Add Student\n");
    printf("2. Delete Student\n");
    printf("3. Search Student\n");
    printf("4. Update Marks\n");
    printf("5. Display All Records\n");
    printf("6. Sort by ID\n");
    printf("7. Sort by Name\n");
    printf("8. Save and Exit\n");
    printf("Enter your choice: ");
}

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    int choice;

    load_students(students, &count);

    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }
        getchar();

        switch (choice) {
            case 1:
                add_student(students, &count);
                break;
            case 2:
                delete_student(students, &count);
                break;
            case 3:
                search_student(students, count);
                break;
            case 4:
                update_marks(students, count);
                break;
            case 5:
                display_students(students, count);
                break;
            case 6:
                sort_by_id(students, count);
                break;
            case 7:
                sort_by_name(students, count);
                break;
            case 8:
                save_students(students, count);
                printf("Data saved. Exiting.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    save_students(students, count);
    return 0;
}
