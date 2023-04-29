#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include <malloc.h>

/*Поля данных: фамилия, год рождения, пол, образование (среднее, высшее), год поступления на работу.
Найти самого старшего сотрудника среди мужчин. Вывести список молодых специалистов (до 28 лет) с высшим образованием.

Повышенный:
    - данные хранятся в бинарном файле записей, а для обработки считываются в двусвязный линейный список
    (если файл не существует, то создается пустой список)
    - при выходе из программы обработанные данные сохраняются в том же файле
    - если параметры пользователем при запуске программы не заданы, имя файла вводится с клавиатуры
    - элементами данных должны являться записи с вариантами (вариативные поля придумать самостоятельно)
    - обязательные операции:
        - добавление элемента в упорядоченный список с сохранением упорядоченности (ключевое поле выбрать самостоятельно)
        - просмотр списка в прямом и обратном направлении
        - удаление произвольного элемента списка
        - поиск в соответствии с индивидуальным вариантом
    - вывод данных осуществлять постранично в табличном виде с графлением визуально подходящими символами
    - предусмотреть листание в прямом и обратном направлении
*/

//Информация о работнике
typedef struct employee{
    char surname[20]; //Фамилия
	int birth; //Год рождения
	char sex; //Пол
	char ed[20]; //Образование
	int employment; //Год поступления на работу
} DataType;

//Узел двусвязного списка
typedef struct node{
    DataType data; //Информация о сотруднике
    struct node *next, *prev; //Указатели на следующую и предыдущую запись
} node;

//Информация о двусвязном списке
typedef struct list{
    node *begin, *end; //Указатели на начало и конец списка
} List;

List *readFile(char * fileName); //Чтение информации файла
bool saveFile(char * fileName, List * list); //Сохранение файла
bool emptyNode(List * list); //Проверка на пустоту
List *deleteNode(List * list, char * name); //Удаление узла списка
DataType inputEmployee(DataType record); //Ввод информации о сотруднике
List *addNode(List * list, DataType record); //Добавление узла в список
void show(List * list); //Вывод записей в алфавитном порядке
void showBackwards(List * list); //Вывод записей в обратном порядке
void theOldestOne(List * list); //Поиск самого старшего сотрудника среди мужчин
void youngEmployees(List * list); //Вывод записей молодых сотрудников с высшим образованием
void printRecord(node* cur); //Вывод одной записи
void printTable(); //Вывод шапки таблицы

int main(int argc, char *argv[]){
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int point;
    char fileName[30];
    List *employees = (List*) malloc(sizeof(List));
    if (argc > 1){ //Если имя файла переданно через параметры
        employees = readFile(argv[1]);
    } else { //Ввод имени файла вручную
        printf("Введите название файла\n");
        gets(fileName);
        while (strlen(fileName) > 29){
            puts("\nСлишком длинное название\nВведите название файла заново\n");
            fflush(stdin);
            gets(fileName);
        }
        employees = readFile(fileName); //Считываем информацию из файла и сохраняем в список
    }

	do{
        puts("\n1. Добавить запись");
        puts("2. Удалить запись");
        puts("3. Вывести список записей");
        puts("4. Вывести список записей в обратном порядке");
        puts("5. Найти самого старшего сотрудника среди мужчин");
        puts("6. Вывести список молодых специалистов (до 28 лет) с высшим образованием");
        puts("7. Сохранить и выйти\n");
        puts("Выберите пункт\n");
        scanf("%d", &point); //Ввод пункта меню

        system("cls");
        switch(point){
            case 1:{ //Добавление записи
                DataType record; //Создаём переменную для записи информации о сотруднике
                record = inputEmployee(record); //Вводим информацию о сотруднике
                employees = addNode(employees, record); //Добавляем в список
                break;
            }
            case 2:{ //Удаление записи
                char name[20];
                if (emptyNode(employees)) //Если список пуст
                    puts("Список пуст");
                else {
                    show(employees); //Выводим все записи списка
                    puts("Введите фамилию сотрудника");
                    scanf("%s", &name);
                    employees = deleteNode(employees, name); //Удаляем запись по фамилии сотрудника
                }
                break;
            }
            case 3:{ //Вывод записей
                if (emptyNode(employees)){
                    puts("Список пуст");
                } else {
                    show(employees);
                }
                break;
            }
            case 4:{ //Вывод записей в обратном порядке
                if (emptyNode(employees)){
                    puts("Список пуст");
                } else {
                    showBackwards(employees);
                }
                break;
            }
            case 5:{ //Поиск самого старшего сотрудника среди мужчин
                if (emptyNode(employees)){
                    puts("Список пуст");
                } else {
                    theOldestOne(employees);
                }
                break;
            }
            case 6:{ //Вывод записей молодых сотрудников с высшим образованием
                if (emptyNode(employees)){
                    puts("Список пуст");
                } else {
                    youngEmployees(employees);
                }
                break;
            }
            case 7:{ //Сохранение файла и выход из программы
                if (saveFile(fileName, employees)){ //Проверка на сохранение файла
                    puts("Файл сохранён");
                }
                else{
                    puts("Файл не сохранён");
                }
				return 0;
                break;
            }
            default:{ //Если был введён неподходящий пункт меню
                puts("Такого пункта нет");
                break;
            }
        }
	} while (point != 7);
    return 0;
}

