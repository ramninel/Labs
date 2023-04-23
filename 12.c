#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

/*�������: � ����� ���������� �������� � ����������� �����������: �������, ��� ��������, ���, 
����������� (�������, ������), ��� ����������� �� ������. ����� ������ �������� ���������� ����� ������.
������� ������ ������� ������������ (�� 28 ���) � ������ ������������.
*/

//��������� ����������
struct employee{ 
	char surname[20];
	int YOB;
	char sex;
	char ed[20];
	int YOE;
};

//�������� ������ ���������
struct employee NewRecord(void){ 
	struct employee record;
	fflush(stdin);
	puts("\n�� �������� ����������� ��������� �� ����������!\n");
	printf("������� (������� ���������� ��������): ");
	while (scanf("%s", &record.surname) != 1 || strlen(record.surname) > 20){
		puts("\n��� ������. ����� ���-������ ��������.");
	}
	printf("��� ��������: ");
	while (scanf("%d", &record.YOB) != 1 || record.YOB > 2005 || record.YOB <= 1700){
		puts("\n�� ����� ������� �� ����. ������� ��� ���.");
	}
	printf("��� (M - male|F - female|O - other): ");
	fflush(stdin);
	while (scanf("%c", &record.sex) != 1 || record.sex != 'M' && record.sex != 'F' && record.sex != 'O'){
		puts("\n������, �������, �� �� ���� ����� �� ������������. ������� ��� ���.");
		fflush(stdin);
	}
	printf("����������� (Secondary|Higher): ");
	fflush(stdin);
	while (scanf("%s", &record.ed) != 1 || strlen(record.ed) > 20){
		puts("\n������� ������ � �������. ������� ��� ���.");
		fflush(stdin);
	}
	printf("��� ����������� �� ������: ");
	while (scanf("%d", &record.YOE) != 1 || record.YOE <= 1700){
		puts("\n���-�� �� ���������. ������� ��� ���.");
		fflush(stdin);
	}
	return record;
}

//����� ��
void done(void){
	fflush(stdin);
	puts("\n��� ��. �������-�� � ����.\n");
	puts("������� ���� ������ �� ����������...");
	getchar();
	system("cls");
}

//����
int menu(int point){
	puts("\n1. ������� ����");
	puts("2. ����������� ���������� �����");
	puts("3. �������� ������ � ����");
	puts("4. ������� ������ �� �����");
	puts("5. ������������� ������ � �����");
	puts("6. ����� ������ ������ �������� ���������� ����� ������");
	puts("7. ������ ����� ������� ������������ � ������ ������������");
	puts("8. �����\n");
	puts("����� ����� ��������?\n");
	scanf("%d", &point);
	return point;
}

//������� ����
void createFile(char* fileName){
	fflush(stdin);
	system("cls");
	printf("\n��� ������?\n\n");
	gets(fileName);
	while (strlen(fileName) > 24){
		puts("\n����� �����. ������� ���-������ ��������.\n");
		gets(fileName);
	}
	FILE* f = fopen(fileName, "rb");
	if (f){
		puts("\n� ��� ����� ��� ����.");
		fclose(f);
		return;
	}
	else {
		fclose(f);
		f = fopen(fileName, "wb");
		if (f) puts("\n�� �������, ���� ������!");
		else puts("�� ����������(");
		fclose(f);
	}
}

//����� ������
void printRecords(char* fileName){
	fflush(stdin);
	system("cls");
	puts("\n����� ���� �������� �����?\n");
	FILE *f;
	gets(fileName);
	f = fopen(fileName, "rb");
	if (f){
		struct employee record;
		printf("----------------------------------------------------------------------------------------\n");
		printf("| �������         | ��� �������� | ���   | �����������       |��� ����������� �� ������|\n");
		printf("----------------------------------------------------------------------------------------\n");	
		while (fread(&record, sizeof(record), 1, f)){
			printf("| %-15s | %-12d | %-5c | %-17s | %-23d |\n", record.surname, record.YOB, record.sex, record.ed, record.YOE);
		}
		printf("----------------------------------------------------------------------------------------\n\n");
	}	
	else puts("\n��� ������.\n");	
}

