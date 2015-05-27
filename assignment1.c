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

int gstTransactions;
int ngstTransactions;
double gstSales;	// gst not included
double ngstSales;

void purchase(void);
void showInventory(void);
void showTransactions(void);

int main(void)
{
	int convertedMenuInput;
	int loopMenu = YES;
	int badInput = NO;
	char menuInput[MAXCHAR];

	// Menu
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

	// Options
	while (loopMenu) {
		printf("Select an option: ");
		scanf("%s", menuInput);
		convertedMenuInput = atoi(menuInput);

		switch (convertedMenuInput) {
			case 1:
				purchase();
				break;
				
			case 2:
				puts("This option allows user to edit items.");
				break;
				
			case 3:
				puts("This option allows user to update items.");
				break;
				
			case 4:
				puts("This option allows user to delete items.");
				break;
				
			case 5:
                showInventory();
				break;
				
			case 6:
				showTransactions();
				break;
				
			case 7:
				puts("Program exiting.");
				loopMenu = NO;
				break;
				
			default:
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
	char receiptPrompt;
	int quantityInput;
	int quantity;
	int itemFound;
	int transactions = 0;
	int isGST;
	double price;
	double subtotal;
	double gstAmount;
	double total;
	double roundedTotal;
	
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

		fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		while(!feof(gstText)) {
			if (strcmp(itemCodeInput, itemCode) == 0) {
				itemFound = YES;
				isGST = YES;
				break;
			}
			fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		}

		fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		while(!feof(ngstText) && !itemFound) {
			if (strcmp(itemCodeInput, itemCode) == 0) {
				itemFound = YES;
				isGST = NO;
				break;
			}
			fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
		}

		if (itemFound) {
			puts("");
			puts("Quantities less than 1 will cancel the selected item");
			printf("Enter the quantity: ");
			scanf("%d", &quantityInput);
			if (quantityInput > 0) {

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
	if (strcmp(itemCodeInput, "-1") && subtotal > 0) {
		printf("Print receipt? (y/n): ");
		for(;;) {
			receiptPrompt = getchar();
			if (receiptPrompt == 'y') {

				transactionsText = fopen("transactions.txt", "r");

				puts("======== Receipt ========");
				puts("Code       Name                     Price      Quantity");
				puts("");
				while(!feof(transactionsText)) {
					fscanf(transactionsText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
					
					subtotal = price * quantityInput;
					gstAmount = subtotal * 0.06;

					printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantityInput);
					if (itemCode[1] == 'G') {
						total += (subtotal + gstAmount);
						printf("Subtotal: %.2lf (%.2lf + %.2lf GST)\n", subtotal + gstAmount, subtotal, gstAmount);
					}
					else {
						total += subtotal;
						printf("Subtotal: %.2lf\n", subtotal);
					}
					puts("");
				}
				printf("Total Sales incl GST: %.2lf\n", total);
				roundedTotal = round(total * 20.0) / 20.0; // rounds prices to 0.05
				printf("Rounding adjusment: %.2lf\n", roundedTotal - total);
				printf("Total after adj incl GST: %.2lf\n", roundedTotal);
				fclose(transactionsText);
				break;
			}
			else if (receiptPrompt == 'n') {
				puts("Transaction concluded");
				break;
			}
			else
				printf("Invalid input, please enter y or n: ");
		}
	}
	else
		puts("Transaction canceled");

	puts("");

	fclose(gstText);
	fclose(ngstText);

	return;
}

void showInventory(void)
{  
	char itemCode[6];
	char itemName[20];
	double itemPrice;
	int quantity;

    puts("GST included Items");
    if ((gstText = fopen("gst.txt", "r")) == NULL ) {
		puts("The file 'gst.txt' could not be opened");
		puts("Please contact your system administrator.");
	}
	else {
		printf("%s \t %s \t %s \t %s \n", "Item Code", "Item Name", "Item Price", "Quantity");

		fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &itemPrice, &quantity);
		while (!feof(gstText)){
			printf("%s \t %s \t %.2lf \t %d \n", itemCode, itemName, itemPrice, quantity);
			fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &itemPrice, &quantity);
		}

		fclose(gstText);
	}

	puts("Non-GST included Items");
    if ((ngstText = fopen("ngst.txt", "r")) ==NULL ) {
		puts("The file 'ngst.txt' could not be opened");
		puts("Please contact your system administrator.");
	}
	else {
		printf("%s \t %s \t %s \t %s\n", "Item Code", "Item Name", "Item Price", "Quantity");

		fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &itemPrice, &quantity);
		while (!feof(ngstText)){
	    	printf("%s \t %s \t %.2lf \t %d \n", itemCode, itemName, itemPrice, quantity);
	    	fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &itemPrice, &quantity);
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
	printf("Today's transactions:    %d", gstTransactions + ngstTransactions);
	printf("Sales with GST:          %.2lf", gstSales);
	printf("Sales without GST:       %.2lf", ngstSales);
	printf("Total sales:             %.2lf", gstSales + ngstSales);
	printf("GST collected:           %.2lf", gstSales * 0.06);
}