List *readFile(char * fileName){ //Считывание файла
    FILE *f;
    DataType record;
    List *list = (List*) malloc(sizeof(List));
//    node *cur = (node*) malloc(sizeof(node));
    list->begin = NULL;
    list->end = NULL;
    if ((f = fopen(fileName, "rb")) == NULL){
        puts("Ошибка открытия файла");
        return NULL; //Если файл не открылся, возвращаем пустой список
    }
    if (fread(&record, sizeof(record), 1, f)){ //Если файл открылся и там есть записи
        node *tmp = (node*) malloc(sizeof(node));
        tmp->data = record;
        list->end = tmp;
        list->begin = tmp;
        tmp -> next = NULL;
        tmp-> prev = NULL;
    } else return NULL; //Иначе возвращаем пустой список
//    cur = list->begin;
    while (fread(&record, sizeof(record), 1, f)){ //Перебираем узлы списка
        node *tmp = (node*) malloc(sizeof(node));
        tmp->data = record;
        tmp->next = NULL;
        tmp->prev = list->end;
        list->end->next = tmp;
        list->end = tmp;
    }
    fclose(f); //Закрываем файл
    return list;
}

bool saveFile(char * fileName, List * list){
    FILE *f;
    if ((f = fopen(fileName, "wb")) == NULL){ //Если файл не удалось создать
        puts("Ошибка создания файла");
        getchar();
        return false;
    }
    if (!list) {
        puts("Cохранён пустой список");
        return true;
    }
    while (list->begin){ //Перебираем весь список и записываем в файл
        if (fwrite(&list->begin->data, sizeof(DataType), 1, f)){
           list->begin = list->begin->next;
        } else {
            return false;
        }
    }
    return true;
}

bool emptyNode(List * list){ //Проверка на пустоту
    if (list == NULL) //Если список пуст
        return true;
    else
        return false;
}

List *deleteNode(List * list, char * name){ //Удаление узла
    node * cur = (node*) malloc(sizeof(node));
    cur = list->begin;
    int choice;
    if (!strcmp(name, cur->data.surname)){ //Если фамилия совпала
        printTable(); //Вывод инфомации о сотруднике
        printRecord(cur);
        puts("Вы хотите удалить эту запись? (1 - да/ 0 - нет)");
        while (scanf("%d", &choice) != 1 || choice != 1 || choice != 2){
            if (choice == 1){ //Если пользователь хочет удалить именно эту запись
                if (!list->begin->next){ //Если это единственный узел списка
                    free(cur);
                    list -> begin = NULL;
                    list -> end = NULL;
                    list = NULL;
                    puts("Последний удалён");
                    return list;
                } else {
                    list->begin = cur->next;
                    if (list->begin){
                        list->begin->prev = NULL;
                    }
                    free(cur);
                    puts("Запись удалена");
                    return list;
                }

            } else if (choice == 0){ //Если это не та запись, которую нужно удалить
                puts("Запись не удалена");
                break;
            } else {
                puts("Некорректный ответ");
            }
        }
        cur = cur->next;
    }
    while (cur->next){ //Перебираем список до последнего узла
        if (!strcmp(name, cur->data.surname)){
            puts("Вы хотите удалить эту запись? (1 - да/ 0 - нет)");
            while (scanf("%d", &choice) ||choice != 1 || choice != 0){
                printTable();
                printRecord(cur);
                if (choice == 1){
                    cur->next->prev = cur->prev;
                    cur->prev->next = cur->next;
                    free(cur);
                    puts("Запись удалена");
                    return list;
                } else if (choice == 0){
                    puts("Запись не удалена");
                    break;
                } else {
                    puts("Некорректный ответ");
                }
            }
        }
        cur = cur->next;
    }
    if (!strcmp(name, cur->data.surname)){ //Если нужная запись последняя
        puts("Вы хотите удалить эту запись? (1 - да/ 0 - нет)");
        while (scanf("%d", &choice) ||choice != 1 || choice != 0){
            printTable();
            printRecord(cur);
            if (choice == 1){
                cur->prev->next = NULL;
                list->end = cur->prev;
                free(cur);
                puts("Запись удалена");
                    return list;
            } else if (choice == 0){
                puts("Запись не удалена");
                break;
            } else {
                puts("Некорректный ответ");
            }
        }
    }
    puts("Это был последний сотрудник с такой фамилией");
    return list;
}

