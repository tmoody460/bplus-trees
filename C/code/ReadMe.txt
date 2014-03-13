This folder contains a possible set of codes that generated the sorted tables. Here, I use a simple model with 4 tables: location, senddatetime, user, and message. Certainly it is possible to be extended to other models. For an illustration purpose, I explicitly wrote ten C codes prefixed with step[0-9], which are corresponding to the steps to generate the tables of sorted and unique rows. We note that these codes share a certain amount of common portions, and only basic C functionalities are used. In practice, the code can be written in a more concise way, and also in other high-level programming languages.

You can type “make” to generate the executables. Given our current dataset with 40000 records:

1)	“./exec0_generate_location 40000” will generate the location table from the original dataset. The resulting location table has 40000 files, which are saved in ../data/unsorted_location.
2)	“./exec1_unique_location 40000” will remove the duplicated rows from the location table. The resulting location table has 19000 files, which are saved in ../data/unique_location.
3)	“./exec2_generate_senddatetime 40000” will generate the senddatetime table from the original dataset. The resulting senddatetime table has 1992168 files, which are saved in ../data/unsorted_senddatetime
4)	“./exec3_unique_senddatetime 1992168” will remove the duplicated rows from the senddatetime table. The resulting senddatetime table has 475909 files, which are saved in ../data/unique_senddatetime
5)	“./exec4_generate_user 40000 19000” will generate the user table from the original dataset. The first argument is the record number of the original dataset, and the second argument is the number of the unique locations. This will build the reference between the user table and the location table. The resulting user table has 40000 files, which are saved in ../data/unsorted_user
6)	“./exec5_unique_user 40000” will remove the duplicated rows from the user table. The resulting user table has 40000 files, which are saved in ../data/unique_user
7)	“./exec6_generate_message 40000 475909” will generate the message table from the original dataset. The first argument is the record number of the original dataset, and the second argument is the number of the unique senddatetimes. This will build the reference between the message table and the senddatetime table. The resulting message tables has 1992168 files, which are saved in ../data/unsorted_message
8)	“./exec7_sort_message 1992168” will sort the messages according to the senddatetime. The resulting message table still has 1992168 files, which are saved in ../data/sorted_messsage
9)	“./exec8_sort_senddatetime 475909” will sort the senddatetime table according the hour values. The resulting senddatetime table still has 475909 files, which are saved in ../data/sorted_senddatetime
10)	“./exec9_sort_user 40000” will sort the user table according to the location IDs. The resulting user table still has 40000 files, which are save in ../data/sorted_user

Note that the codes will automatically control the number of files in a folder: if the number of files in a subfolder is large than a threshold, the following files will be generated in another subfolder. Currently the threshold is 100000.

I also provide a set of C codes to generate the commands to print different table files. The names of commands start with “exec_print_”. The following shows an example: “./exec_print_location ../data/unsorted_location/0100000/location_0000008.dat” will print the content of the 8th location file. Using the bash shell programming, we can easily print multiple files: “for i in `seq –w 0 9`; do ./exec_print_location ../data/unsorted_location/0100000/location_000000$i.dat” will print the first 10 location files. 

One useful tip for development is to start with a small number of files. For example, you can test the codes using 10 records, rather than the full 40000 records, to verify the correctness. 





