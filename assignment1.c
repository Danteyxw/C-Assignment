/*-------------------------------------------------------------------
*/
/* ITS60304– Assignment #1 */
/* C Programming */
/* Student Name: <Wan Yee Xiong> <Karen Sim Tze Mien> */
/* Student ID: <0324206> <0322562> */
/*-------------------------------------------------------------------
*/

#include <stdio.h>
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

void printMenu(void);
void printPurchaseMenu(void);
void printQuantityPrompt(void);
void cont(void);
void printReceipt(char yes, char no, char itemCode[CODELENGTH], char itemName[MAXCHAR], double price, int quantity);

int main(void)
{
	int convertedMenuInput;
	int loopMenu = YES;
	int badInput = NO;
	char menuInput[MAXCHAR];

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
				puts("This option allows user to edit items.");
				cont();
				break;
				
			case 3:
				puts("This option allows user to update items.");
				cont();
				break;
				
			case 4:
				puts("This option allows user to delete items.");
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
			
	return 0;
}

void purchase(void)
{
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

	itemFound = NO;

	printPurchaseMenu();
	printf("Enter the item code: ");
	scanf("%s", itemCodeInput);
	while(strcmp(itemCodeInput, "-1") && strcmp(itemCodeInput, "c")) {
		do {
			fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
			if (strcmp(itemCodeInput, itemCode) == 0) {
				itemFound = YES;
				isGST = YES;
				break;
			}
		} while(!feof(gstText));

		if (itemFound == NO) {
			do {
				fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
				if (strcmp(itemCodeInput, itemCode) == 0) {
					itemFound = YES;
					isGST = NO;
					break;
				}
			} while(!feof(ngstText));
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
}

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
}

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
}