#define unsInt unsigned int

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <string>

/*

    problems with scrolling back / forward exist sometimes
*/

using namespace std;

enum instr{guitar = 0, bass_guitar, synth};

enum neck{BOLT_ON = 0, SET_NECK, NECK_TROUGH};

enum bassType{PRECISION = 0, JAZZ, HALF_ACOUSTIC, NO_FRETS};

enum logic{NO = 0, YES};

enum comparing{DIFFERENT = 0, SAME, SIMILAR};

enum handOrientation{LH = 0, RH};

//structs

struct bassInfo
{
    enum bassType typeOfBass : 2;
    unsInt strings : 4;
    enum neck neck : 2;
    unsInt frets : 5;
    enum handOrientation hand : 1;
}; // 14
struct guitarInfo
{
    unsInt strings : 6;
    enum neck neck : 2;
    unsInt frets : 6;
    enum handOrientation hand : 1;
}; // 14
struct synthInfo
{
    unsInt keys : 7;
    enum logic usb : 1;
    enum logic modulWheel : 1;
    enum logic imitHamMech : 1;
    unsInt analOutput : 4;
}; // 14
union adv
{
    struct guitarInfo guiInfo;
    struct synthInfo synInfo;
    struct bassInfo bassInfo;
};
struct elemInfo
{
    char company_name[30];
    char company_mail[30];
    char model_name[30];
    enum instr instrType;
    
    unsInt yearProduce : 12;
    double weight;

    bool wasInUse;
    unsInt itemNumb;
    unsInt price;

    union adv adv;
};
struct dBaseElement
{
    struct elemInfo aboutElem;
    struct dBaseElement *nextItem;
    struct dBaseElement *prevItem;
};
struct dBCategory
{
    enum instr musInstrCategory;    

    struct dBCategory *nextCategory;
    struct dBCategory *prevCategory;

    struct dBaseElement *dBFirstElem;
    struct dBaseElement *dBLastElem;

    unsigned long itemsNumb;
};
struct dBase
{
    unsInt categQuant = 0;
    struct dBCategory *firstCateg;
    struct dBCategory *lastCateg;
};
struct foundElemData
{
    dBaseElement *found_element;
    enum comparing isSame;
};

//functions
void clearscr ( void );

struct dBaseElement *musElemAlloc(void);

struct dBCategory *categAlloc(void);

struct dBase *dBAlloc(void);

void initDataBase(struct dBase *dB);

void delDataBase(struct dBase *dB);

void readFromFile(struct dBase *dB, FILE *file);

struct foundElemData *findElem(struct dBase *dB, struct dBaseElement *potElement);

struct dBCategory *getCateg(struct dBase *dB, struct dBaseElement *elem);

void withdrawElements(struct dBase *dB, struct dBaseElement *elemToWithdrow, int numbToWithdraw);

void delElem(struct dBase *dB, struct dBaseElement *elemToDel);

enum comparing dBaseElemsCmp(struct dBaseElement *first, struct dBaseElement *second);

void printAdvElemInfo(struct dBaseElement *element);

void printBasicElemInfo(struct dBaseElement *element);

void printCategInfo(struct dBCategory *category);

void delElem(struct dBase *dB, struct dBaseElement *elemToDel);

struct dBaseElement *getInfo();

void itemOption(struct dBase *dB, struct dBaseElement *actElem);

void addItem(struct dBase *dB, dBaseElement *newElem);

void addItemAfter(struct foundElemData *isElementInCateg, dBaseElement *newElem);

void addItemBefore(struct foundElemData *isElementInCateg, dBaseElement *newElem);

void addFirstItem(struct dBCategory *Category, dBaseElement *newElem);

void scrollDB(struct dBase *dB);

char *FGETS(char *str, int num, FILE *stream);


