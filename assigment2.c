/*-------------------------------------------------------------------
*/
/* ITS60304– Assignment #1 */
/* C Programming */
/* Student Name: <Wan Yee Xiong> <Karen Sim Tze Mien> */
/* Student ID: <0324206> <0322562> */
/*-------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 25
#define CODELENGTH 6

enum boolean { NO, YES };

FILE *gstText;
FILE *ngstText;
FILE *transactionsText;

int flush;
int gstTransactions;
int ngstTransactions;
double gstSales;	// gst not included
double ngstSales;

void purchase(void);
void showInventory(void);
void showTransactions(void);
void edit(void);
void delete(void);

void printMenu(void);
void printPurchaseMenu(void);
void printQuantityPrompt(void);
void printDeleteMenu(void);
void cont(void);

void printReceipt(char yes, char no, char itemCode[CODELENGTH], char itemName[MAXCHAR], double price, int quantity);
void replaceFile(char fileName[25], char itemCodeInput[8]);


int main(void)
{
	FILE *tempGst;
	FILE *tempNgst;
	char menuInput[MAXCHAR];
	char itemCode[CODELENGTH];
	char itemName[MAXCHAR];
	double price;
	int quantity;
	int convertedMenuInput;
	int loopMenu = YES;
	int badInput = NO;
	char a;
	char b;

	// Conforming files to standard
	if ((gstText = fopen("gst.txt", "r")) == NULL ) {
		puts("The file 'gst.txt' could not be opened");
		puts("Please contact your system administrator.");
		return;
	}
	else if ((ngstText = fopen("ngst.txt", "r")) == NULL ) {
		puts("The file 'ngst.txt' could not be opened");
		puts("Please contact your system administrator.");
		return;
	}

	fseek(gstText, -1, SEEK_END);
	fseek(ngstText, -1, SEEK_END);

	fscanf(gstText, "%c", &a);
	fscanf(ngstText, "%c", &b);

	fclose(gstText);
	fclose(ngstText);

	gstText = fopen("gst.txt", "a");
	ngstText = fopen("ngst.txt", "a");

	if (a != '\n')
		fprintf(gstText, "\n");
	if (b != '\n')
		fprintf(ngstText, "\n");

	fclose(gstText);
	fclose(ngstText);
    // end of standardizing

	system("clear");

	// Options
	while (loopMenu) {
		printMenu();

		printf("Select an option: ");
		fgets(menuInput, MAXCHAR-1, stdin);

		convertedMenuInput = atoi(menuInput);

		system("clear");

		switch (convertedMenuInput) {
			case 1:
				purchase();
				flush = getchar();
				cont();
				break;
				
			case 2:
				edit();
				cont();
				break;
				
			case 3:
				puts("This option allows user to update items.");
				cont();
				break;
				
			case 4:
				delete();
				cont();
				break;
				
			case 5:
				showInventory();
				cont();
				break;
				
			case 6:
				showTransactions();
				cont();
				break;
				
			case 7:
				loopMenu = NO;
				break;
				
			default:
				badInput = YES;
				break;
		}
		system("clear");

		if (badInput) {
			puts("Invalid input. Please enter your selection again.");
			badInput = NO;
		}
	} 
} // end of main

// Option 1: Purchase
void purchase(void)
{
	FILE *backupGst;
	FILE *backupNgst;
	char itemCodeInput[CODELENGTH];
	char itemCode[CODELENGTH];
	char itemName[MAXCHAR];
	char receiptPrompt[MAXCHAR];
	int quantityInput;
	int quantity;
	int itemFound;
	int transactions = 0;
	int isGST;
	int badInput;
	int cancelItem;
	double price;
	double subtotal;
	double gstAmount;
	double total;
	
	if ((gstText = fopen("gst.txt", "r")) == NULL ) {
		puts("The file 'gst.txt' could not be opened");
		puts("Please contact your system administrator.");
		return;
	}
	else if ((ngstText = fopen("ngst.txt", "r")) == NULL ) {
		puts("The file 'ngst.txt' could not be opened");
		puts("Please contact your system administrator.");
		return;
	}
	transactionsText = fopen("transactions.txt", "w");
	fclose(transactionsText);

	// creating backups
	fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
	while (!feof(gstText)){ //write in new file
        fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
        fprintf(backupGst, "%s;%s;%.2lf;%d\n", itemCode, itemName, price, quantity);
	}
    rewind(gstText);

    fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
    while (!feof(gstText)){ //write in new file
        fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
        fprintf(backupGst, "%s;%s;%.2lf;%d\n", itemCode, itemName, price, quantity);
	}
    rewind(gstText);

	itemFound = NO;

	printPurchaseMenu();
	printf("Enter the item code: ");
	scanf("%s", itemCodeInput);
	while(strcmp(itemCodeInput, "-1") && strcmp(itemCodeInput, "c")) {
		fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		while(!feof(gstText)) {
			fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
			if (strcmp(itemCodeInput, itemCode) == 0) {
				itemFound = YES;
				isGST = YES;
				break;
			}
		}

		if (itemFound == NO) {
			fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
			while(!feof(ngstText)) {
				fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
				if (strcmp(itemCodeInput, itemCode) == 0) {
					itemFound = YES;
					isGST = NO;
					break;
				}
			}
		}		

		// Quantity Prompt
		if (itemFound == YES) {
			system("clear");

			printQuantityPrompt();
			puts("Code       Name                     Price      Stock");
			printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantity);
			puts("");
			printf("Enter the quantity: ");

			for(;;) {

				if (scanf("%d", &quantityInput) != 1) {
					system("clear");
					puts("Invalid input, please enter a number");
				}

				else if (quantityInput > 0 && quantityInput <= quantity) {

					subtotal = price * quantityInput;
					gstAmount = subtotal * 0.06;

					// Write to file
					transactionsText = fopen("transactions.txt", "a");
					fprintf(transactionsText, "%s;%s;%.2lf;%d\n", itemCode, itemName, price, quantityInput);

					puts("");
					puts("Code       Name                     Price      Quantity");
					printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantityInput);
					puts("");
					if (isGST) {	// is the item GST taxable?
						printf("Subtotal: %.2lf (%.2lf + %.2lf GST)\n", subtotal + gstAmount, subtotal, gstAmount);
						gstTransactions += quantityInput;
						gstSales += subtotal;
						total += (subtotal + gstAmount);
					}
					else {
						printf("Subtotal: %.2lf\n", subtotal);
						ngstTransactions += quantityInput;
						ngstSales += subtotal;
						total += subtotal;
					}

					fclose(transactionsText);

					// Deduct quantity


					flush = getchar();
					cont();
					break;
				}

				else if (quantityInput > quantity) {
					system("clear");
					printf("Amount exceeded stock, please enter a number equal or less than %d\n", quantity);
				}
				
				else {
					cancelItem = YES;
					break;
				}

				flush = getchar();

				printQuantityPrompt();
				puts("Code       Name                     Price      Stock");
				printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantity);
				puts("");
				printf("Enter the quantity: ");
			}
		}
		else
			badInput = YES;

		rewind(gstText);
		rewind(ngstText);
		itemFound = NO;

		system("clear");

		if (cancelItem == YES) {
			puts("Item canceled");
			cancelItem = NO;
		}
		else if (badInput == YES) {
			puts("Invalid item.");
			badInput = NO;
		}
		
		printPurchaseMenu();

		puts("Cart");
		puts("=======");
		puts("Code       Name                     Price      Quantity");

		// Cart
		transactionsText = fopen("transactions.txt", "r");

		fscanf(transactionsText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		while(!feof(transactionsText)) {	
			subtotal = price * quantity;
			gstAmount = subtotal * 0.06;

			printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantity);
			fscanf(transactionsText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		}
		fclose(transactionsText); 
		puts("");
		printf("Total: %.2lf", total);
		puts(""); // End of Cart

		printf("Enter the item code: ");
		scanf("%s", itemCodeInput);
	}

	// Receipt
	char yesReceipt = 'y';
	char noReceipt =  'n';
	if (strcmp(itemCodeInput, "-1") && subtotal > 0) {
		printf("Print receipt? (%c/%c): ", yesReceipt, noReceipt);
		flush = getchar();
		printReceipt(yesReceipt, noReceipt, itemCode, itemName, price, quantity);
	}
	else
		puts("Transaction canceled"); // End of Receipt

	fclose(gstText);
	fclose(ngstText);

	return;
} // end of Purchase

// Option 2: Edit Items
void edit(void)
{
	typedef struct
	{
		char name[MAXCHAR];
		FILE *fp;
	} textFile;

	textFile text;

    char itemCode[8];
    char itemName[25];
    double price;
    int quantity;
    FILE *temp;
	char itemCodeInput[8];
	int itemFound;
	int Name = 1;
	int Price = 2;
	int Quantity = 3;
	int choice;
	char newItemName[30];
	double newPrice;
	int newQuantity;

	puts("------------------------------------");
	puts("Edit Items");
	puts("------------------------------------");
	puts("Enter -1 to cancel transaction");
	puts("");

	printf("Enter the item code to be edited: ");
	scanf("%s", itemCodeInput);

	if((gstText = fopen("gst.txt", "r")) == NULL){
        puts("File not found.");
        puts("Please contact your system administrator.");
        return;
	}
    else if ((ngstText = fopen("ngst.txt", "r")) == NULL){
        puts("File not found.");
        puts("Please contact your system administrator.");
        return;
    }

	while (strcmp(itemCodeInput, "-1") != 0){ //start transaction??

        itemFound = NO;

        fscanf(text.fp, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
        while (!feof(text.fp)) { // check if item match in gst.txt
            text.fp = gstText;
            fscanf(text.fp, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
            if (strcmp(itemCodeInput, itemCode) == 0) {
                itemFound = YES;
                strcpy(text.name, "gst.txt");
                break;
            }
        }
        rewind(text.fp);

        if (itemFound == NO){
        	text.fp = ngstText;
        	fscanf(text.fp, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
            while (!feof(text.fp)) { // check if item match in ngst.txt
                fscanf(text.fp, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
                if (strcmp(itemCodeInput, itemCode) == 0) {
                    itemFound = YES;
                    strcpy(text.name, "ngst.txt");
                    break;
                }
            }
            rewind(text.fp);
        }
        
        if (itemFound == NO){ //if item cant be found
            puts("Item cannot be found.");
        }

        else if (itemFound == YES){ //if item found
            printf ("Item found\n\n");
            //print out selected item
			puts("Code       Name                     Price      Stock");
			printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantity);
            puts("");

            flush = getchar();
            puts("Which would you like to edit?");
            puts("1. Name");
            puts("2. Price");
            puts("3. Quantity");
            scanf("%d", &choice);

            for(;;) {
            	if (choice == Name){ //If 1 chosen
	                printf("Change name of item.\n");
	                printf("Please enter the new name: ");
	                scanf(" %24[^\n]", newItemName); //get the new name of the item

	                newPrice = price;
	                newQuantity = quantity;
	                break;
	            }

	            else if (choice == Price){ //If 2 chosen
	                printf("Change the price of item.\n");
	                printf("Please enter the new price: ");
	                scanf(" %lf", &newPrice); //get the new price of the item

	                strcpy(newItemName, itemName);
	                newQuantity = quantity;
	                break;
	            }

	            else if (choice == Quantity){ //If 3 chosen
	                printf("Change the quantity of item.\n");
	                printf("Please enter the new quantity: ");
	                scanf(" %d", &newQuantity); //get the new quantity of the item

	                strcpy(newItemName, itemName);
	                newPrice = price;
	                break;
	            }
	            else {
	                puts("Invalid input. Please enter any of the following");
	                puts("1. Name");
	                puts("2. Price");
	                puts("3. Quantity");
	            }
            }

            temp = fopen("temp.txt", "w");
            fscanf(text.fp, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
            while (!feof(text.fp)){ //printing
	            fscanf(text.fp, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
	            if (strcmp(itemCodeInput, itemCode) == 0){ 
	                fprintf(temp, "%s;%s;%.2lf;%d\n", itemCodeInput, newItemName, newPrice, newQuantity);
	            }
	            else {
	            	fprintf(temp, "%s;%s;%.2lf;%d\n", itemCode, itemName, price, quantity); 
	            }
	        }

	        fclose(temp);
        	remove(text.name);
        	rename("temp.txt", text.name);

        } //end to if item found

        rewind(text.fp);

        printf("Enter the item code to be edited: ");
        scanf("%s", itemCodeInput);

    } //end to while loop

    fclose(gstText);
    fclose(ngstText);

    puts("Exited.");

}

// Option 4: Delete Items
void delete(void)
{
    char itemCodeInput[8];
    char itemCode[8];
    char itemName[25];
    double price;
    int quantity;
    int gst;
    int itemFound;
    int badInput;
    int cancelDelete;
    int stockNotEmpty;
 	int itemDeleted;
    char ch; //for y/n
    char flush;

	printDeleteMenu();

	printf("Enter item code to be deleted: ");
	scanf("%s", itemCodeInput);

	while (strcmp(itemCodeInput, "-1") != 0) { //compare input and code
        
        //open the files
        if ((gstText = fopen("gst.txt", "r")) == NULL ) {
            puts("The file 'gst.txt' could not be opened");
            puts("Please contact your system administrator.");
            return;
        }
        else if ((ngstText = fopen("ngst.txt", "r")) == NULL ) {
            puts("The file 'ngst.txt' could not be opened");
            puts("Please contact your system administrator.");
            return;
        }

        itemFound = NO;
        gst = NO;

        fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		while(!feof(gstText)) { // check if item code matches in gst.txt
			fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
			if (strcmp(itemCodeInput, itemCode) == 0) {
				itemFound = 1;
                gst = 1;
				break;
			}
		} 

        if (itemFound == NO) { //open ngst file

        	fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
            while (!feof(ngstText)) { // check if item code matches in ngst.txt
                fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
                if (strcmp(itemCodeInput, itemCode) == 0) {
                    itemFound = 1;
                    break;
                }
            }
        }
        fclose(gstText);
        fclose(ngstText);


        if (itemFound == YES) { //if matches gst

            printf("Item found.\n\n");
			puts("Code       Name                     Price      Stock");
			printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantity);

            flush = getchar();
            printf("Confirm to delete item? (y/n)\n");
            for(;;) {
	        	scanf(" %c", &ch);

	            if (ch == 'y'){ //if y to delete

	                if (quantity != 0) { //say no if quantity not zero
	                    stockNotEmpty = YES;
	                }
	                else { //if quantity is zero
	                    if (gst) {
	                        replaceFile("gst.txt\0", itemCodeInput);
	                    }
	                    else {
	                        replaceFile("ngst.txt\0", itemCodeInput);
	                    }
	                    itemDeleted = YES;
	                }
	                break;      

	            } //end if y to delete

	            else if (ch == 'n'){ //if n to delete, cancel transaction
	                cancelDelete = YES;
	                break;
	            }
	            else {
	            	puts("invalid input, please enter y or n");
	            }

            }

        } //end item found
        else {
        	badInput = YES;
        }

        system("clear");

        if (itemDeleted == YES) {
        	puts("Item deleted.");
        }
		else if (cancelDelete == YES) {
			puts("Deletion canceled.");
			cancelDelete = NO;
		}
		else if (badInput == YES) {
			puts("Invalid item.");
			badInput = NO;
		}
		else if (stockNotEmpty == YES) {
			puts("Stock is not zero. Item cannot be deleted.");
		}
		
		puts("");
		printDeleteMenu();

		printf("\nEnter item code to be deleted: ");
        scanf("%s", itemCodeInput);

    } // end of while(strcmp)
} // end of delete


// Option 5: Show Inventory
void showInventory(void)
{  
	char itemCode[6];
	char itemName[20];
	double price;
	int quantity;

    puts("| GST included Items |");
    if ((gstText = fopen("gst.txt", "r")) == NULL ) {
		puts("The file 'gst.txt' could not be opened");
		puts("Please contact your system administrator.");
	}
	else {
		puts("Code       Name                     Price      Quantity\n");

		fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		while (!feof(gstText)){
			printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantity);
			fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		}

		fclose(gstText);
	}

	puts("");
	puts("| Non-GST included Items |");
    if ((ngstText = fopen("ngst.txt", "r")) ==NULL ) {
		puts("The file 'ngst.txt' could not be opened");
		puts("Please contact your system administrator.");
	}
	else {
		puts("Code       Name                     Price      Quantity\n");

		fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		while (!feof(ngstText)){
			printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantity);
			fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		}

		fclose(ngstText);
	}
	return;
} // end of showInventory

void showTransactions(void)
{
	puts("------------------------------------");
	puts("Transaction Details");
	puts("------------------------------------");
	puts("");
	printf("Today's transactions:    %d\n", gstTransactions + ngstTransactions);
	printf("Sales with GST:          %.2lf\n", gstSales * 1.06);
	printf("Sales without GST:       %.2lf\n", ngstSales);
	printf("Total sales:             %.2lf\n", gstSales*1.06 + ngstSales);
	printf("GST collected:           %.2lf\n", gstSales * 0.06);

	return;
}

void printMenu(void)
{
	puts("------------------------------------");
	puts("Grocery Retail");
	puts("------------------------------------");
	puts("1. Purchase items");
	puts("2. Edit items");
	puts("3. Update items");
	puts("4. Delete items");
	puts("5. Show inventory");
	puts("6. Show daily transaction");
	puts("7. Exit");
	puts("");
	puts("------------------------------------");
	puts("");

	return;
}

void printPurchaseMenu(void)
{
	puts("------------------------------------");
	puts("Purchase");
	puts("------------------------------------");
	puts("Enter -1 to cancel transaction");
	puts("Enter c to conclude transaction");
	puts("");

	return;
}

void printQuantityPrompt(void)
{
	puts("------------------------------------");
	puts("Purchase");
	puts("------------------------------------");
	puts("Quantities less than 1 will cancel the selected item");
	puts("");

	return;
}

void printDeleteMenu(void)
{
	puts("------------------------------------");
	puts("Delete Items");
	puts("------------------------------------");
	puts("Enter -1 to exit");
	puts("");

	return;
}

void cont(void)
{
	int c;

	puts("");
	puts("Press enter to continue");
	while ((c = getchar()) != '\n');

	return;
}

void printReceipt(char yes, char no, char itemCode[CODELENGTH], char itemName[MAXCHAR], double price, int quantity)
{
	int c;
	double gstAmount;
	double subtotal;
	double total;
	double roundedTotal;

	system("clear");

	while (c = getchar()) {
		if (c == yes) {

			transactionsText = fopen("transactions.txt", "r");

			system("clear");

			puts("======== Receipt ========");
			puts("Code       Name                     Price      Quantity");
			puts("");

			fscanf(transactionsText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
			while(!feof(transactionsText)) {	
				subtotal = price * quantity;
				gstAmount = subtotal * 0.06;

				printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantity);
				if (itemCode[1] == 'G') {
					total += (subtotal + gstAmount);
					printf("Subtotal: %.2lf (%.2lf + %.2lf GST)\n", subtotal + gstAmount, subtotal, gstAmount);
				}
				else {
					total += subtotal;
					printf("Subtotal: %.2lf\n", subtotal);
				}
				puts("");

				fscanf(transactionsText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
			}
			printf("Total Sales incl GST: %.2lf\n", total);
			roundedTotal = round(total * 20.0) / 20.0; // rounds prices to 0.05
			printf("Rounding adjusment: %.2lf\n", roundedTotal - total);
			printf("Total after adj incl GST: %.2lf\n", roundedTotal);
			fclose(transactionsText);
			break;
		}
		else if (c == no) {
			puts("Transaction concluded");
			break;
		}
		else
			if (c != '\n')
				flush = getchar();
			printf("Invalid input, please enter y or n: ");
	}
} // end of printReceipt

void replaceFile(char fileName[25], char itemCodeInput[8]) {
    FILE *temp;
    FILE *file;
    char itemCode[8];
    char itemName[25];
    double price;
    int quantity;

    if ((file = fopen(fileName, "r")) != NULL) {
        temp = fopen("temp.txt", "w"); //open new file to write

        fscanf(file, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
        while (!feof(file)){ //write in new file
            if (strcmp(itemCodeInput, itemCode) != 0) {
                fprintf(temp, "%s;%s;%.2lf;%d\n", itemCode, itemName, price, quantity);
            }
            fscanf(file, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
        }

        fclose(temp);
        fclose(file);
        remove(fileName);
        rename("temp.txt", fileName);
    } 
    else {
        puts("The file could no be found");
    }
    
    return;
} // end of replaceFile