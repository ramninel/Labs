#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include <malloc.h>

/*���� ������: �������, ��� ��������, ���, ����������� (�������, ������), ��� ����������� �� ������.
����� ������ �������� ���������� ����� ������. ������� ������ ������� ������������ (�� 28 ���) � ������ ������������.

����������:
    - ������ �������� � �������� ����� �������, � ��� ��������� ����������� � ���������� �������� ������
    (���� ���� �� ����������, �� ��������� ������ ������)
    - ��� ������ �� ��������� ������������ ������ ����������� � ��� �� �����
    - ���� ��������� ������������� ��� ������� ��������� �� ������, ��� ����� �������� � ����������
    - ���������� ������ ������ �������� ������ � ���������� (����������� ���� ��������� ��������������)
    - ������������ ��������:
        - ���������� �������� � ������������� ������ � ����������� ��������������� (�������� ���� ������� ��������������)
        - �������� ������ � ������ � �������� �����������
        - �������� ������������� �������� ������
        - ����� � ������������ � �������������� ���������
    - ����� ������ ������������ ����������� � ��������� ���� � ���������� ��������� ����������� ���������
    - ������������� �������� � ������ � �������� �����������
*/

//���������� � ���������
typedef struct employee{
    char surname[20]; //�������
	int birth; //��� ��������
	char sex; //���
	char ed[20]; //�����������
	int employment; //��� ����������� �� ������
} DataType;

//���� ����������� ������
typedef struct node{
    DataType data; //���������� � ����������
    struct node *next, *prev; //��������� �� ��������� � ���������� ������
} node;

//���������� � ���������� ������
typedef struct list{
    node *begin, *end; //��������� �� ������ � ����� ������
} List;

List *readFile(char * fileName); //������ ���������� �����
bool saveFile(char * fileName, List * list); //���������� �����
bool emptyNode(List * list); //�������� �� �������
List *deleteNode(List * list, char * name); //�������� ���� ������
DataType inputEmployee(DataType record); //���� ���������� � ����������
List *addNode(List * list, DataType record); //���������� ���� � ������
void show(List * list); //����� ������� � ���������� �������
void showBackwards(List * list); //����� ������� � �������� �������
void theOldestOne(List * list); //����� ������ �������� ���������� ����� ������
void youngEmployees(List * list); //����� ������� ������� ����������� � ������ ������������
void printRecord(node* cur); //����� ����� ������
void printTable(); //����� ����� �������

int main(int argc, char *argv[]){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int point;
    char fileName[30];
    List *employees = (List*) malloc(sizeof(List));
    if (argc > 1){ //���� ��� ����� ��������� ����� ���������
        employees = readFile(argv[1]);
    } else { //���� ����� ����� �������
        printf("������� �������� �����\n");
        gets(fileName);
        while (strlen(fileName) > 29){
            puts("\n������� ������� ��������\n������� �������� ����� ������\n");
            fflush(stdin);
            gets(fileName);
        }
        employees = readFile(fileName); //��������� ���������� �� ����� � ��������� � ������
    }

	do{
        puts("\n1. �������� ������");
        puts("2. ������� ������");
        puts("3. ������� ������ �������");
        puts("4. ������� ������ ������� � �������� �������");
        puts("5. ����� ������ �������� ���������� ����� ������");
        puts("6. ������� ������ ������� ������������ (�� 28 ���) � ������ ������������");
        puts("7. ��������� � �����\n");
        puts("�������� �����\n");
        scanf("%d", &point); //���� ������ ����

        system("cls");
        switch(point){
            case 1:{ //���������� ������
                DataType record; //������ ���������� ��� ������ ���������� � ����������
                record = inputEmployee(record); //������ ���������� � ����������
                employees = addNode(employees, record); //��������� � ������
                break;
            }
            case 2:{ //�������� ������
                char name[20];
                if (emptyNode(employees)) //���� ������ ����
                    puts("������ ����");
                else {
                    show(employees); //������� ��� ������ ������
                    puts("������� ������� ����������");
                    scanf("%s", &name);
                    employees = deleteNode(employees, name); //������� ������ �� ������� ����������
                }
                break;
            }
            case 3:{ //����� �������
                if (emptyNode(employees)){
                    puts("������ ����");
                } else {
                    show(employees);
                }
                break;
            }
            case 4:{ //����� ������� � �������� �������
                if (emptyNode(employees)){
                    puts("������ ����");
                } else {
                    showBackwards(employees);
                }
                break;
            }
            case 5:{ //����� ������ �������� ���������� ����� ������
                if (emptyNode(employees)){
                    puts("������ ����");
                } else {
                    theOldestOne(employees);
                }
                break;
            }
            case 6:{ //����� ������� ������� ����������� � ������ ������������
                if (emptyNode(employees)){
                    puts("������ ����");
                } else {
                    youngEmployees(employees);
                }
                break;
            }
            case 7:{ //���������� ����� � ����� �� ���������
                if (saveFile(fileName, employees)){ //�������� �� ���������� �����
                    puts("���� �������");
                }
                else{
                    puts("���� �� �������");
                }
				return 0;
                break;
            }
            default:{ //���� ��� ����� ������������ ����� ����
                puts("������ ������ ���");
                break;
            }
        }
	} while (point != 7);
    return 0;
}