int main()
{    
    char usrAnswr = 's';
    int tempInt = 0;
    struct dBase *MuzShop = NULL;   

    clearscr();
    while(tempInt != 4)
    {
        if(MuzShop != NULL)
            printf("0| Scroll the Data Base\n");
        if(MuzShop == NULL)
            printf("1| Init base\n");
        if(MuzShop != NULL)
            printf("2| Del base\n");
        if(MuzShop != NULL)
            printf("3| Read from file\n");
        printf("4| Exit\n");
        scanf("%d", &tempInt);
        clearscr();
        if(tempInt == 0)
        {
            scrollDB(MuzShop);
            clearscr();
        }    
        else if(tempInt == 1 && MuzShop == NULL)
        {
            MuzShop = dBAlloc();
            initDataBase(MuzShop);
            printf("database is initialized");
        }
        else if(tempInt == 2 && MuzShop != NULL)
        {
            delDataBase(MuzShop);
            printf("database successfully deleted\n");
        }
        else if(tempInt == 3 && MuzShop != NULL)
        {
            FILE *fl;
            fl = fopen("database/input.txt", "r");    
            feof(fl);        
            readFromFile(MuzShop, fl);
            fclose(fl);
            clearscr();
            printf("file is succesfully read\n");
        }
        else if(tempInt == 4)
            if(MuzShop != NULL)
                delDataBase(MuzShop);
        
    }
    
    return 0;
}

void clearscr ( void )
{
    printf("\033[2J\033[1;1H");
}

struct dBaseElement *musElemAlloc(void)
{
    struct dBaseElement *output = (struct dBaseElement *)malloc(sizeof(struct dBaseElement));
    output->nextItem = NULL;
    output->prevItem = NULL;
    return output;
}

struct dBCategory *categAlloc(void)
{
    return (struct dBCategory *)malloc(sizeof(struct dBCategory));
}

struct dBase *dBAlloc(void)
{
    return (struct dBase *)malloc(sizeof(struct dBase));
}

void initDataBase(struct dBase *dB)
{
    struct dBCategory *tempCateg = categAlloc();

    dB->firstCateg = tempCateg;

    tempCateg->musInstrCategory = guitar;
    tempCateg->itemsNumb = 0;
    tempCateg->dBFirstElem = NULL;
    tempCateg->dBLastElem = NULL;

    tempCateg->nextCategory = categAlloc();
    tempCateg = tempCateg->nextCategory;
    
    tempCateg->musInstrCategory = bass_guitar;
    tempCateg->itemsNumb = 0;
    tempCateg->dBFirstElem = NULL;
    tempCateg->dBLastElem = NULL;
    tempCateg->nextCategory = categAlloc();
    tempCateg = tempCateg->nextCategory;  

    tempCateg->musInstrCategory = synth;
    tempCateg->itemsNumb = 0;
    tempCateg->dBFirstElem = NULL;
    tempCateg->dBLastElem = NULL;  

    dB->lastCateg = tempCateg;
    dB->categQuant = 3;

    tempCateg = NULL;
}

void delDataBase(struct dBase *dB)
{
    struct dBCategory *pTempCateg = dB->firstCateg;
    struct dBaseElement *pTempElem;
    
    while(pTempCateg != NULL){
        pTempElem = pTempCateg->dBFirstElem;
        if(pTempElem != NULL)
        {
            while(pTempElem != pTempCateg->dBLastElem)
            {
                pTempElem = pTempElem->nextItem;
                free(pTempElem->prevItem);
            }
            free(pTempElem);
        }
        if(pTempCateg->nextCategory != NULL)
        {
            pTempCateg = pTempCateg->nextCategory;
            free(pTempCateg->prevCategory);
        }
    }
    free(pTempCateg);   
    free(dB);
}

