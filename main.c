#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#ifndef MAX_NAME_LENGTH
#define MAX_NAME_LENGTH 65
#endif

#ifndef MAX_PHONE_NR_LENGTH
#define MAX_PHONE_NR_LENGTH 15
#endif

typedef struct patient
{
    char name[MAX_NAME_LENGTH];
    short unsigned year;
    char phone_nr[MAX_PHONE_NR_LENGTH];
    bool pays;
} patient_t;

patient_t *load(char *fn, int *c);
void save(char *fname, patient_t *p, int size);
void mainmenu(patient_t **patients, short unsigned *choice, int *size);
bool printps(patient_t *ps, int size);
patient_t delete (patient_t **ps, int *size);
bool add(patient_t **ps, int *size);
int change(patient_t *ps, int size);

int main(int argc, char **argv)
{
    patient_t *tmp = NULL;

    int size = 0;
    short unsigned* op = (short unsigned *)malloc(sizeof(short unsigned));

    do
    {
        mainmenu(&tmp, op, &size);
    } while (*op != 0);

    free(tmp);
    return EXIT_SUCCESS;
}

patient_t *load(char *fn, int *c)
{
    FILE *f = fopen(fn, "r");
    if (!f)
    {
        printf("File %s not found!\n", fn);
        return NULL;
    }
    int _c = 0;
    patient_t *p = (patient_t *)malloc(sizeof(patient_t));

    char line[121];
    while (fgets(line, 120, f))
    {
        char *newname = strtok(line, "|");
        strcpy(p[_c].name, newname);

        short unsigned newyear = atoi(strtok(NULL, "|"));
        p[_c].year = newyear;

        char *newnr = strtok(NULL, "|");
        strcpy(p[_c].phone_nr, newnr);

        int newpay = atoi(strtok(NULL, "|"));
        p[_c].pays = newpay;

        ++_c;
        p = (patient_t *)realloc(p, sizeof(patient_t) * (_c + 1));
        line[0] = 0;
    }

    fclose(f);
    *c = _c;
    printf("Loaded pointer: %p\n", p);
    return p;
}

void save(char *fname, patient_t *ps, int size)
{
    if (ps == NULL)
    {
        printf("\nNincs lista");
        return;
    }
    FILE *f = fopen(fname, "w");

    patient_t *p;
    for (int i = 0; i < size; ++i)
    {
        p = ps + i;
        fprintf(f, "%s|", p->name);
        fprintf(f, "%hu|", p->year);
        fprintf(f, "%s|", p->phone_nr);
        fprintf(f, "%i", p->pays ? 1 : 0);
        fprintf(f, "\n");
    }

    fclose(f);
}

patient_t delete (patient_t **ps, int *size)
{
    patient_t del = {.name = "HIBA"};
    if (*ps == NULL || ps == NULL || size == NULL || *size <= 0)
        printf("\nNincs ");
    else
    {

        printf("\nH??nyadik p??cienst t??r??ln??: ");

        int i;
        char tmpbuff[12];

        scanf("%s", tmpbuff);
        if(isdigit(tmpbuff[0]))
            i = atoi(tmpbuff);
        else
            i = -99;

        if (*size > --i || i < 0 )
        {
            del = *(*ps + i);
            for (int c = i; c < *size-1; c++)
                ps[c] = ps[c + 1];
            *ps = (patient_t *)realloc(ps, --(*size) * sizeof(patient_t));
        }
        else
            printf("\nRossz sorsz??m!");
    }
    return(del);
}

bool add(patient_t ** ps, int *size)
{
    patient_t newp = {};

    char buff[120];
    printf("??j p??ciens neve (Max. %i karakter): ", MAX_NAME_LENGTH-1);
    scanf("%c", &buff[0]);
    scanf("%[^\n]", newp.name);

    do{
        printf("??j p??ciens sz??let??si ??ve: ");
        scanf("%s", buff);
        newp.year = atoi(buff);
    } while( !(newp.year > 1902 && newp.year < 10000) );

    printf("??j p??ciens telefonsz??ma (Max. %i karakter): ", MAX_PHONE_NR_LENGTH-1);
    scanf("%s", buff);
    strcpy(newp.phone_nr, buff);

    bool success = true;

    for(int i = 0; i < strlen(newp.phone_nr); ++i)
        if( i < strlen(newp.phone_nr) && !isdigit(newp.phone_nr[i]))
            success = false;

    printf("??j p??ciens fizet?? (1/0): ");
    scanf("\n%c", &buff[0]);
    newp.pays = atoi(buff);

    if(success)
    {
        *ps = (patient_t*)realloc(*ps, ++(*size) * sizeof(patient_t));
        (*ps)[*size - 1] = newp;
    }

    return(success);
}

