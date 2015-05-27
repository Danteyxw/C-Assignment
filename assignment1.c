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
int flush;

void purchase(void);

void showInventory(void);

int main(void)
{
	int convertedMenuInput;
	int loopMenu = YES;
	int badInput = NO;
	char menuInput[MAXCHAR];

	// Menu
		printf("------------------------------------\n");
		printf("Grocery Retail\n");
		printf("------------------------------------\n");
		printf("1. Purchase items\n");
		printf("2. Edit items\n");
		printf("3. Update items\n");
		printf("4. Delete items\n");
		printf("5. Show inventory\n");
		printf("6. Show daily transaction\n");
		printf("7. Exit\n");
		printf("\n");

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
				printf("This option allows user to edit items.\n");
				break;
				
			case 3:
				printf("This option allows user to update items.\n");
				break;
				
			case 4:
				printf("This option allows user to delete items.\n");
				break;
				
			case 5:
                showInventory();
				break;
				
			case 6:
				printf("stub here\n");
				break;
				
			case 7:
				printf("Program exiting.\n");
				loopMenu = NO;
				break;
				
			default:
				printf("Invalid input. Please enter your selection again.\n");
				badInput = YES;
				break;
		}
	}
			
	return 0;
}

void showInventory (void)
{  
	char itemCode[6];
	char itemName[20];
	double itemPrice;
	int quantity;

    printf("GST included Items\n");
    if ((gstText = fopen("gst.txt", "r")) == NULL ) {
		puts("The file 'gst.txt' could not be opened");
		puts("Please contact your system administrator.");
	}
	else {
		printf("%s \t %s \t %s \t %s \n", "Item Code", "Item Name", "Item Price", "Quantity");

		while (!feof(gstText)){
			fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &itemPrice, &quantity);
			printf("%s \t %s \t %.2lf \t %d \n", itemCode, itemName, itemPrice, quantity);
		}

		fclose(gstText);
	}

	printf("Non-GST included Items\n");
    if ((ngstText = fopen("ngst.txt", "r")) ==NULL ) {
		puts("The file 'ngst.txt' could not be opened");
		puts("Please contact your system administrator.");
	}
	else {
		printf("%s \t %s \t %s \t %s\n", "Item Code", "Item Name", "Item Price", "Quantity");

		while (!feof(ngstText)){
			fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &itemPrice, &quantity);
	    	printf("%s \t %s \t %.2lf \t %d \n", itemCode, itemName, itemPrice, quantity);
		}

		fclose(ngstText);
	}

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

	printf("------------------------------------\n");
	printf("Purchase\n");
	printf("------------------------------------\n");
	printf("Enter -1 to cancel transaction\n");
	printf("Enter c to conclude transaction\n");
	printf("\n");

	printf("Enter the item code: ");
	scanf("%s", itemCodeInput);
	while(strcmp(itemCodeInput, "-1") && strcmp(itemCodeInput, "c")) {
		while(!feof(gstText)) {
			fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
			if (strcmp(itemCodeInput, itemCode) == 0) {
				itemFound = YES;
				isGST = YES;
				break;
			}
			fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
			if (strcmp(itemCodeInput, itemCode) == 0) {
				itemFound = YES;
				isGST = NO;
				break;
			}
		}

		if (itemFound) {
			printf("\n");
			printf("Quantities less than 1 will cancel the selected item\n");
			printf("Enter the quantity: ");
			scanf("%d", &quantityInput);
			if (quantityInput > 0) {

				subtotal = price * quantityInput;
				gstAmount = subtotal * 0.06;
				// Write to file
				fprintf(transactionsText, "%s;%s;%.2lf;%d\n", itemCode, itemName, price, quantityInput);

				printf("\n");
				printf("Code       Name                     Price      Quantity\n");
				printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantityInput);
				printf("\n");
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
				printf("\n");
			}
			else
				printf("Item canceled\n");
		}
		else
			printf("Invalid item.\n");

		rewind(gstText);
		rewind(ngstText);
		itemFound = NO;
		printf("Enter the item code: ");
		scanf("%s", itemCodeInput);
	}

	fclose(transactionsText);

	// Receipt
	if (strcmp(itemCodeInput, "-1") && subtotal > 0) {
		flush = getchar();
		printf("Print receipt? (y/n): ");
		for(;;) {
			receiptPrompt = getchar();
			flush = getchar();
			if (receiptPrompt == 'y') {

				transactionsText = fopen("transactions.txt", "r");

				printf("======== Receipt ========\n");
				printf("Code       Name                     Price      Quantity\n");
				printf("\n");
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
					printf("\n");
				}
				printf("Total Sales incl GST: %.2lf\n", total);
				roundedTotal = round(total * 20.0) / 20.0; // rounds prices to 0.05
				printf("Rounding adjusment: %.2lf\n", roundedTotal - total);
				printf("Total after adj incl GST: %.2lf\n", roundedTotal);
				fclose(transactionsText);
				break;
			}
			else if (receiptPrompt == 'n') {
				printf("Transaction concluded\n");
				break;
			}
			else
				printf("Invalid input, please enter y or n: ");
		}
	}
	else
		printf("Transaction canceled\n");

	fclose(gstText);
	fclose(ngstText);

	return;
}