void readFromFile(struct dBase *dB, FILE *file)
{
    if (feof(file))
		fseek(file, 0, SEEK_SET);
	char str[256];
    int temp;
    struct dBaseElement *newElem;
    while (!feof(file))
	{
        newElem = musElemAlloc();

        FGETS(str, 30, file);
		while (str[0] == '\n' || str[0] == '\0')
		{
			FGETS(str, 30, file);
			if (feof(file))
				return;
		}
        if(strcmp(str, "guitar") == 0)
            newElem->aboutElem.instrType = guitar;
        else if(strcmp(str, "bass guitar") == 0)
            newElem->aboutElem.instrType = bass_guitar;
        else if(strcmp(str, "synth") == 0)
            newElem->aboutElem.instrType = synth;
        
        //company name
        fscanf(file, "%s", str);
        strcpy(newElem->aboutElem.company_name, str);

        //company mail
        fscanf(file, "%s", str);
        strcpy(newElem->aboutElem.company_mail, str);

        //item model name
        fscanf(file, "%s", str);
        strcpy(newElem->aboutElem.model_name, str);

        fscanf(file, "%s", str);
        if(str[0] == 'y')
            newElem->aboutElem.wasInUse = true;
        else
            newElem->aboutElem.wasInUse = false;
        
        //weight
        //fgets(str, 30, file);
        fscanf(file, "%lf", &(newElem->aboutElem.weight));
        newElem->aboutElem.weight = int(newElem->aboutElem.weight * 10) / 10.;

        //number of item
        fscanf(file, "%d", &(newElem->aboutElem.itemNumb));        
        
        //price
        fscanf(file, "%d", &(newElem->aboutElem.price));        
        
        //year of produce
        fscanf(file, "%d", &(temp));        
        newElem->aboutElem.yearProduce = temp;

        if(newElem->aboutElem.instrType == guitar)
        {
            fscanf(file, "%d%s", &(temp), str);
            newElem->aboutElem.adv.guiInfo.frets = temp;
            if(strcmp(str, "LH") == 0 || strcmp(str, "lh") == 0)
                newElem->aboutElem.adv.guiInfo.hand = LH;
            else
                newElem->aboutElem.adv.guiInfo.hand = RH;  
            
            fscanf(file, "%s", str);
            if(strcmp(str, "BO") == 0 || strcmp(str, "bo") == 0 ||
                strcmp(str, "bolt_neck") == 0 || strcmp(str, "BOLT_NECK") == 0)
                newElem->aboutElem.adv.guiInfo.neck = BOLT_ON;
            else if(strcmp(str, "SN") == 0 || strcmp(str, "sn") == 0 ||
                    strcmp(str, "SET_NECK") == 0 || strcmp(str, "set_neck") == 0)
                newElem->aboutElem.adv.guiInfo.neck = SET_NECK;
            else if(strcmp(str, "NT") == 0 || strcmp(str, "nt") == 0 ||
                    strcmp(str, "NECK_TROUGH") == 0 || strcmp(str, "neck_trough") == 0)
                newElem->aboutElem.adv.guiInfo.neck = NECK_TROUGH;
            else
                printf("problem with neck definition, 486 line");
            
            fscanf(file, "%d", &(temp));
            newElem->aboutElem.adv.guiInfo.strings = temp;
        }
        else if(newElem->aboutElem.instrType == bass_guitar)
        {
            fscanf(file, "%d%s", &(temp), str);
            newElem->aboutElem.adv.guiInfo.frets = temp;
            if(strcmp(str, "LH") == 0 || strcmp(str, "lh") == 0)
                newElem->aboutElem.adv.guiInfo.hand = LH;
            else
                newElem->aboutElem.adv.guiInfo.hand = RH;

            fscanf(file, "%s", str);
            if(strcmp(str, "BO") == 0 || strcmp(str, "bo") == 0 ||
                strcmp(str, "bolt_neck") == 0 || strcmp(str, "BOLT_NECK") == 0)
                newElem->aboutElem.adv.guiInfo.neck = BOLT_ON;
            else if(strcmp(str, "SN") == 0 || strcmp(str, "sn") == 0 ||
                    strcmp(str, "SET_NECK") == 0 || strcmp(str, "set_neck") == 0)
                newElem->aboutElem.adv.guiInfo.neck = SET_NECK;
            else if(strcmp(str, "NT") == 0 || strcmp(str, "nt") == 0 ||
                    strcmp(str, "NECK_TROUGH") == 0 || strcmp(str, "neck_trough") == 0)
                newElem->aboutElem.adv.guiInfo.neck = NECK_TROUGH;
            else
                printf("problem with neck definition, 486 line");

            fscanf(file, "%d", &(temp));
            newElem->aboutElem.adv.bassInfo.strings = temp;

            fscanf(file, "%s", str);
            if(strcmp(str, "precision") == 0)
                newElem->aboutElem.adv.bassInfo.typeOfBass = PRECISION;
            else if(strcmp(str, "jazz") == 0)
                newElem->aboutElem.adv.bassInfo.typeOfBass = JAZZ;
            else if(strcmp(str, "half acoustic") == 0)
                newElem->aboutElem.adv.bassInfo.typeOfBass = HALF_ACOUSTIC;
            else if(strcmp(str, "nofrets") == 0 || strcmp(str, "no frets") == 0)
                newElem->aboutElem.adv.bassInfo.typeOfBass = NO_FRETS;
        }
        else if(newElem->aboutElem.instrType == synth)
        {
            fscanf(file, "%s", str);
            if(str[0] == 'y')
                newElem->aboutElem.adv.synInfo.imitHamMech = YES;
            else
                newElem->aboutElem.adv.synInfo.imitHamMech = NO;
            
            fscanf(file, "%d", &(temp));
            newElem->aboutElem.adv.synInfo.keys = temp;

            fscanf(file, "%s", str);
            if(str[0] == 'y')
                newElem->aboutElem.adv.synInfo.usb = YES;
            else
                newElem->aboutElem.adv.synInfo.usb = NO;
            
            fscanf(file, "%s", str);
            if(str[0] == 'y')
                newElem->aboutElem.adv.synInfo.modulWheel = YES;
            else
                newElem->aboutElem.adv.synInfo.modulWheel = NO;
            
            fscanf(file, "%d", &(temp));
            newElem->aboutElem.adv.synInfo.analOutput = temp;
        }

        addItem(dB, newElem);
    }
}

