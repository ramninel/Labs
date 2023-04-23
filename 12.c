#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

/*Задание: в файле содержатся сведения о сотрудниках лаборатории: фамилия, год рождения, пол, 
образование (среднее, высшее), год поступления на работу. Найти самого старшего сотрудника среди мужчин.
Вывести список молодых специалистов (до 28 лет) с высшим образованием.
*/

//структура сотрудника
struct employee{ 
	char surname[20];
	int YOB;
	char sex;
	char ed[20];
	int YOE;
};

//Создание записи работника
struct employee NewRecord(void){ 
	struct employee record;
	fflush(stdin);
	puts("\nНе забудьте переключить раскладку на английскую!\n");
	printf("Фамилия (буквами латинского алфавита): ");
	while (scanf("%s", &record.surname) != 1 || strlen(record.surname) > 20){
		puts("\nТак нельзя. Нужно что-нибудь покороче.");
	}
	printf("Год рождения: ");
	while (scanf("%d", &record.YOB) != 1 || record.YOB > 2005 || record.YOB <= 1700){
		puts("\nМы таких молодых не берём. Давайте ещё раз.");
	}
	printf("Пол (M - male|F - female|O - other): ");
	fflush(stdin);
	while (scanf("%c", &record.sex) != 1 || record.sex != 'M' && record.sex != 'F' && record.sex != 'O'){
		puts("\nПрости, конечно, но мы пока таких не регистрируем. Давайте ещё раз.");
		fflush(stdin);
	}
	printf("Образование (Secondary|Higher): ");
	fflush(stdin);
	while (scanf("%s", &record.ed) != 1 || strlen(record.ed) > 20){
		puts("\nСлишком длинно и пафосно. Давайте ещё раз.");
		fflush(stdin);
	}
	printf("Год поступления на работу: ");
	while (scanf("%d", &record.YOE) != 1 || record.YOE <= 1700){
		puts("\nЧто-то вы заврались. Давайте ещё раз.");
		fflush(stdin);
	}
	return record;
}

//Когда всё
void done(void){
	fflush(stdin);
	puts("\nТут всё. Пойдёмте-ка в меню.\n");
	puts("Нажмите куда угодно на клавиатуре...");
	getchar();
	system("cls");
}

//Меню
int menu(int point){
	puts("\n1. Создать файл");
	puts("2. Просмотреть содержимое файла");
	puts("3. Добавить данные в файл");
	puts("4. Удалить данные из файла");
	puts("5. Корректировка данных в файле");
	puts("6. Найти нашего самого старшего сотрудника среди мужчин");
	puts("7. Список наших молодых специалистов с высшим образованием");
	puts("8. Выход\n");
	puts("Какой пункт выбираем?\n");
	scanf("%d", &point);
	return point;
}

//Создать файл
void createFile(char* fileName){
	fflush(stdin);
	system("cls");
	printf("\nКак назовём?\n\n");
	gets(fileName);
	while (strlen(fileName) > 24){
		puts("\nМнога букаф. Введите что-нибудь покороче.\n");
		gets(fileName);
	}
	FILE* f = fopen(fileName, "rb");
	if (f){
		puts("\nУ нас такое уже есть.");
		fclose(f);
		return;
	}
	else {
		fclose(f);
		f = fopen(fileName, "wb");
		if (f) puts("\nВсё сделано, файл создан!");
		else puts("Не получилось(");
		fclose(f);
	}
}

//Вывод записи
void printRecords(char* fileName){
	fflush(stdin);
	system("cls");
	puts("\nКакой файл смотреть будем?\n");
	FILE *f;
	gets(fileName);
	f = fopen(fileName, "rb");
	if (f){
		struct employee record;
		printf("----------------------------------------------------------------------------------------\n");
		printf("| Фамилия         | Год рождения | Пол   | Образование       |Год поступления на работу|\n");
		printf("----------------------------------------------------------------------------------------\n");	
		while (fread(&record, sizeof(record), 1, f)){
			printf("| %-15s | %-12d | %-5c | %-17s | %-23d |\n", record.surname, record.YOB, record.sex, record.ed, record.YOE);
		}
		printf("----------------------------------------------------------------------------------------\n\n");
	}	
	else puts("\nНет такого.\n");	
}

//Добавить что-то в файл
void addRecord(char* fileName){
	fflush(stdin);
	system("cls");
	puts("\nКакой файл смотреть будем? Если его нет, то не волнуйтесь, создадим новый!\n");
	FILE *f;
	gets(fileName);
	f = fopen(fileName, "ab");
	if (f){
		int a;
		do {
			struct employee record;
			record = NewRecord();
			fwrite(&record, sizeof(record), 1, f);
			fclose(f);
			puts("\nДанные добавлены.");
			puts("\nНе хотите ещё что-нибудь добавить?(1 - хочу/Не 1 - не хочу)\n");
			scanf("%d", &a);
			if (a != 1){
				puts("\nПонял, принял.");
				return;
			}
		} while (a != 2);
		
	}	
	else puts("\nНе получилось(\n");
}