DataType inputEmployee(DataType record){ //Ввод информации о сотруднике
	printf("Фамилия сотрудника: ");
	while (scanf("%s", &record.surname) != 1 || strlen(record.surname) > 20){
		puts("\nТакая фамилия не подходит\nВведите что-то другое");
	}
	printf("Год рождения: ");
	while (scanf("%d", &record.birth) != 1 || record.birth > 2005 || record.birth <= 1900){
		puts("\nТакой год рожения не подходит\nВведите что-то другое");
	}
	printf("Пол (М - мужской|Ж - женский): ");
	fflush(stdin);
	while (scanf("%c", &record.sex) != 1 || record.sex != 'М' && record.sex != 'Ж'){
		puts("\nТакой пол не подходит\nВведите что-то другое");
		fflush(stdin);
	}
	printf("Образование: ");
	fflush(stdin);
	while (scanf("%s", &record.ed) != 1 || strlen(record.ed) > 20){
		puts("\nТакое обрахование не подходит\nВведите что-то другое");
		fflush(stdin);
	}
	printf("Год поступления на работу: ");
	while (scanf("%d", &record.employment) != 1 || record.employment <= 1945){
		puts("\nТакой год не подходит\nВведите что-то другое");
		fflush(stdin);
	}
	return record;
}

List *addNode(List * list, DataType record){ //Добавление нового узла
    node *tmp = (node*) malloc(sizeof(node));
    if (list){ //Если список существует
        node *cur = (node*) malloc(sizeof(node));
        cur = list->begin;
        tmp->data = record;
        //Если новая строка больше, то вставляем новый узел в начало
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
        //Сравниваем каждую последующую строку с новой
        //Если новая больше, то вставляем перед ней
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
        //Если новая больше последней, то вставляем в конец списка
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
    } else { //Если списка нет, то создаём новый и вставляем новую строку туда первой
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

void show(List * list){ //Вывод всех записей
    node * cur = list->begin;
    printTable();
    while (cur){ //Пока текущая запись не NULL
        printRecord(cur);
        cur = cur->next; //Переходим к следующей
    }
    return;
}

void showBackwards(List * list){ //Вывод всех записей в обратном порядке
    if (!list) {
        return;
    }
    node * cur = list->end;
    printTable();
    while (cur){ //Пока текущая запись не NULL
        printRecord(cur);
        cur = cur->prev; //Переходим к предыдущей
    }
}

void theOldestOne(List * list){ //Поиск самого старшего сотрудника среди мужчин
    node * cur = list->begin;
    int year = 2021; //Переменная, с которой будем сравнивать год рождения
    char name[20];
    while (cur){
        if (cur->data.sex == 'М'){ //Если сотрудник мужчина
            if (cur->data.birth < year){ //Если этот сотрудник старше предыдущего
                year = cur->data.birth;
                strcpy(name, cur->data.surname);
            }
        }
        cur = cur->next;
    }
    if (year != 2021){
        printf("%s самый старший сотрудник среди мужчин\n", name);
    } else {
        puts("\nСреди сотрудников нет мужчин");
    }
}

void youngEmployees(List * list){ //Вывод записей молодых сотрудников с высшим образованием
    node * cur = list->begin;
    int i = 0; //Счётчик
    printTable();
    while (cur){
        if (2021 - cur->data.birth < 28 && strcmp("Высшее", cur->data.ed) == 0){ //Если сотрудник моложе 28 и имеет высше образование
            printRecord(cur);
            i++;
        }
        cur = cur->next;
    }
    if (i == 0){ //Если счётчик всё ещё 0
        puts("Таких сотрудников нет");
    }
}

void printRecord(node* cur){ //Вывод записи в табличном виде
    printf("| %-15s | %-12d | %-5c | %-17s | %-23d |\n", cur->data.surname, cur->data.birth, cur->data.sex, cur->data.ed, cur->data.employment);
    printf("----------------------------------------------------------------------------------------\n");
}

void printTable(){ //Вывод шапки таблицы
    printf("----------------------------------------------------------------------------------------\n");
    printf("| Фамилия         | Год рождения | Пол   | Образование       |Год поступления на работу|\n");
    printf("----------------------------------------------------------------------------------------\n");
}