struct foundElemData *findElem(struct dBase *dB, struct dBaseElement *potElement)
{
    struct foundElemData *output = (struct foundElemData *)malloc(sizeof(struct foundElemData));
    output->found_element = NULL;
    output->isSame = DIFFERENT;
    enum comparing tempStatus = DIFFERENT;
    //temp elem to save data about similar item 
    struct dBaseElement *foundElem = NULL;
    struct dBaseElement *tempElement = getCateg(dB, potElement)->dBFirstElem;

    /*
    if(givenCateg->nextCategory = NULL && givenCateg != potElement->aboutElem.instrType)
        print("theres no such category in db");
    */

    //checking first element of a category
    if(tempElement != NULL)
    {
        tempStatus = dBaseElemsCmp(tempElement, potElement);
        if(tempStatus == SAME)
        {
            output->found_element = tempElement;
            output->isSame = SAME;
        }
        else if(tempStatus == SIMILAR)
        {
            foundElem = tempElement;
        }
        //checkig other elements of a category
        while(tempElement->nextItem != NULL && output->isSame != SAME)
        {
            tempElement = tempElement->nextItem;
            tempStatus = dBaseElemsCmp(tempElement, potElement);
            if(tempStatus == SAME)
            {
                output->found_element = tempElement;
                output->isSame = SAME;
            }
            else if(tempStatus == SIMILAR)
            {
                foundElem = tempElement;
            }
        }
        if(output->isSame == DIFFERENT && foundElem != NULL)
        {
            output->found_element = foundElem;
            output->isSame = SIMILAR;
        }
    }
    return output;
}

struct dBCategory *getCateg(struct dBase *dB, struct dBaseElement *elem)
{
    struct dBCategory *categ = dB->firstCateg;     
    while(categ->nextCategory != NULL && categ->musInstrCategory != elem->aboutElem.instrType)
        categ = categ->nextCategory;
    