//�������� ���-�� � ����
void addRecord(char* fileName){
	fflush(stdin);
	system("cls");
	puts("\n����� ���� �������� �����? ���� ��� ���, �� �� ����������, �������� �����!\n");
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
			puts("\n������ ���������.");
			puts("\n�� ������ ��� ���-������ ��������?(1 - ����/�� 1 - �� ����)\n");
			scanf("%d", &a);
			if (a != 1){
				puts("\n�����, ������.");
				return;
			}
		} while (a != 2);
		
	}	
	else puts("\n�� ����������(\n");
}

//�������� �����
void deleteRecord(char* fileName){
	fflush(stdin);
	system("cls");
	char sn[20];
	int i = 0;
	puts("\n� ����� ����� ���� ������?\n");
	FILE *f, *fs;
	gets(fileName);
	f = fopen(fileName, "rb");
	if (f){
		struct employee record;
		printf("----------------------------------------------------------------------------------------\n");
		printf("| �������         | ��� �������� | ���   | �����������       |��� ����������� �� ������|\n");
		printf("----------------------------------------------------------------------------------------\n");	
		while (fread(&record, sizeof(record), 1, f)){
			printf("| %-15s | %-12d | %-5c | %-17s | %-23d |\n", record.surname, record.YOB, record.sex, record.ed, record.YOE);
		}
		printf("----------------------------------------------------------------------------------------\n\n");
		fflush(stdin);
		puts("���� ���������?\n");
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
		if (i == 0) puts("\n��� ��� �����? � ��� ����� ���.");
		else puts("\n������ ���������.");
		fclose(f);
		fclose(fs);
		remove(fileName);
		rename("BrandoLab", fileName);
	}
	else puts("\n�� ����������(");
//	perror("Status");
}

//�������������
void editRecord(char* fileName){
	fflush(stdin);
	system("cls");
	puts("\n����� ���� �������������� �����?\n");
	FILE *f;
	gets(fileName);
	f = fopen(fileName, "r+b");
	if (f){
		char sn[20], e[20], s;
		int year, emp, i = 0, c;
		struct employee record;
		printf("----------------------------------------------------------------------------------------\n");
		printf("| �������         | ��� �������� | ���   | �����������       |��� ����������� �� ������|\n");
		printf("----------------------------------------------------------------------------------------\n");	
		while (fread(&record, sizeof(record), 1, f)){
			printf("| %-15s | %-12d | %-5c | %-17s | %-23d |\n", record.surname, record.YOB, record.sex, record.ed, record.YOE);
		}
		printf("----------------------------------------------------------------------------------------\n\n");
		fflush(stdin);
		puts("������� ������� ����������, ������ � ������� ���� ��������.\n");
		scanf("%s", sn);
		rewind(f);
		while (fread(&record, sizeof(record), 1, f)){
			if (strcmp(sn, record.surname) == 0){
				i++;
				do {
					puts("\n��� �� �� ������ ��������?\n");
					puts("1. �������\n2. ��� ��������\n3. ���\n4. �����������\n5. ��� ����������� �� ������\n6. ��������� ���� ������\n");
					scanf("%d", &c);
					switch (c){
						case 1:
							printf("\n������� ����� �������: ");
							while (scanf("%s", sn) != 1 || strlen(sn) > 20){
								puts("\n����������. ������� ��� ���.\n");
							}
							fseek(f, -sizeof(record), SEEK_CUR);
							fwrite(&sn, sizeof(record.surname), 1, f);
							fseek(f, 0, SEEK_CUR);
							puts("\n������������� ������.");
							break;
						case 2:
							printf("\n������� ����� ��� ��������: ");
							while (scanf("%d", &year) != 1 || year > 2005 || year <= 1700){
								puts("\n���� �� �����. �� ����� ������� �� ����. ������� ��� ���.\n");
							}	
							fseek(f, -sizeof(record) + sizeof(record.surname), SEEK_CUR);
							fwrite(&year, sizeof(record.YOB), 1, f);
							fseek(f, 0, SEEK_CUR);
							puts("\n������������� ������.");
							break;
						case 3:
							fflush(stdin);
							printf("\n������� ����� ���: ");
							while (scanf("%c", &s) != 1 || s != 'M' && s != 'F' && s != 'O'){
								puts("\n������, �������, �� �� ���� ����� �� ������������. ������� ��� ���.\n");
								fflush(stdin);
							}
							fseek(f, -sizeof(record) + sizeof(record.surname) + sizeof(record.YOB), SEEK_CUR);
							fwrite(&s, sizeof(record.sex), 1, f);
							fseek(f, 0, SEEK_CUR);
							puts("\n������������� ������.");
							break;
						case 4:
							fflush(stdin);
							printf("\n������� ����� �����������: ");
							while (scanf("%s", e) != 1 || strlen(e) > 20){
								puts("\n������� ������ � �������. ������� ��� ���.\n");
								fflush(stdin);
							}
							fseek(f, -sizeof(record) + sizeof(record.surname) + sizeof(record.YOB) + sizeof(record.sex), SEEK_CUR);
							fwrite(&e, sizeof(record.ed), 1, f);
							fseek(f, 0, SEEK_CUR);
							puts("\n������������� ������.");
							break;
						case 5:
							fflush(stdin);
							printf("\n��� ����������� �� ������: ");
							while (scanf("%d", &emp) != 1 || emp <= 1700){
								puts("\n���-�� �� ���������. ������� ��� ���.");
								fflush(stdin);
							}
							fseek(f, -sizeof(record) + sizeof(record.surname) + sizeof(record.YOB) + sizeof(record.sex) + sizeof(record.ed), SEEK_CUR);
							fwrite(&emp, sizeof(record.YOE), 1, f);
							fseek(f, 0, SEEK_CUR);
							puts("\n������������� ������.");
							break;
						case 6:
							puts("\n���������.");
							break;
						default:
							puts("\n�� �� �����, ��� ��� �����.");
							break;
					}
				}	while (c != 6);
				break;
			}
		}
		if (i == 0) puts("\n��� � ��� ������.");
	}	
	else puts("\n��� ������.");
	fclose(f);
}

