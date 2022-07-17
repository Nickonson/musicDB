#define unsInt unsigned int

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

/*

    - item option (need to be fixed)
    - get info is useless

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
    unsInt strings : 5;
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

void createNewElement(struct dBase *dB);

struct dBaseElement *getInfo(enum instr instr_categ);

void readFromFile(struct dBase *dB, FILE *file);

struct foundElemData *findElem(struct dBCategory *givenCateg, struct dBaseElement *potElement);

void withdrawElements(struct dBase *dB, struct dBaseElement *elemToWithdrow, int numbToWithdraw);

void delElem(struct dBase *dB, struct dBaseElement *elemToDel);

enum comparing dBaseElemsCmp(struct dBaseElement *first, struct dBaseElement *second);

void printAdvElemInfo(struct dBaseElement *element);

void printBasicElemInfo(struct dBaseElement *element);

void printCategInfo(struct dBCategory *category);

void delElem(struct dBase *dB, struct dBaseElement *elemToDel);

void itemOption(struct dBase *dB, struct dBaseElement *actElem, struct foundElemData *isElementInCateg);

void addItem(struct dBase *dB,  struct foundElemData *isElementInCateg, dBaseElement *newElem);

void addItemAfter(struct foundElemData *isElementInCateg, dBaseElement *newElem);

void addItemBefore(struct foundElemData *isElementInCateg, dBaseElement *newElem);

void addFirstItem(struct dBCategory *Category, dBaseElement *newElem);

void scrollDB(struct dBase *dB);

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
            scrollDB(MuzShop);
        else if(tempInt == 1 && MuzShop == NULL)
        {
            MuzShop = dBAlloc();
            initDataBase(MuzShop);
        }
        else if(tempInt == 2 && MuzShop != NULL)
            delDataBase(MuzShop);
        else if(tempInt == 3 && MuzShop != NULL)
        {
            FILE *fl;
            fl = fopen("input.txt", "r");
            readFromFile(MuzShop, fl);
            fclose(fl);
        }
    }
    
    
    return 0;
}

void clearscr ( void )
{
    printf("\033[2J\033[1;1H");
}

struct dBaseElement *musElemAlloc(void)
{
    return (struct dBaseElement *)malloc(sizeof(struct dBaseElement));
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

    printf("\nDB initialized\n");
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
    printf("\nDB is deleted successfuly\n");
}

void createNewElement(struct dBase *dB)
{
    struct dBCategory *dBCategory = dB->firstCateg;
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
    while(dBCategory->musInstrCategory != instr_categ && dBCategory->nextCategory != NULL)
    {
        dBCategory = dBCategory->nextCategory;
    }
    if(dBCategory->musInstrCategory != instr_categ)
        printf("!!! u have problems with categs 298 line !!!");

    struct dBaseElement *newElem = getInfo(instr_categ);
    struct foundElemData *isElementInCateg = findElem(dBCategory, newElem);

    addItem(dB, isElementInCateg, newElem);
}

struct dBaseElement *getInfo(enum instr instr_categ)
{
    struct dBaseElement *newElem = musElemAlloc();
    
    char usrAns = 'n';
    newElem->aboutElem.instrType = instr_categ;

    clearscr();
    gets(newElem->aboutElem.company_name);
    printf("item company name : ");
    gets(newElem->aboutElem.company_name);

    printf("\nitem model company name : ");
    gets(newElem->aboutElem.model_name);
    
    printf("\nweight of the item (kg) : ");
    cin >> newElem->aboutElem.weight;
    
    printf("\nwas this item in use before (y/n) : ");
    scanf("%c", &usrAns);
    if(usrAns == 'y')
        newElem->aboutElem.wasInUse = true;
    printf("\n\n");

    unsInt tempInt; // for dereference || reduction to bit type int
    if(instr_categ == guitar)
    {
        printf("\n\nhow many frets it has : ");
        scanf("%d", &tempInt);
        newElem->aboutElem.adv.guiInfo.frets = tempInt;

        printf("\nis it left or right handed \n\
1) Left handed\n\
2) Right handed\n");
        scanf("%d", &tempInt);
        tempInt -= 1;
        newElem->aboutElem.adv.guiInfo.hand = (enum handOrientation)tempInt;
        
        printf("\ntype of the neck \n\
1) BOLT_ON\n\
2) SET_NECK\n\
3) NECK_TROUGH\n");
        scanf("%d", &tempInt);
        tempInt -= 1;
        newElem->aboutElem.adv.guiInfo.neck = (enum neck)tempInt;

        printf("number of strings : ");
        scanf("%d", &tempInt);
        newElem->aboutElem.adv.guiInfo.strings = tempInt;
    }
    else if(instr_categ == bass_guitar)
    {
        printf("\nbass type\n");
        printf("1) PBASS\n\
2) JBASS\n\
3) HALF_ACOUSTIC\n\
4) NO_FRETS");
        scanf("%d", &tempInt);
        tempInt -= 1;
        newElem->aboutElem.adv.bassInfo.typeOfBass = (enum bassType)tempInt;
        
        printf("strings number : ");
        scanf("%d", &tempInt);
        newElem->aboutElem.adv.bassInfo.strings = tempInt;

        printf("\nnecktype\n\
1) Bolt On\n\
2) Set neck\n\
3) Neck through\n\
_________________\n");
        scanf("%d", &tempInt);
        tempInt -= 1;
        newElem->aboutElem.adv.bassInfo.neck = (enum neck)tempInt;

        printf("\nfrets number : ");
        scanf("%d", &tempInt);
        newElem->aboutElem.adv.bassInfo.frets = tempInt;

        printf("\nis it left or right handed \n\
1) Left handed\n\
2) Right handed\n");
        scanf("%d", &tempInt);
        tempInt -= 1;
        newElem->aboutElem.adv.bassInfo.hand = (enum handOrientation)tempInt;
        
    }
    else if(instr_categ == synth)
    {
        printf("\ndoes it has hammer imitating mechanism\n\
1) No\n\
2) Yes");
        scanf("%d", &tempInt);
        tempInt -= 1;
        newElem->aboutElem.adv.synInfo.imitHamMech = (enum logic)tempInt;

        printf("\nhow many keys : ");
        scanf("%d", &tempInt);
        newElem->aboutElem.adv.synInfo.keys = tempInt;

        printf("\ndoes it has usb?\n\
1) Yes\n\
2) No");
        scanf("%d", &tempInt);
        newElem->aboutElem.adv.synInfo.usb = (enum logic)tempInt;

        printf("\ndoes it has modualtion wheel?\n\
1) Yes\n\
2) No");
        scanf("%d", &tempInt);
        newElem->aboutElem.adv.synInfo.modulWheel = (enum logic)tempInt;
        
        printf("\nhow many analog output instrument has : ");
        scanf("%d", &tempInt);
        newElem->aboutElem.adv.synInfo.analOutput = tempInt;
    }
    newElem->nextItem = NULL;
    newElem->prevItem = NULL;
    return newElem;
}

void readFromFile(struct dBase *dB, FILE *file)
{
    if (feof(file))
		fseek(file, 0, SEEK_SET);
	char str[256];
    while (!feof(file))
	{
        struct dBaseElement *newElem = musElemAlloc();
        int temp;

        fgets(str, 255, file);
		while (str[0] == '\n')
		{
			fgets(str, 255, file);
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
        fgets(str, 30, file);
        strcpy(newElem->aboutElem.company_name, str);

        //company mail
        fgets(str, 30, file);
        strcpy(newElem->aboutElem.company_mail, str);

        //item model name
        fgets(str, 30, file);
        strcpy(newElem->aboutElem.model_name, str);

        fgets(str, 30, file);
        if(str[0] == 'y')
            newElem->aboutElem.wasInUse = true;
        else
            newElem->aboutElem.wasInUse = false;
        
        //weight
        //fgets(str, 30, file);
        fscanf(file, "%s%lf", str, &(newElem->aboutElem.weight));

        //number of item
        fscanf(file, "%s%d", str, &(newElem->aboutElem.itemNumb));        
        
        //price
        fscanf(file, "%s%d", str, &(newElem->aboutElem.price));        
        
        //year of produce
        newElem->aboutElem.yearProduce = 0;
        fscanf(file, "%s%d", str, &(temp));        
        newElem->aboutElem.yearProduce = temp;

        if(newElem->aboutElem.instrType == guitar)
        {
            fscanf(file, "%s%d", str, &(temp));
            newElem->aboutElem.adv.guiInfo.frets = temp;

            fgets(str, 30, file);
            if(strcmp(str, "LH") == 0 || strcmp(str, "lh") == 0)
                newElem->aboutElem.adv.guiInfo.hand = LH;
            else
                newElem->aboutElem.adv.guiInfo.hand = RH;  
            
            fgets(str, 30, file);
            if(strcmp(str, "BO") == 0 || strcmp(str, "bo") == 0)
                newElem->aboutElem.adv.guiInfo.neck = BOLT_ON;
            else if(strcmp(str, "SN") == 0 || strcmp(str, "sn") == 0)
                newElem->aboutElem.adv.guiInfo.neck = SET_NECK;
            else if(strcmp(str, "NT") == 0 || strcmp(str, "nt") == 0)
                newElem->aboutElem.adv.guiInfo.neck = NECK_TROUGH;
            else
                printf("problem with neck definition, 486 line");
            
            fscanf(file, "%s%d", str, &(temp));
            newElem->aboutElem.adv.guiInfo.strings = temp;
        }
        else if(newElem->aboutElem.instrType == bass_guitar)
        {
            fscanf(file, "%s%d", str, &(temp));
            newElem->aboutElem.adv.bassInfo.frets = temp;

            fgets(str, 30, file);
            if(strcmp(str, "precision") == 0)
                newElem->aboutElem.adv.bassInfo.typeOfBass = PRECISION;
            else if(strcmp(str, "jazz") == 0)
                newElem->aboutElem.adv.bassInfo.typeOfBass = JAZZ;
            else if(strcmp(str, "half acoustic") == 0)
                newElem->aboutElem.adv.bassInfo.typeOfBass = HALF_ACOUSTIC;
            else if(strcmp(str, "nofrets") == 0 || strcmp(str, "no frets") == 0)
                newElem->aboutElem.adv.bassInfo.typeOfBass = NO_FRETS;
            
            fscanf(file, "%s%d", str, &(temp));
            newElem->aboutElem.adv.bassInfo.strings = temp;
        }
        else if(newElem->aboutElem.instrType == synth)
        {
            fgets(str, 30, file);
            if(str[0] == 'y')
                newElem->aboutElem.adv.synInfo.imitHamMech = YES;
            else
                newElem->aboutElem.adv.synInfo.imitHamMech = NO;
            
            fscanf(file, "%s%d", str, &(temp));
            newElem->aboutElem.adv.synInfo.keys = temp;

            fgets(str, 30, file);
            if(str[0] == 'y')
                newElem->aboutElem.adv.synInfo.usb = YES;
            else
                newElem->aboutElem.adv.synInfo.usb = NO;
            
            fgets(str, 30, file);
            if(str[0] == 'y')
                newElem->aboutElem.adv.synInfo.modulWheel = YES;
            else
                newElem->aboutElem.adv.synInfo.modulWheel = NO;
            
            fscanf(file, "%s%d", str, &(temp));
            newElem->aboutElem.adv.synInfo.analOutput = temp;
        }
        
    }
}

struct foundElemData *findElem(struct dBCategory *givenCateg, struct dBaseElement *potElement)
{
    struct foundElemData *output = (struct foundElemData *)malloc(sizeof(struct foundElemData));
    enum comparing tempStatus = DIFFERENT;
    //temp elem to save data about similar item 
    struct dBaseElement *foundElem = NULL;
    struct dBaseElement *tempElement = givenCateg->dBFirstElem;
    
    //checking first element of a category
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
    return output;
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

    if(strcmp(first->aboutElem.company_name, second->aboutElem.company_name) &&
        strcmp(first->aboutElem.model_name, second->aboutElem.model_name) &&
        first->aboutElem.instrType == second->aboutElem.instrType)
        {
            if(first->aboutElem.weight == second->aboutElem.weight &&
                first->aboutElem.yearProduce == second->aboutElem.yearProduce && 
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
    printf("Company name : %s\n", element->aboutElem.company_name);
    printf("Model name   : %s\n", element->aboutElem.model_name);
    printf("instrument type : ");
    if(element->aboutElem.instrType == guitar)
        printf("guitar\n");
    else if(element->aboutElem.instrType == bass_guitar)
        printf("bass guitar\n");
    else if(element->aboutElem.instrType == synth)
        printf("synthesizer\n");
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

void itemOption(struct dBase *dB, struct dBaseElement *actElem, struct foundElemData *isElementInCateg)
{
    int tempInt = 0;
    while(tempInt != 5){
        clearscr();
        printBasicElemInfo(actElem);
        if(isElementInCateg == NULL || isElementInCateg->isSame == SAME)
        {
            printf("1| Stock item\n");
            if(actElem->aboutElem.itemNumb > 0)
                printf("2| Withdraw item\n");
            printf("3| Delete item from base\n");
        }
        else if(isElementInCateg != NULL && isElementInCateg->isSame == SIMILAR)
        {
            printf("1| This element is the same, change the status to SAME\n");
            printf("2| Add as new element\n");
        }
        printf("4| Show advance info about item\n\
5| Back to scroll\n");
        scanf("%d", &tempInt);

        if(tempInt == 1)
        {
            if(isElementInCateg == NULL || isElementInCateg->isSame == SAME)
            {
                printf("how manny do u wanna stock?\n");
                scanf("%d", &tempInt);
                actElem->aboutElem.itemNumb += tempInt;
            }
            else if(isElementInCateg != NULL && isElementInCateg->isSame == SIMILAR)
            {
                isElementInCateg->isSame = SAME;
            }
        }
        else if(tempInt == 2)
        {
            if(isElementInCateg->isSame == SAME && actElem->aboutElem.itemNumb > 0)
            {
                clearscr();
                printBasicElemInfo(actElem);
                printf("we have %d\n", actElem->aboutElem.itemNumb);
                printf("how many do u wanna withdraw : ");
                scanf("%d", &tempInt);
                if(tempInt > actElem->aboutElem.itemNumb)
                    printf("\nwe dont have such number of instrument");
            }
            else if(isElementInCateg != NULL && isElementInCateg->isSame == SIMILAR)
            {
                addItem(dB, isElementInCateg, actElem);
            }
        }
        else if(tempInt == 3 && (isElementInCateg == NULL || isElementInCateg->isSame == SAME))
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

void addItem(struct dBase *dB, struct foundElemData *isElementInCateg, dBaseElement *newElem)
{
    struct dBCategory *dBCategory = dB->firstCateg;
    while(dBCategory != NULL && dBCategory->musInstrCategory != newElem->aboutElem.instrType)
    {
        dBCategory = dBCategory->nextCategory;
    }
    char userAnswer = 'n';
    int temp;
    if(isElementInCateg->isSame == SAME)
    {

        printf("\nsuch element s already exist, wanna store? (y/n)\n");
        scanf("%c", &userAnswer);
        if(userAnswer == 'Y' || userAnswer == 'y')
        {
            printf("\nhow many? (number)\n");
            scanf("%d", &temp);
            isElementInCateg->found_element->aboutElem.itemNumb += temp;
        }
    }
    else if(isElementInCateg->isSame == SIMILAR)
    {
        printf("\nwe have similar item to this, wanna add? It will be stored close to the found one (y/n)\n");
        scanf("%c", &userAnswer);
        if(userAnswer == 'Y' || userAnswer == 'y')
        {
            if(isElementInCateg->found_element->aboutElem.wasInUse == 1)
            {
                //wasInUse == 1 znachit stavim posle foundElem
                addItemAfter(isElementInCateg, newElem);
            }
            else
            {
                //wasInUse == 0 znachit do foundElem
                addItemBefore(isElementInCateg, newElem);
            }
        }
    }
    else
    {
        printf("\nwe dont have such item in our base, wanna add elem? (y/n)\n");
        scanf("%c", &userAnswer);
        if(userAnswer == 'Y' || userAnswer == 'y')
        {
            //no elements in categ
            addFirstItem(dBCategory, newElem);
        }
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
    unsInt tempInt = 5;
    struct dBCategory *scrollingCategory = dB->firstCateg;
    struct dBaseElement *actElem;
    
    while(tempInt != 6)
    {
        while(tempInt > 6)
        {
            clearscr();
            printf("items from which category wanna see?\n\
1| guitars\n\
2| basses\n\
3| synths\n\
4| find element\n\
5| create new element\n\
6| exit");
            scanf("%d", &tempInt);
        }
        if(tempInt == 6)
            break;
        else if(tempInt == 5)
        {
            createNewElement(dB);
        }else if(tempInt == 4)
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
            
            struct dBaseElement *elemToFind = getInfo(instr_categ);
            struct foundElemData *isElementInCateg = findElem(dBCategory, elemToFind);
            if(isElementInCateg->found_element != NULL)
            {
                itemOption(dB, actElem, isElementInCateg);
            }
        }else if(tempInt < 4 && tempInt > 0)
        {
            tempInt -= 1;
            actCateg = (enum instr)tempInt;

            while(scrollingCategory->musInstrCategory != actCateg && 
                scrollingCategory != NULL)
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
                    printf("actual category is empty, wanna add item? (y/n)");
                    scanf("%c", &usrAnswr);
                    if(usrAnswr == 'y')
                    {
                        createNewElement(dB);
                    }
                }
                if(tempInt == 2)
                {
                    itemOption(dB, actElem, NULL);
                }
                else if(tempInt == 0 && actElem->prevItem != NULL)
                    actElem = actElem->prevItem;
                else if(tempInt == 1 && actElem->nextItem != NULL)
                    actElem = actElem->nextItem;
            }
        }
    }
}