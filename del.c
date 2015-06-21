#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replaceFile(char fileName[25], char itemCodeInput[8]);

int main (void)
{
    FILE *gstText;
    FILE *ngstText;
    char itemCodeInput[8];
    char itemCode[8];
    char itemName[25];
    double price;
    int quantity;
    int gst;
    int itemFound;
    char ch; //for y/n
    char flush;

	puts("------------------------------------");
	puts("Delete Items");
	puts("------------------------------------");
	puts("Enter -1 to exit");
	puts("");

	printf("Enter item code to be deleted: ");
	scanf("%s", itemCodeInput);

	while (strcmp(itemCodeInput, "-1")!= 0){ //compare input and code
        
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

        itemFound = 0;
        gst = 0;

		while(!feof(gstText)) { // check if item code matches in gst.txt
			fscanf(gstText, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
			if (strcmp(itemCodeInput, itemCode) == 0) {
				itemFound = 1;
                gst = 1;
				break;
			}
		} 

        if (itemFound == 0) { //open ngst file

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


        if (itemFound == 1){ //if matches gst

            printf("Item found.\n\n");
			puts("Code       Name                     Price      Stock");
			printf("%-10s %-24s %-10.2lf %-10d\n", itemCode, itemName, price, quantity);

            flush = getchar();
            printf("Confirm to delete item? (y/n)\n");
            scanf(" %c", &ch);

            if (ch == 'y'){ //if y to delete

                if (quantity != 0){ //say no if quantity not zero
                    printf("Stock is not zero. Item cannot be deleted.\n");
                }
                else { //if quantity is zero
                    if (gst) {
                        replaceFile("gst.txt\0", itemCodeInput);
                    }
                    else {
                        replaceFile("ngst.txt\0", itemCodeInput);
                    }
                    printf("Item deleted\n");

                } //end if quantity is zero                             

            } //end if y to delete

            else if (ch == 'n'){ //if n to delete, cancel transaction
                printf("Deletion cancelled.\n");
            }

        } //end item found
 
        printf("\nEnter item code to be deleted: ");
        scanf("%s", itemCodeInput);

    } // end of while(strcmp)
}

void replaceFile(char fileName[25], char itemCodeInput[8]) {
    FILE *temp;
    FILE *file;
    char itemCode[8];
    char itemName[25];
    double price;
    int quantity;

    if ((file = fopen(fileName, "r")) != NULL) {
        temp = fopen("temp.txt", "w"); //open new file to write

        while (!feof(file)){ //write in new file
            fscanf(file, " %9[^;];%25[^;];%lf;%d", itemCode, itemName, &price, &quantity);
            if (strcmp(itemCodeInput, itemCode) != 0) { //avoid the deleted item
                fprintf(temp, "%s;%s;%.2lf;%d\n", itemCode, itemName, price, quantity);
            }
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
}