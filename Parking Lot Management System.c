#include<stdio.h>//This is build in .it use for input and output(scanf,printf)
#include<stdlib.h>//it use for malloc,alloc ,free function
#include<string.h>//strcpy,strcmp
#include<time.h>//for use time
#define max_slot 10//we use it for fixed our slot size , for it now database size is fixed
//we declear global variable for that we use them in any where of our code

struct vehicle{
char plate[100];
char type[100];
int plote;
time_t time_entry;
//for show present time we use time_t data type
};
int free_slot =-1;
//we use void function where no return type and no argument coz it help us to safe from make the code complex
//we declear function here 1st then we make user define function after the main function and call them in main function 
void checkin();
void checkout();
int admin_login();//it use for set admin panel login system
void admin_panel();
void find_free_slot();
void update_slot_status();
void show_slot_status();
void view_history();
// we just call those function in this main part
int main(){
int select_option;
for(;;){
//Here system give user for select option and help to do there right operation 
printf("     WELCOME SIR/MADAM\n");
printf("\n*****************************\n*     PARKING LOT SYSTEM    *\n*****************************\n");
printf("\nAvailable Services:\n");
printf("_______________________\n");
printf("\n1.CheckIN\n");
printf("2.CheckOUT\n");
printf("3.Admin Panel\n");
printf("4.Exit from System\n");
printf("_______________________\n");

//there is no info where loop start where it finish, so when user enter input the loop start when give break the loop stop

printf("ENTER OPTION:\n");
scanf("%d",&select_option);
if(select_option==1) checkin();
else if(select_option==2) checkout();
//else if(select_option==3) admin_panel();
else if(select_option == 3) {
if (admin_login()) {
admin_panel(); 
} else 
{
printf("Invalid Username or Password\n");
}
}
else if (select_option==4) 
break;//break use to leave from loop
else{ printf("Error!\nEnter option is wrong. Try Again\n");
}
}


return 0;}
// Build all function here
//we use slot.txt for slot related every function 

void find_free_slot() {
// we make file for save all data of function ,r help to read file , when use w it help to edit file also help to create file when there is no file. if there is a remain file w mode help us edit other function data in the same file
FILE *slot = fopen("slot.txt", "r");
if (!slot) {
printf("slot.txt file missing, creating...\n");
slot = fopen("slot.txt", "w");
if (!slot) {
printf("Error to create slot.txt\n");
free_slot = -1;
return;
}
for (int i = 0; i < max_slot; i++) {
fprintf(slot, "0\n"); // All slot are avail
}
fclose(slot);
slot = fopen("slot.txt", "r");
if (!slot) {
printf("Error to open slot.txt again\n");
free_slot = -1;
return;
}
}

int status;
free_slot = -1;
for (int i = 0; i < max_slot; i++) {
fscanf(slot, "%d", &status);
if (status == 0) {
free_slot = i;
break;
}
}
fclose(slot);

if (free_slot == -1) {
printf("Sorry, no free slot available!\n");
}
}


