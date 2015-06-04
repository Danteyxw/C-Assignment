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
void cont(void);
void printReceipt(char yes, char no, char itemCode[CODELENGTH], char itemName[MAXCHAR], double price, int quantity);

int main(void)
{
	int convertedMenuInput;
	int loopMenu = YES;
	int badInput = NO;
	char menuInput[MAXCHAR];

	// Options
	while (loopMenu) {
		printMenu();

		printf("Select an option: ");
		fgets(menuInput, MAXCHAR-1, stdin);

		convertedMenuInput = atoi(menuInput);

		switch (convertedMenuInput) {
			case 1:
				system("clear");
				purchase();
				flush = getchar();
				cont();
				puts("");
				system("clear");
				break;
				
			case 2:
				system("clear");
				puts("This option allows user to edit items.");
				puts("");
				break;
				
			case 3:
				system("clear");
				puts("This option allows user to update items.");
				puts("");
				break;
				
			case 4:
				system("clear");
				puts("This option allows user to delete items.");
				puts("");
				break;
				
			case 5:
				system("clear");
                showInventory();
                puts("");
				break;
				
			case 6:
				system("clear");
				showTransactions();
				puts("");
				break;
				
			case 7:
				system("clear");
				puts("Program exiting.");
				loopMenu = NO;
				break;
				
			default:
				system("clear");
				puts("Invalid input. Please enter your selection again.");
				badInput = YES;
				break;
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
	int c;
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
	else if ((transactionsText = fopen("transactions.txt", "w")) == NULL) {
		puts("The file 'transactions.txt' could not be opened");
		puts("Please contact your system administrator.");
		return;
	}

	itemFound = NO;

	puts("------------------------------------");
	puts("Purchase");
	puts("------------------------------------");
	puts("Enter -1 to cancel transaction");
	puts("Enter c to conclude transaction");
	puts("");

	printf("Enter the item code: ");
	scanf("%s", itemCodeInput);
	while(strcmp(itemCodeInput, "-1") && strcmp(itemCodeInput, "c")) {
		do { // R*R
			fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
			if (strcmp(itemCodeInput, itemCode) == 0) {
				itemFound = YES;
				isGST = YES;
				break;
			}
		} while(!feof(gstText));

		if (!itemFound) {
			do {
				fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
				if (strcmp(itemCodeInput, itemCode) == 0) {
					itemFound = YES;
					isGST = NO;
					break;
				}
			} while(!feof(ngstText));
		}		

		if (itemFound) {
			puts("");
			puts("Quantities less than 1 will cancel the selected item");
			printf("Enter the quantity: ");
			if ((scanf("%d", &quantityInput)) == 1 && quantityInput > 0) {

				subtotal = price * quantityInput;
				gstAmount = subtotal * 0.06;

				// Write to file
				fprintf(transactionsText, "%s;%s;%.2lf;%d\n", itemCode, itemName, price, quantityInput);

				puts("");
				puts("Code       Name                     Price      Quantity\n");
				printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantityInput);
				puts("");
				if (isGST) {	// is the item GST taxable?
					printf("Subtotal: %.2lf (%.2lf + %.2lf GST)\n", subtotal + gstAmount, subtotal, gstAmount);
					gstTransactions += quantityInput;
					gstSales += subtotal;
				}
				else {
					printf("Subtotal: %.2lf\n", subtotal);
					ngstTransactions += quantityInput;
					ngstSales += subtotal;
				}
				puts("");
			}
			else
				puts("Item canceled");
		}
		else
			puts("Invalid item.");

		rewind(gstText);
		rewind(ngstText);
		itemFound = NO;
		printf("Enter the item code: ");
		scanf("%s", itemCodeInput);
	}

	fclose(transactionsText);

	// Receipt
	char yesReceipt = 'y';
	char noReceipt =  'n';
	if (strcmp(itemCodeInput, "-1") && subtotal > 0) {
		printf("Print receipt? (%c/%c): ", yesReceipt, noReceipt);
		flush = getchar();
		printReceipt(yesReceipt, noReceipt, itemCode, itemName, price, quantity);
	}
	else
		puts("Transaction canceled");

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
	printf("Sales with GST:          %.2lf\n", gstSales);
	printf("Sales without GST:       %.2lf\n", ngstSales);
	printf("Total sales:             %.2lf\n", gstSales + ngstSales);
	printf("GST collected:           %.2lf\n", gstSales * 0.06);

	return;
}

void printMenu(void)
{
	puts("");
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

void cont(void)
{
	int c;

	puts("");
	puts("Press enter to continue");
	c = getchar();

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
			printf("Invalid input, please enter y or n: ");
	}
}