List *readFile(char * fileName){ //���������� �����
    FILE *f;
    DataType record;
    List *list = (List*) malloc(sizeof(List));
//    node *cur = (node*) malloc(sizeof(node));
    list->begin = NULL;
    list->end = NULL;
    if ((f = fopen(fileName, "rb")) == NULL){
        puts("������ �������� �����");
        return NULL; //���� ���� �� ��������, ���������� ������ ������
    }
    if (fread(&record, sizeof(record), 1, f)){ //���� ���� �������� � ��� ���� ������
        node *tmp = (node*) malloc(sizeof(node));
        tmp->data = record;
        list->end = tmp;
        list->begin = tmp;
        tmp -> next = NULL;
        tmp-> prev = NULL;
    } else return NULL; //����� ���������� ������ ������
//    cur = list->begin;
    while (fread(&record, sizeof(record), 1, f)){ //���������� ���� ������
        node *tmp = (node*) malloc(sizeof(node));
        tmp->data = record;
        tmp->next = NULL;
        tmp->prev = list->end;
        list->end->next = tmp;
        list->end = tmp;
    }
    fclose(f); //��������� ����
    return list;
}

bool saveFile(char * fileName, List * list){
    FILE *f;
    if ((f = fopen(fileName, "wb")) == NULL){ //���� ���� �� ������� �������
        puts("������ �������� �����");
        getchar();
        return false;
    }
    if (!list) {
        puts("C������ ������ ������");
        return true;
    }
    while (list->begin){ //���������� ���� ������ � ���������� � ����
        if (fwrite(&list->begin->data, sizeof(DataType), 1, f)){
           list->begin = list->begin->next;
        } else {
            return false;
        }
    }
    return true;
}

bool emptyNode(List * list){ //�������� �� �������
    if (list == NULL) //���� ������ ����
        return true;
    else
        return false;
}

List *deleteNode(List * list, char * name){ //�������� ����
    node * cur = (node*) malloc(sizeof(node));
    cur = list->begin;
    int choice;
    if (!strcmp(name, cur->data.surname)){ //���� ������� �������
        printTable(); //����� ��������� � ����������
        printRecord(cur);
        puts("�� ������ ������� ��� ������? (1 - ��/ 0 - ���)");
        while (scanf("%d", &choice) != 1 || choice != 1 || choice != 2){
            if (choice == 1){ //���� ������������ ����� ������� ������ ��� ������
                if (!list->begin->next){ //���� ��� ������������ ���� ������
                    free(cur);
                    list -> begin = NULL;
                    list -> end = NULL;
                    list = NULL;
                    puts("��������� �����");
                    return list;
                } else {
                    list->begin = cur->next;
                    if (list->begin){
                        list->begin->prev = NULL;
                    }
                    free(cur);
                    puts("������ �������");
                    return list;
                }

            } else if (choice == 0){ //���� ��� �� �� ������, ������� ����� �������
                puts("������ �� �������");
                break;
            } else {
                puts("������������ �����");
            }
        }
        cur = cur->next;
    }
    while (cur->next){ //���������� ������ �� ���������� ����
        if (!strcmp(name, cur->data.surname)){
            puts("�� ������ ������� ��� ������? (1 - ��/ 0 - ���)");
            while (scanf("%d", &choice) ||choice != 1 || choice != 0){
                printTable();
                printRecord(cur);
                if (choice == 1){
                    cur->next->prev = cur->prev;
                    cur->prev->next = cur->next;
                    free(cur);
                    puts("������ �������");
                    return list;
                } else if (choice == 0){
                    puts("������ �� �������");
                    break;
                } else {
                    puts("������������ �����");
                }
            }
        }
        cur = cur->next;
    }
    if (!strcmp(name, cur->data.surname)){ //���� ������ ������ ���������
        puts("�� ������ ������� ��� ������? (1 - ��/ 0 - ���)");
        while (scanf("%d", &choice) ||choice != 1 || choice != 0){
            printTable();
            printRecord(cur);
            if (choice == 1){
                cur->prev->next = NULL;
                list->end = cur->prev;
                free(cur);
                puts("������ �������");
                    return list;
            } else if (choice == 0){
                puts("������ �� �������");
                break;
            } else {
                puts("������������ �����");
            }
        }
    }
    puts("��� ��� ��������� ��������� � ����� ��������");
    return list;
}