    if(categ->nextCategory == NULL && categ->musInstrCategory != elem->aboutElem.instrType)
    {
        categ = NULL;
        printf("\nneed categ seems like not exist at all\n");
    }
    return categ;
}

//more likely getting from stock (sellin) than deleting
void withdrawElements(struct dBase *dB, struct dBaseElement *elemToWithdrow, int numbToWithdraw)
{
    if(elemToWithdrow->aboutElem.itemNumb < numbToWithdraw)
    {
        printf("\nwe dont have such numb of this item, only : %d\n you'd better wait for arrival some time\n", elemToWithdrow->aboutElem.itemNumb);
    }
    else if(elemToWithdrow->aboutElem.itemNumb >= numbToWithdraw)
    {
        elemToWithdrow->aboutElem.itemNumb -= numbToWithdraw;
    }
}

//deleting fully like elem from the base but not the item from stock
void delElem(struct dBase *dB, struct dBaseElement *elemToDel)
{
    struct dBCategory *tempCateg = dB->firstCateg;
    while(tempCateg != NULL && tempCateg->musInstrCategory != elemToDel->aboutElem.instrType)
        tempCateg = tempCateg->nextCategory;
    
    if(tempCateg != NULL)
    {
        tempCateg->itemsNumb--;
        if(elemToDel->prevItem != NULL)
        {
            elemToDel->prevItem->nextItem = elemToDel->nextItem;            
        }
        if(elemToDel->nextItem != NULL)
        {
            elemToDel->nextItem->prevItem = elemToDel->prevItem;
        }
        free(elemToDel);
    }
    else
    {
        printf("!!! you have pointer on element, but u dont have the category of that element in database, smth went wrong !!!");
    }
}

enum comparing dBaseElemsCmp(struct dBaseElement *first, struct dBaseElement *second)
{
    enum comparing output = DIFFERENT;
    printf("%s",first->aboutElem.company_name);
    if(strcmp(first->aboutElem.company_name, second->aboutElem.company_name) == 0 &&
        strcmp(first->aboutElem.model_name, second->aboutElem.model_name) == 0 &&
        first->aboutElem.instrType == second->aboutElem.instrType)
        {
            if(first->aboutElem.yearProduce == second->aboutElem.yearProduce && 
                first->aboutElem.wasInUse == second->aboutElem.wasInUse)
                {
                    output = SAME;
                }
            else 
                    output = SIMILAR;     
        }
    return output;
}