//����� ���
void TheMan(char* fileName){
	fflush(stdin);
	system("cls");
	char sn[20];
	int year = 2021;
	FILE *f;	
	puts("\n� ����� ������ ��������?\n");
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
			printf("\n����� ��� � ��� %s.\n", sn);
		}
		else puts("\n��� ������ ���.");	
	}
	else puts("\n��� ������.");
}

//�� ����
void youngEmployees(char* fileName){
	fflush(stdin);
	system("cls");
	puts("\n����� ���� �������� �����?\n");
	FILE *f;
	gets(fileName);
	f = fopen(fileName, "rb");
	if (f){
		int i = 0;
		struct employee record;
		printf("----------------------------------------------------------------------------------------\n");
		printf("| �������         | ��� �������� | ���   | �����������       |��� ����������� �� ������|\n");
		printf("----------------------------------------------------------------------------------------\n");	
		while (fread(&record, sizeof(record), 1, f)){
			if (2021 - record.YOB < 28 && strcmp("Higher", record.ed) == 0){
				printf("| %-15s | %-12d | %-5c | %-17s | %-23d |\n", record.surname, record.YOB, record.sex, record.ed, record.YOE);
				i = 1;
			}
		}
		printf("----------------------------------------------------------------------------------------\n");
		if (i == 0) puts("\n� ��� ������ ���������� � ���������.");
	}	
	else puts("\n��� ������.");
}

//�������
int main() {
	setlocale(LC_ALL, "Rus");
	
	char fileName[25];
	
	puts("\n������������, �������. ��� ����� ������?");
	int point;
	do{
		point = menu(point);
		switch (point){
			case 1: //������� ����
				createFile(fileName);
				done();
				break;
			case 2: //����������� ����
				printRecords(fileName);
				done();
				break;
			case 3: //�������� ���-�� � ����
				addRecord(fileName);
				done();
				break;
			case 4: //�����
				deleteRecord(fileName);
				done();
				break;
			case 5: //�������������
				editRecord(fileName);
				done();
				break;
			case 6: //����������-�������
				TheMan(fileName);
				done();
				break;
			case 7: //�����������
				youngEmployees(fileName);
				done();
				break;				
			case 8: //��������
				system("cls");
				printf("\n����� ��������. �������� ���� � ����� �������.\n");
				return 0;
			default: //���� ������������ �������
				system("cls");
				printf("\n����� ��� � ����� ����. �������� ���-������ ������.\n");
				break;
		}
		fflush(stdin);
	} while (point != 8);
	
	return 0;
}

