This is a modified version of your code. We discuss the modifications in our report.

You can type “make” to generate the executables. Given a dataset with 2000 records:

You can type "make phase1_queries" to make the phase 1 queries.

1)	“./exec0_generate_location 2000” will generate the location table from the original dataset. The resulting location table has 2000 files, which are saved in ../data/unsorted_location.
2)	“./exec1_unique_location 2000” will remove the duplicated rows from the location table. The resulting location table has 1725 files, which are saved in ../data/unique_location.
3)	“./exec2_generate_senddatetime 2000” will generate the senddatetime table from the original dataset. The resulting senddatetime table has 98357 files, which are saved in ../data/unsorted_senddatetime
4)	“./exec3_unique_senddate 98357” will remove the duplicated rows from the senddate table. The resulting senddate table has 336 files, which are saved in ../data/unique_senddate
4h)	“./exec3h_unique_sendtime 98357” will remove the duplicated rows from the sendtime table. The resulting sendtime table has 1440 files, which are saved in ../data/unique_sendtime
5)	“./exec4_generate_user 2000 1725” will generate the user table from the original dataset. The first argument is the record number of the original dataset, and the second argument is the number of the unique locations. This will build the reference between the user table and the location table. The resulting user table has 2000 files, which are saved in ../data/unsorted_user
6)	“./exec5_unique_user 2000” will remove the duplicated rows from the user table. The resulting user table has 2000 files, which are saved in ../data/unique_user
7)	“./exec6_generate_message 2000 98357” will generate the message table from the original dataset. The first argument is the record number of the original dataset, and the second argument is the number of the unique senddatetimes. This will build the reference between the message table and the senddatetime table. The resulting message tables has 98357 files, which are saved in ../data/unsorted_message
8)	“./exec7_sort_message 98357” will sort the messages according to the senddatetime. The resulting message table still has 98357 files, which are saved in ../data/sorted_messsage
9)	“./exec9_sort_user 2000” will sort the user table according to the location IDs. The resulting user table still has 2000 files, which are save in ../data/sorted_user

Note that the codes will automatically control the number of files in a folder: if the number of files in a subfolder is large than a threshold, the following files will be generated in another subfolder. Currently the threshold is 100000.

I also provide a set of C codes to generate the commands to print different table files. The names of commands start with “exec_print_”. The following shows an example: “./exec_print_location ../data/unsorted_location/0100000/location_0000008.dat” will print the content of the 8th location file. Using the bash shell programming, we can easily print multiple files: “for i in `seq –w 0 9`; do ./exec_print_location ../data/unsorted_location/0100000/location_000000$i.dat” will print the first 10 location files. 