void printAdvElemInfo(struct dBaseElement *element)
{
    clearscr();
    printf("\n___________________________________________\n");
    printf("Company name : %s\n", element->aboutElem.company_name);
    printf("Model name   : %s\n", element->aboutElem.model_name);
    printf("instrument type : ");
    if(element->aboutElem.instrType == guitar)
    {
        printf("guitar\n");
        printf("with %d strings\n", element->aboutElem.adv.guiInfo.strings);
        printf("guitar has ");
        if(element->aboutElem.adv.guiInfo.neck == 0)
            printf("BOLT ON neck");
        else if(element->aboutElem.adv.guiInfo.neck == 1)
            printf("SET NECK \n");
        else if(element->aboutElem.adv.guiInfo.neck == 2)
            printf("NECK TROUGH");
        printf("%d frets\n", element->aboutElem.adv.guiInfo.frets);
        if(element->aboutElem.adv.guiInfo.hand == LH)
            printf("and its LEFT HANDED\n");
        else
            printf("and its RIGH HANDED\n");

    }
    else if(element->aboutElem.instrType == bass_guitar)
    {
        printf("bass guitar\n");
        printf("with %d strings\n", element->aboutElem.adv.bassInfo.strings);
        printf("%d frets\n", element->aboutElem.adv.bassInfo.frets);
        printf("type of bass : ");
        if(element->aboutElem.adv.bassInfo.typeOfBass == PRECISION)
            printf("PRECISION\n");
        else if(element->aboutElem.adv.bassInfo.typeOfBass == JAZZ)
            printf("JAZZ\n");
        else if(element->aboutElem.adv.bassInfo.typeOfBass == NO_FRETS)
            printf("NO FRETS\n");
        else if(element->aboutElem.adv.bassInfo.typeOfBass == HALF_ACOUSTIC)
            printf("HALF ACOUSTIC\n");

        printf("bass guitar has ");  
        if(element->aboutElem.adv.bassInfo.neck == 0)
            printf("BOLT ON neck");
        else if(element->aboutElem.adv.bassInfo.neck == 1)
            printf("SET NECK \n");
        else if(element->aboutElem.adv.bassInfo.neck == 2)
            printf("NECK TROUGH");

        if(element->aboutElem.adv.bassInfo.hand == LH)
            printf("and its LEFT HANDED\n");
        else
            printf("and its RIGH HANDED\n");
    }
    else if(element->aboutElem.instrType == synth)
    {
        printf("synthesizer\n");
        printf("with %d keys\n", element->aboutElem.adv.synInfo.keys);
        printf("usb : ");
        if(element->aboutElem.adv.synInfo.usb == 1)
            printf("yes\n");
        else
            printf("no\n");

        printf("modulation wheel : ");
        if(element->aboutElem.adv.synInfo.modulWheel == 1)
            printf("yes\n");
        else
            printf("no\n");
        
        printf("hammer imitation mechanism : ");
        if(element->aboutElem.adv.synInfo.imitHamMech == 1)
            printf("yes\n");
        else
            printf("no\n");
        
        printf("and %d analog outputs\n", element->aboutElem.adv.synInfo.analOutput);
    }
    printf("\nnumb of the instrument we have in stock : %d\n", element->aboutElem.itemNumb);
    printf("the weight of the instrument : %f\n", element->aboutElem.weight);
    printf("was in use : ");
    if(element->aboutElem.wasInUse)
    {
        printf("yes\n\n");
    }
    else
    {
        printf("no\n\n");    
    }
    printf("price : %d dollars\n", element->aboutElem.price);
    printf("___________________________________________\n");
}

void printBasicElemInfo(struct dBaseElement *element)
{
    clearscr();
    printf("Company name : %s\n", element->aboutElem.company_name);
    printf("Model name   : %s\n", element->aboutElem.model_name);
    printf("instrument type : ");
    if(element->aboutElem.instrType == guitar)
        printf("guitar\n");
    else if(element->aboutElem.instrType == bass_guitar)
        printf("bass guitar\n");
    else if(element->aboutElem.instrType == synth)
        printf("synthesizer\n");
    if(element->aboutElem.itemNumb == 0)
        printf("out of stock, contact seller for advanced info\n");
    else
        printf("numb of the instrument we have in stock : %d\n", element->aboutElem.itemNumb);
    printf("price : %d dollars\n", element->aboutElem.price); 
}

void printCategInfo(struct dBCategory *category)
{
    printf("category instrument type : ");
    if(category->musInstrCategory == guitar)
        printf("guitar\n");
    else if(category->musInstrCategory == bass_guitar)
        printf("bass guitar\n");
    else if(category->musInstrCategory == synth)
        printf("synthesizer\n");
    printf("items numb in category : %lu\n", category->itemsNumb);
}

struct dBaseElement *getInfo()
{
    struct dBaseElement *potElem = musElemAlloc();
    int temp = -1;