int change(patient_t *ps, int size)
{
    int pos = -99;
    if (ps == NULL)
        printf("\nNincs lista");
    else
    {
        int i;
        printf("\nH??nyadik p??cienst m??dos??tan??: ");
        
        char tmpbuff[12];
        scanf("%s", tmpbuff);
        if(isdigit(tmpbuff[0]))
            i = atoi(tmpbuff);
        else
            i = -99;
        i--;
        printf("Majom");
        if (size > i && i >= 0)
        {
            int op;
            patient_t *memory = ps + i;
            patient_t  *p = (patient_t*)malloc(sizeof(patient_t));
            *p = *memory;
            do
            {
                printf("Mit szeretne m??dos??tani?\n 1 - N??v\n 2 - Sz??let??si ??v\n 3 - Telefonsz??m\n 4 - Fizet??sre hajland??\n\n9 - VISSZA\n0 - MENT??S\nV??lasztott opci??: ");
                scanf("%i", &op);
                char buff[12];

                switch (op)
                {
                case 0:
                    *memory = *p;
                    free(p);
                    pos = i;
                    break;
                case 1:
                    printf("R??gi n??v: %s\n??j n??v: ", p->name);
                    scanf("%c", buff);
                    scanf("%[^\n]", p->name);
                    break;
                case 2:
                    printf("R??gi ??v: %hu\n??j ??v: ", p->year);
                    scanf("%hu", &p->year);
                    break;
                case 3:
                    printf("R??gi sz??m: %s\n??j sz??m: ", p->phone_nr);
                    scanf("%s", p->phone_nr);
                    break;
                case 4:
                    printf("R??gi ??llapot: %s\n??j ??llapot(1/0): ", p->pays ? "Igaz" : "Hamis");
                    scanf("\n%s", buff);
                    p->pays = atoi(buff);
                    break;
                case 9:
                    break;
                default:
                    printf("Nincs ilyen m??velet!\n");
                    break;
                }
                printf("%d", op);
            }while(op != 9 && op != 0);
        }
        else
            printf("Rossz sorsz??m!");
    }
    return(pos);
}

bool printps(patient_t *ps, int size)
{
    system("clear");
    bool success;
    if (ps == NULL)
    {
        success = false;
        printf("Nincs lista!");
    }
    else
    {
        if (size > 0)
            for (int i = 0; i < size; ++i)
                printf("%i: %s | %hu | %s | %s\n", i + 1, ps[i].name, ps[i].year, ps[i].phone_nr, ps[i].pays ? "Igaz" : "Hamis");
        else
            printf("Nincs p??ciens!");
        success = true;
    }
    return(success);
}

void mainmenu(patient_t **patients, short unsigned *choice, int *size)
{
    char fn[20] = "";

    printf("\n\n--------------------------------\n            --MENU--\n--------------------------------");
    printf("\nMit szeretn??l csin??lni: ");
    printf("\n1 - Jelenlegi lista ki??r??sa");
    printf("\n2 - Jelenlegi lista m??dos??t??sa");
    printf("\n3 - Jelenlegi list??b??l t??rl??s");
    printf("\n4 - Jelenlegi list??ba ??j ad??s");
    printf("\n5 - Lista beolvas??sa f??jb??l");
    printf("\n6 - Lista ment??se f??jlba");
    printf("\n\n0 - KIL??P??S");
    printf("\n*********************************\nMit szeretn??l csin??lni: ");

    char tmpbuff[120];
    scanf("%s", tmpbuff);
    if(isdigit(tmpbuff[0]))
        *choice = atoi(tmpbuff);
    else
        *choice = 99;

    switch (*choice)
    {
    case 0:
        exit(0);
        break;
    case 1:
        printps(*patients, *size);
        break;
    case 2:
        printps(*patients, *size);
        change(*patients, *size);
        break;
    case 3:
        printps(*patients, *size);
        patient_t deltmp = delete(patients, size);
        if( !strcmp(deltmp.name, "HIBA"))
            printf("\n%s sikeresen t??r??lve lett!", deltmp.name);
        else
            printf("\nSikertelen t??rl??s!");
        break;
    case 4:
        if(add(patients, size))
            printf("\n%s sikeresen fel lett v??ve az adatb??zisba %d-ik p??ciensk??nt", "Ap??d", *size);
        else
            printf("\nAdatr??gz??t??s sikertelen!");
        break;
    case 5:
        printf("\nF??jln??v: ");
        scanf("%s", fn);

        if (*patients != NULL)
            free(*patients);

        *patients = load(fn, size);
        break;
    case 6:
        printf("\nF??jln??v: ");
        scanf("%s", fn);
        save(fn, *patients, *size);
        break;

    default:
        system("clear");
        printf("Hib??s v??laszt??s");
    }
}