#include <stdio.h>
#include <string.h>

enum boolean { NO, YES };

int main(void)
{
    char itemCode[8];
    char itemName[25];
    double price;
    int quantity;
	FILE *gstText;
	FILE *ngstText;
    FILE *temp;
    FILE *text;
	char itemCodeInput[8];
	int itemFound;
	int Name = 1;
	int Price = 2;
	int Quantity = 3;
	int choice;
	char newItemName[30];
	double newPrice;
	int newQuantity;

	if((gstText = fopen("purchase.txt", "r")) == NULL){
        puts("File not found.");
        puts("Please contact your system administrator.");
        return;
	}
    else if ((ngstText = fopen("purchase.txt", "r")) == NULL){
        puts("File not found.");
        puts("Please contact your system administrator.");
        return;
    }

	temp = fopen("tempPurchase.txt", "w"); //open temp file

	puts("------------------------------------");
	puts("Edit Items");
	puts("------------------------------------");
	puts("Enter -1 to cancel transaction");
	puts("");

	printf("Enter the item code to be edited: ");
	scanf("%s", itemCodeInput);

	while (strcmp(itemCodeInput, "-1") != 0){ //start transaction

        itemFound = NO;

        do { // check if item match in gst.txt
            fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
            if (strcmp(itemCodeInput, itemCode) == 0){
                itemFound = YES;
                text = gstText;
                break;
            }
        } while (!feof(gstText));

        if (itemFound == NO){
            do { // check if item match in gst.txt
                fscanf(ngstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
                if (strcmp(itemCodeInput, itemCode) == 0){
                    itemFound = YES;
                    text = ngstText;
                    break;
                }
            } while (!feof(ngstText));
        }

        if (itemFound == NO){ //if item cant be found
            printf("Item cannot be found.\n");
        }

        else if (itemFound == YES){ //if item found
            printf ("Item found\n\n");
            //print out selected item
			puts("Code       Name                     Price      Stock");
			printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantity);
            puts("");

            puts("Which would you like to edit?");
            puts("1. Name");
            puts("2. Price");
            puts("3. Quantity");
            scanf("%d", &choice);

            if (choice == Name){ //If 1 chosen
                printf("Change name of item.\n");
                printf("Please enter the new name: ");
                scanf("%s", newItemName); //get the new name of the item

                fprintf(temp, "%s; %s; %.2lf; %d\n", itemCodeInput, newItemName, price, newQuantity);
                // print to tempPurchase.txt the edited item
            }

            if (choice == Price){ //If 2 chosen
                printf("Change the price of item.\n");
                printf("Please enter the new price: ");
                scanf("%lf", &newPrice); //get the new price of the item

                fprintf(temp, "%s; %s; %.2lf; %d\n", itemCodeInput, itemName, newPrice, quantity);
                // print to tempPurchase.txt the edited item
            }

            if (choice == Quantity){ //If 3 chosen
                printf("Change the quantity of item.\n");
                printf("Please enter the new quantity: ");
                scanf("%d", &newQuantity); //get the new quantity of the item

                fprintf(temp, "%s; %s; %.2lf; %d\n", itemCodeInput, itemName, price, quantity);
                //print to tempPurchase.txt the edited item
            }
            else {
                puts("Invalid input. Please enter any of the following");
                puts("1. Name");
                puts("2. Price");
                puts("3. Quantity");
            }

        } //end to if item found
        puts("mewo");
        do{ //printing the rest of the purchase.txt to tempPurchase.txt
            fscanf(text.fp, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
            if (strcmp(itemCodeInput, itemCode) != 0){ //skipping the edited item
                fprintf(temp, "%s ; %s ; %.2lf ; %d\n", itemCode, itemName, price, quantity);
            }
        }while (!feof(temp));

        fclose(gstText);
        fclose(ngstText);
        fclose(temp);
        remove("purchase.txt");
        rename("tempPurchase.txt", "purchase.txt");

        printf("Enter the item code to be edited: ");
        scanf("%s", itemCodeInput);

    } //end to while loop

    printf("Transaction cancelled.\n");

}