    printf("\ncompany name : ");
    scanf("%s", potElem->aboutElem.company_name);
    printf("\nitem model name : ");
    scanf("%s", potElem->aboutElem.model_name);
    printf("\ncategory of the instrument");
    while(temp < 1 || temp > 3){
        printf("\n1| guitar\n2| Bass guitar\n3| synth\n");
        scanf("%d", &temp);
    }
    potElem->aboutElem.instrType = (enum instr)(temp - 1);
    printf("year of produce : ");
    scanf("%d", &(temp));
    potElem->aboutElem.yearProduce = temp;
    printf("was it in use\n1| yes\n2| no\n");
    scanf("%d", &temp);
    if(temp == 1)
        potElem->aboutElem.wasInUse = YES;
    else
        potElem->aboutElem.wasInUse = NO;
    return potElem;
}

void itemOption(struct dBase *dB, struct dBaseElement *actElem)
{
    int tempInt = 0;
    while(tempInt != 5){
        
        if(tempInt != 4)
        {
            clearscr();
            printBasicElemInfo(actElem);
        }
        printf("1| Stock item\n");
        if(actElem->aboutElem.itemNumb > 0)
            printf("2| Withdraw item\n");
        printf("3| Delete item from base\n");
        printf("4| Show advance info about item\n");
        printf("5| Back to scroll\n");
        scanf("%d", &tempInt);

        if(tempInt == 1)
        {           
            printf("how manny do u wanna stock?\n");
            tempInt = -1;
            while(tempInt < 0)
            {
                scanf("%d", &tempInt);
                if(tempInt < 0)
                    printf("cant stock negative number of items, write positive or zero\n\n");
            }
            actElem->aboutElem.itemNumb += tempInt;   
        }
        else if(tempInt == 2)
        {
            clearscr();
            printBasicElemInfo(actElem);
            
            tempInt = -1;
            while(tempInt < 0 || tempInt > actElem->aboutElem.itemNumb)
            {
                printf("we have %d\n", actElem->aboutElem.itemNumb);
                printf("print '0' to escape\n");
                printf("how many do u wanna withdraw : ");
                scanf("%d", &tempInt);
                if(tempInt < 0)
                    printf("cant stock negative number of items, write positive or zero\n\n");
                else if(tempInt > actElem->aboutElem.itemNumb)
                    printf("\nwe dont have such number of instrument"); 
            }
            if(tempInt != 0)
                withdrawElements(dB, actElem, tempInt);  
        }
        else if(tempInt == 3)
        {
            struct dBaseElement *temp;
            if(actElem->prevItem != NULL)
                temp = actElem->prevItem;
            else if(actElem->nextItem != NULL)
                temp = actElem->nextItem;
            else 
                temp = NULL;
            delElem(dB, actElem);
            
            if(temp != NULL)
                actElem = temp;
        }
        else if(tempInt == 4)
        {
            printAdvElemInfo(actElem);
        }
    }
}

void addItem(struct dBase *dB, dBaseElement *newElem)
{   
    struct foundElemData *isElementInCateg = findElem(dB, newElem);

    if(isElementInCateg->isSame == SAME)
    {
        isElementInCateg->found_element->aboutElem.itemNumb += newElem->aboutElem.itemNumb;
    }
    else if(isElementInCateg->isSame == SIMILAR)
    {
        if(isElementInCateg->found_element->aboutElem.wasInUse == 1)
            //wasInUse == 1 znachit stavim posle foundElem
            addItemAfter(isElementInCateg, newElem);
        else
            //wasInUse == 0 znachit do foundElem
            addItemBefore(isElementInCateg, newElem);
    }
    else
    {
        //no elements in categ
        addFirstItem(getCateg(dB, newElem), newElem);
    }
}

void addItemAfter(struct foundElemData *isElementInCateg, dBaseElement *newElem)
{    
    if(isElementInCateg->found_element->nextItem != NULL)
    {
        newElem->nextItem = isElementInCateg->found_element->nextItem;
        newElem->nextItem->prevItem = newElem;
    }
    newElem->prevItem = isElementInCateg->found_element;
    isElementInCateg->found_element->nextItem = newElem;
}