void checkin() {
int user_slot;
FILE *slot = fopen("slot.txt", "r+");
if (!slot) {
printf("slot.txt is not exist. Now create a file...\n");
slot = fopen("slot.txt", "w");
if (!slot) {
printf("Error to create slot.txt\n");
return;
}
for (int i = 0; i < max_slot; i++) {
fprintf(slot, "0\n");
}
fclose(slot);
slot = fopen("slot.txt", "r+");
if (!slot) {
printf("Error to open slot.txt\n");
return;
}
}

int status[max_slot];
for (int i = 0; i < max_slot; i++) {
fscanf(slot, "%d", &status[i]);
}

rewind(slot);  // move back to start of file

printf("Enter desired slot number (1 to %d): ", max_slot);
scanf("%d", &user_slot);

if (user_slot < 1 || user_slot > max_slot) {
printf("Invalid slot number!\n");
fclose(slot);
return;
}

if (status[user_slot - 1] == 1) {
printf("Slot %d is not available. Try again.\n", user_slot);
fclose(slot);
return;
}

struct vehicle v;
printf("Enter vehicle plate number: ");
scanf("%s", v.plate);
printf("Enter vehicle type (Allowed: car, bike, truck): ");
scanf("%s", v.type);

    //Check allowed vehicle types
if (strcmp(v.type, "car") != 0 && strcmp(v.type, "bike") != 0 && strcmp(v.type, "truck") != 0) {
printf("Invalid vehicle type! Only 'car', 'bike', or 'truck' allowed.\n");
fclose(slot);
return;
}

v.plote = user_slot;

int year, month, day, hour, minute, second;
struct tm t = {0};

printf("Enter entry date (YYYY MM DD): ");
scanf("%d %d %d", &year, &month, &day);
printf("Enter entry time (HH MM SS): ");
scanf("%d %d %d", &hour, &minute, &second);

t.tm_year = year - 1900;
t.tm_mon = month - 1;
t.tm_mday = day;
t.tm_hour = hour;
t.tm_min = minute;
t.tm_sec = second;

v.time_entry = mktime(&t); // struct tm → time_t

    // Update slot status to occupied
for (int i = 0; i < max_slot; i++) {
if (i == user_slot - 1) {
fprintf(slot, "1\n");
} else {
fprintf(slot, "%d\n", status[i]);
}
}
fclose(slot);

    // Save vehicle info
FILE *park = fopen("park_vehicle.txt", "a");
if (!park) {
printf("Error to  open park_vehicle.txt\n");
return;
}

fprintf(park, "%s\n%s\n%d\n%ld\n", v.plate, v.type, v.plote, v.time_entry);
fclose(park);

    
char time_str[100];
strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&v.time_entry));
printf("Vehicle (%s) parked at slot number %d.\n", v.plate, v.plote);
printf("Entry time: %s\n", time_str);
}
//this update function help to admin for reset all slot when holiday or any issue but this function is not important 
void update_slot_status(){
FILE *slot;
slot=fopen("slot.txt","w");//we use w because it help to edit file
//(!slot) means the file not exist 
if(!slot){
printf("Error.Try other way.");
return;
}
for(int i=0;i<max_slot;i++){
fprintf(slot,"0\n");}//where we didn't use %d because we want reset all data,  so w set all slot data 0 . it mean slot is free or available 
fclose(slot);
}

//show slot status use for full slot status that is it avail or not

void show_slot_status(){
FILE *slot;
slot=fopen("slot.txt","r");
//(!slot) means the file not exist 
if(!slot){
printf("Error.Try other way.");
return;
}
int status;
for(int i=0;i<max_slot;i++){
fscanf(slot,"%d",&status);
if(status ==0) printf("Available:%d\n",i+1);
else if(status==1) printf("Not available:%d\n",i+1);
else printf("ERROR");

}


fclose(slot);


}








void checkout() {
char plate[100];
printf("Enter The Plate NO. of Vehicle:\n");
scanf("%s", plate);

FILE *read_file = fopen("park_vehicle.txt", "r");
FILE *write_file = fopen("temp.txt", "w");
FILE *slot_file = fopen("slot.txt", "r+");
FILE *history_file = fopen("history.txt", "a");

if (!read_file || !write_file || !slot_file || !history_file) {
printf("Error to open the file.\n");
if(read_file) fclose(read_file);
if(write_file) fclose(write_file);
if(slot_file) fclose(slot_file);
if(history_file) fclose(history_file);
return;
}

struct vehicle v;
int found = 0;
time_t exit_time;

while(fscanf(read_file, "%s %s %d %ld", v.plate, v.type, &v.plote, &v.time_entry) == 4) {
if (strcmp(v.plate, plate) == 0 && !found) {
found = 1;

            
int year, month, day, hour, minute, second;
struct tm t = {0};

printf("Enter The Date And Time of Checkout:\n");
printf("DATE (YYYY MM DD): ");
scanf("%d %d %d", &year, &month, &day);
printf("TIME (HH MM SS): ");
scanf("%d %d %d", &hour, &minute, &second);

t.tm_year = year - 1900;
t.tm_mon = month - 1;
t.tm_mday = day;
t.tm_hour = hour;
t.tm_min = minute;
t.tm_sec = second;

exit_time = mktime(&t);

double duration = difftime(exit_time, v.time_entry) / 60.0;
int fee;
if (strcmp(v.type, "car") == 0 || strcmp(v.type, "Car") == 0) {
fee = (int)(duration * 2);
} else {
fee = (int)(duration * 1);
}
if (fee < 0) fee = 0;

            
char paid_status[10];
printf("Are You Paid the Bill For Your Vehicle (yes/no): ");
scanf("%s", paid_status);

if (strcmp(paid_status, "yes") != 0) {
printf("Not Paid, Try Again\n");
fclose(read_file);
fclose(write_file);
fclose(slot_file);
fclose(history_file);
remove("temp.txt");
return;
}

            
char time_str[100];
strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&exit_time));

printf("\nParking Time: %.2f Minute\n", duration);
printf("Total Bill: %d TAKA\n", fee);
printf("Checkout Time: %s\n", time_str);

            // keep slot free
fseek(slot_file, 0, SEEK_SET);
for (int i = 0; i < v.plote - 1; i++) {
fscanf(slot_file, "%*s");
}
long pos = ftell(slot_file);
fseek(slot_file, pos, SEEK_SET);
fprintf(slot_file, "0\n");

            
fprintf(history_file, "%s\n%s\n%d\n%ld\n%ld\n%d\n", v.plate, v.type, v.plote, v.time_entry, exit_time, fee);
} else {
fprintf(write_file, "%s\n%s\n%d\n%ld\n", v.plate, v.type, v.plote, v.time_entry);
}
}