DataType inputEmployee(DataType record){ //���� ���������� � ����������
	printf("������� ����������: ");
	while (scanf("%s", &record.surname) != 1 || strlen(record.surname) > 20){
		puts("\n����� ������� �� ��������\n������� ���-�� ������");
	}
	printf("��� ��������: ");
	while (scanf("%d", &record.birth) != 1 || record.birth > 2005 || record.birth <= 1900){
		puts("\n����� ��� ������� �� ��������\n������� ���-�� ������");
	}
	printf("��� (� - �������|� - �������): ");
	fflush(stdin);
	while (scanf("%c", &record.sex) != 1 || record.sex != '�' && record.sex != '�'){
		puts("\n����� ��� �� ��������\n������� ���-�� ������");
		fflush(stdin);
	}
	printf("�����������: ");
	fflush(stdin);
	while (scanf("%s", &record.ed) != 1 || strlen(record.ed) > 20){
		puts("\n����� ����������� �� ��������\n������� ���-�� ������");
		fflush(stdin);
	}
	printf("��� ����������� �� ������: ");
	while (scanf("%d", &record.employment) != 1 || record.employment <= 1945){
		puts("\n����� ��� �� ��������\n������� ���-�� ������");
		fflush(stdin);
	}
	return record;
}

List *addNode(List * list, DataType record){ //���������� ������ ����
    node *tmp = (node*) malloc(sizeof(node));
    if (list){ //���� ������ ����������
        node *cur = (node*) malloc(sizeof(node));
        cur = list->begin;
        tmp->data = record;
        //���� ����� ������ ������, �� ��������� ����� ���� � ������
        if (strcmp(tmp->data.surname, cur->data.surname) < 0){
            tmp->next = list->begin;
            tmp->prev = NULL;
            if (list->begin){
                list->begin->prev = tmp;
            }
            else {
                list->end = tmp;
            }
            list->begin = tmp;
            return list;
        }
        //���������� ������ ����������� ������ � �����
        //���� ����� ������, �� ��������� ����� ���
        while (cur->next){
            if (strcmp(tmp->data.surname, cur->data.surname) > 0 && strcmp(tmp->data.surname, cur->next->data.surname) < 0){
                if (!cur->next->next) {
                    break;
                }
                tmp->next = cur->next;
                tmp->prev = cur;
                cur->next->prev = tmp;
                cur->next = tmp;

                return list;
            }
            cur = cur->next;
        }
        //���� ����� ������ ���������, �� ��������� � ����� ������
        if (strcmp(tmp->data.surname, cur->data.surname) > 0){
            tmp->next = NULL;
            tmp->prev = list->end;
            if (list->end){
                list->end->next = tmp;
            } else {
                list->begin = tmp;
            }
            list->end = tmp;
            return list;
        }
    } else { //���� ������ ���, �� ������ ����� � ��������� ����� ������ ���� ������
        List * list = (List*) malloc(sizeof(List));
        tmp->data = record;
        tmp->next = NULL;
        tmp->prev = NULL;
        list->begin = tmp;
        list->end = tmp;
        return list;
    }
    return list;
}

void show(List * list){ //����� ���� �������
    node * cur = list->begin;
    printTable();
    while (cur){ //���� ������� ������ �� NULL
        printRecord(cur);
        cur = cur->next; //��������� � ���������
    }
    return;
}

void showBackwards(List * list){ //����� ���� ������� � �������� �������
    if (!list) {
        return;
    }
    node * cur = list->end;
    printTable();
    while (cur){ //���� ������� ������ �� NULL
        printRecord(cur);
        cur = cur->prev; //��������� � ����������
    }
}

void theOldestOne(List * list){ //����� ������ �������� ���������� ����� ������
    node * cur = list->begin;
    int year = 2021; //����������, � ������� ����� ���������� ��� ��������
    char name[20];
    while (cur){
        if (cur->data.sex == '�'){ //���� ��������� �������
            if (cur->data.birth < year){ //���� ���� ��������� ������ �����������
                year = cur->data.birth;
                strcpy(name, cur->data.surname);
            }
        }
        cur = cur->next;
    }
    if (year != 2021){
        printf("%s ����� ������� ��������� ����� ������\n", name);
    } else {
        puts("\n����� ����������� ��� ������");
    }
}

void youngEmployees(List * list){ //����� ������� ������� ����������� � ������ ������������
    node * cur = list->begin;
    int i = 0; //�������
    printTable();
    while (cur){
        if (2021 - cur->data.birth < 28 && strcmp("������", cur->data.ed) == 0){ //���� ��������� ������ 28 � ����� ����� �����������
            printRecord(cur);
            i++;
        }
        cur = cur->next;
    }
    if (i == 0){ //���� ������� �� ��� 0
        puts("����� ����������� ���");
    }
}

void printRecord(node* cur){ //����� ������ � ��������� ����
    printf("| %-15s | %-12d | %-5c | %-17s | %-23d |\n", cur->data.surname, cur->data.birth, cur->data.sex, cur->data.ed, cur->data.employment);
    printf("----------------------------------------------------------------------------------------\n");
}

void printTable(){ //����� ����� �������
    printf("----------------------------------------------------------------------------------------\n");
    printf("| �������         | ��� �������� | ���   | �����������       |��� ����������� �� ������|\n");
    printf("----------------------------------------------------------------------------------------\n");
}