//Удаление файла
void deleteRecord(char* fileName){
	fflush(stdin);
	system("cls");
	char sn[20];
	int i = 0;
	puts("\nВ каком файле наша жертва?\n");
	FILE *f, *fs;
	gets(fileName);
	f = fopen(fileName, "rb");
	if (f){
		struct employee record;
		printf("----------------------------------------------------------------------------------------\n");
		printf("| Фамилия         | Год рождения | Пол   | Образование       |Год поступления на работу|\n");
		printf("----------------------------------------------------------------------------------------\n");	
		while (fread(&record, sizeof(record), 1, f)){
			printf("| %-15s | %-12d | %-5c | %-17s | %-23d |\n", record.surname, record.YOB, record.sex, record.ed, record.YOE);
		}
		printf("----------------------------------------------------------------------------------------\n\n");
		fflush(stdin);
		puts("Кого увольняем?\n");
		gets(sn);

		rewind(f);
		fs = fopen("BrandoLab", "wb");
		while (fread(&record, sizeof(record), 1, f)){
			if (strcmp(sn, record.surname) != 0) fwrite(&record, sizeof(record), 1, fs);
			else {
				i = 1;
				break;
			}
		}
		while (fread(&record, sizeof(record), 1, f)){
			fwrite(&record, sizeof(record), 1, fs);
		}	
		if (i == 0) puts("\nЭто кто такой? У нас таких нет.");
		else puts("\nЗапись удалилась.");
		fclose(f);
		fclose(fs);
		remove(fileName);
		rename("BrandoLab", fileName);
	}
	else puts("\nНе получилось(");
//	perror("Status");
}

//Корректировка
void editRecord(char* fileName){
	fflush(stdin);
	system("cls");
	puts("\nКакой файл корректировать будем?\n");
	FILE *f;
	gets(fileName);
	f = fopen(fileName, "r+b");
	if (f){
		char sn[20], e[20], s;
		int year, emp, i = 0, c;
		struct employee record;
		printf("----------------------------------------------------------------------------------------\n");
		printf("| Фамилия         | Год рождения | Пол   | Образование       |Год поступления на работу|\n");
		printf("----------------------------------------------------------------------------------------\n");	
		while (fread(&record, sizeof(record), 1, f)){
			printf("| %-15s | %-12d | %-5c | %-17s | %-23d |\n", record.surname, record.YOB, record.sex, record.ed, record.YOE);
		}
		printf("----------------------------------------------------------------------------------------\n\n");
		fflush(stdin);
		puts("Введите фамилию сотрудника, данные о котором надо изменить.\n");
		scanf("%s", sn);
		rewind(f);
		while (fread(&record, sizeof(record), 1, f)){
			if (strcmp(sn, record.surname) == 0){
				i++;
				do {
					puts("\nЧто бы вы хотели изменить?\n");
					puts("1. Фамилию\n2. Год рождения\n3. Пол\n4. Образование\n5. Год поступления на работу\n6. Выпустите меня отсуда\n");
					scanf("%d", &c);
					switch (c){
						case 1:
							printf("\nВведите новую фамилию: ");
							while (scanf("%s", sn) != 1 || strlen(sn) > 20){
								puts("\nДлинновато. Давайте ещё раз.\n");
							}
							fseek(f, -sizeof(record), SEEK_CUR);
							fwrite(&sn, sizeof(record.surname), 1, f);
							fseek(f, 0, SEEK_CUR);
							puts("\nКорректировка учтена.");
							break;
						case 2:
							printf("\nВведите новый год рождения: ");
							while (scanf("%d", &year) != 1 || year > 2005 || year <= 1700){
								puts("\nБыть не может. Мы таких молодых не берём. Давайте ещё раз.\n");
							}	
							fseek(f, -sizeof(record) + sizeof(record.surname), SEEK_CUR);
							fwrite(&year, sizeof(record.YOB), 1, f);
							fseek(f, 0, SEEK_CUR);
							puts("\nКорректировка учтена.");
							break;
						case 3:
							fflush(stdin);
							printf("\nВведите новый пол: ");
							while (scanf("%c", &s) != 1 || s != 'M' && s != 'F' && s != 'O'){
								puts("\nПрости, конечно, но мы пока таких не регистрируем. Давайте ещё раз.\n");
								fflush(stdin);
							}
							fseek(f, -sizeof(record) + sizeof(record.surname) + sizeof(record.YOB), SEEK_CUR);
							fwrite(&s, sizeof(record.sex), 1, f);
							fseek(f, 0, SEEK_CUR);
							puts("\nКорректировка учтена.");
							break;
						case 4:
							fflush(stdin);
							printf("\nВведите новое образование: ");
							while (scanf("%s", e) != 1 || strlen(e) > 20){
								puts("\nСлишком длинно и пафосно. Давайте ещё раз.\n");
								fflush(stdin);
							}
							fseek(f, -sizeof(record) + sizeof(record.surname) + sizeof(record.YOB) + sizeof(record.sex), SEEK_CUR);
							fwrite(&e, sizeof(record.ed), 1, f);
							fseek(f, 0, SEEK_CUR);
							puts("\nКорректировка учтена.");
							break;
						case 5:
							fflush(stdin);
							printf("\nГод поступления на работу: ");
							while (scanf("%d", &emp) != 1 || emp <= 1700){
								puts("\nЧто-то вы заврались. Давайте ещё раз.");
								fflush(stdin);
							}
							fseek(f, -sizeof(record) + sizeof(record.surname) + sizeof(record.YOB) + sizeof(record.sex) + sizeof(record.ed), SEEK_CUR);
							fwrite(&emp, sizeof(record.YOE), 1, f);
							fseek(f, 0, SEEK_CUR);
							puts("\nКорректировка учтена.");
							break;
						case 6:
							puts("\nВыпускаем.");
							break;
						default:
							puts("\nМы не знаем, что это такое.");
							break;
					}
				}	while (c != 6);
				break;
			}
		}
		if (i == 0) puts("\nНет у нас такого.");
	}	
	else puts("\nНет такого.");
	fclose(f);
}