void addItemBefore(struct foundElemData *isElementInCateg, dBaseElement *newElem)
{
    if(isElementInCateg->found_element->prevItem != NULL)
    {
        newElem->prevItem = isElementInCateg->found_element->prevItem;
        newElem->prevItem->nextItem = newElem;
    }
    newElem->nextItem = isElementInCateg->found_element;
    isElementInCateg->found_element->prevItem = newElem;
}

void addFirstItem(struct dBCategory *Category, dBaseElement *newElem)
{
    if(Category->dBFirstElem == NULL)
    {
        Category->dBFirstElem = newElem;
        Category->dBLastElem = newElem;
    }
    else if(Category->dBFirstElem != NULL)
    {
        newElem->nextItem = Category->dBFirstElem;
        newElem->nextItem->prevItem = newElem;
        Category->dBFirstElem = newElem;
    }
}

void scrollDB(struct dBase *dB)
{
    enum instr actCateg;
    char usrAnswr;
    unsInt tempInt = 6;
    struct dBCategory *scrollingCategory = dB->firstCateg;
    struct dBaseElement *actElem;
    int exit = 5;

    while(tempInt != exit)
    {
        while(tempInt > exit)
        {
            clearscr();
            printf("items from which category wanna see?\n\
1| guitars\n\
2| basses\n\
3| synths\n\
4| find element\n\
5| exit\n");
            scanf("%d", &tempInt);
        }
        if(tempInt == 5)
            break;
        else if(tempInt == 4)
        {
            enum instr instr_categ;
            int temp = 0;
            while(temp < 1 || temp > 3)
            {
                printf("\nwhats the category of instrument?\n\
1| guitar\n\
2| bass\n\
3| synth");
                scanf("%d", &temp);
                clearscr();
            }
            instr_categ = (enum instr)temp;
            struct dBCategory *dBCategory = dB->firstCateg;
            while(dBCategory->musInstrCategory != instr_categ && dBCategory->nextCategory != NULL)
            {
                dBCategory = dBCategory->nextCategory;
            }
            if(dBCategory->musInstrCategory != instr_categ)
                printf("!!! u have problems with categs 298 line !!!");
            
            struct dBaseElement *elemToFind = getInfo();
            struct foundElemData *isElementInCateg = findElem(dB, elemToFind);
            if(isElementInCateg->found_element != NULL)
            {
                itemOption(dB, actElem);
            }else
                printf("\n\nsuch item doesnt exist, check input information\n");
        }else if(tempInt < 4 && tempInt > 0)
        {
            tempInt -= 1;
            actCateg = (enum instr)tempInt;

            while(scrollingCategory->musInstrCategory != actCateg && 
                scrollingCategory->nextCategory != NULL)
            {
                scrollingCategory = scrollingCategory->nextCategory;
            }
            actElem = scrollingCategory->dBFirstElem;

            tempInt = 0;
            while(tempInt != 3)
            {
                clearscr();
                if(actElem != NULL)
                {
                    printBasicElemInfo(actElem);
                    printf("______________________\n");
                    if(actElem->prevItem != NULL)
                        printf("0| Back\n");
                    if(actElem->nextItem != NULL)
                        printf("1| Forward\n");
                    printf("\
2| Options with item\n");
                    printf("\
3| Exit from scrolling\n");
                    printf("______________________\n");
                    scanf("%d", &tempInt);
                }else
                {
                    printf("actual category is empty\n");
                    tempInt = 3;
                }
                if(tempInt == 2)
                {
                    itemOption(dB, actElem);
                }
                else if(tempInt == 0 && actElem->prevItem != NULL)
                    actElem = actElem->prevItem;
                else if(tempInt == 1 && actElem->nextItem != NULL)
                    actElem = actElem->nextItem;
            }
            tempInt = 6;
        }
    }
}

char *FGETS(char *str, int num, FILE *stream)
{
    fgets(str, num, stream);
    int i = 0;
    char enter = '\n';
    while(i < num && str[i] != '\0' && str[i] != '\n')
        i++;
    if(str[i] == '\n')
        str[i] = '\0';
    return str;
}