fclose(read_file);
fclose(write_file);
fclose(slot_file);
fclose(history_file);

remove("park_vehicle.txt");
rename("temp.txt", "park_vehicle.txt");

if (!found) {
printf("\nThe Vehicle Is Not Found\n");
} else {
printf("\nCheckout Successful .Thanks\n");
}
}
int admin_login() {
char input_user[50], input_pass[50];
char file_user[50], file_pass[50];

FILE *f = fopen("admin_cred.txt", "r");

    
if (!f) {
printf("Admin credentials file missing! Creating default credentials...\n");
f = fopen("admin_cred.txt", "w");
if (!f) {
printf("Could not create admin_cred.txt\n");
return 0;
}
        
fprintf(f, "admin 1234\n");
fclose(f);
f = fopen("admin_cred.txt", "r");
if (!f) {
printf("Could not reopen admin_cred.txt\n");
return 0;
}
}


fscanf(f, "%s %s", file_user, file_pass);
fclose(f);

    
printf("\nEnter UserName: ");
scanf("\n%s", input_user);
printf("\nEnter Password: ");
scanf("\n%s", input_pass);

if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
return 1; 
} else {
return 0; 
}
}
void admin_panel(){
int select_option;
//Here system give admin for select option and help to do there right operation 
printf("             WELCOME\n");
printf("===============================\nPARKING LOT SYSTEM ADMIN PANEL\n===============================\n");

printf("Available Option:\n");
printf("_______________________\n");
printf("1.Show Slot Status\n");
printf("2.Show Vehicle History \n");
printf("3.Reset All Slot\n");
printf("4.Exit to Main Menu\n");
printf("_______________________\n");
while(1){
printf("ENTER OPTION:\n");
scanf("%d",&select_option);
//there is no info where loop start where it finish, so when user enter input the loop start when give break the loop stop

if(select_option==1) show_slot_status();
else if(select_option==2) view_history();
else if(select_option==3) update_slot_status();
else if (select_option==4){printf("Now Main Menu Select option:\n");
break;}
        
else printf("Error!\nEnter option is wrong.Try Again\n");
}
}

void view_history(){
FILE *history;
history=fopen("history.txt","r");
//(!slot) means the file not exist 
if(!history){
printf("Error.Try other way.");
return;
}
struct vehicle v;
time_t exit_time;
int fee;
printf("\n Vehicle History \n");
while(fscanf(history, "%s %s %d %ld %ld %d", v.plate, v.type, &v.plote, &v.time_entry, &exit_time, &fee) == 6) {
char entry_time_str[100];
char exit_time_str[100];

strftime(entry_time_str, sizeof(entry_time_str), "%Y-%m-%d %H:%M:%S", localtime(&v.time_entry));
strftime(exit_time_str, sizeof(exit_time_str), "%Y-%m-%d %H:%M:%S", localtime(&exit_time));

printf("Plate: %s\n", v.plate);
printf("Type: %s\n", v.type);
printf("Slot: %d\n", v.plote);
printf("Entry Time: %s\n", entry_time_str);
printf("Exit Time: %s\n", exit_time_str);
printf("Fees: %d\n", fee);
printf("-------------------------\n");
}
}