//Самый дед
void TheMan(char* fileName){
	fflush(stdin);
	system("cls");
	char sn[20];
	int year = 2021;
	FILE *f;	
	puts("\nС каким файлом работаем?\n");
	gets(fileName);
	f = fopen(fileName, "rb");
	if (f){
		struct employee record;
		while (fread(&record, sizeof(record), 1, f)){
			if (record.sex == 'M'){
				if (record.YOB < year){
					year = record.YOB;
					strcpy(sn, record.surname);
				}
			}
		}
		if (year != 2021){
			printf("\nСамый дед у нас %s.\n", sn);
		}
		else puts("\nТут мужчин нет.");	
	}
	else puts("\nНет такого.");
}

//Не деды
void youngEmployees(char* fileName){
	fflush(stdin);
	system("cls");
	puts("\nКакой файл смотреть будем?\n");
	FILE *f;
	gets(fileName);
	f = fopen(fileName, "rb");
	if (f){
		int i = 0;
		struct employee record;
		printf("----------------------------------------------------------------------------------------\n");
		printf("| Фамилия         | Год рождения | Пол   | Образование       |Год поступления на работу|\n");
		printf("----------------------------------------------------------------------------------------\n");	
		while (fread(&record, sizeof(record), 1, f)){
			if (2021 - record.YOB < 28 && strcmp("Higher", record.ed) == 0){
				printf("| %-15s | %-12d | %-5c | %-17s | %-23d |\n", record.surname, record.YOB, record.sex, record.ed, record.YOE);
				i = 1;
			}
		}
		printf("----------------------------------------------------------------------------------------\n");
		if (i == 0) puts("\nУ нас только пенсионеры и школьники.");
	}	
	else puts("\nНет такого.");
}

//Главное
int main() {
	setlocale(LC_ALL, "Rus");
	
	char fileName[25];
	
	puts("\nЗдравствуйте, товарищ. Что будем делать?");
	int point;
	do{
		point = menu(point);
		switch (point){
			case 1: //Создать файл
				createFile(fileName);
				done();
				break;
			case 2: //Просмотреть файл
				printRecords(fileName);
				done();
				break;
			case 3: //Добавить что-то в файл
				addRecord(fileName);
				done();
				break;
			case 4: //Удоли
				deleteRecord(fileName);
				done();
				break;
			case 5: //Корректировка
				editRecord(fileName);
				done();
				break;
			case 6: //Сотрудники-мужчины
				TheMan(fileName);
				done();
				break;
			case 7: //Специалисты
				youngEmployees(fileName);
				done();
				break;				
			case 8: //Прощание
				system("cls");
				printf("\nВсего хорошего. Берегите себя и своих близких.\n");
				return 0;
			default: //Если пользователь дурачок
				system("cls");
				printf("\nЭтого нет в нашем меню. Выберите что-нибудь другое.\n");
				break;
		}
		fflush(stdin);
	} while (point != 8);
	
	return 0;
}

