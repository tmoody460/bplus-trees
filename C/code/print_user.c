#include "tables.h"

int main(int argc, char **argv)
{
    /* print usage if needed */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s user_table_file\n", argv[0]);
        exit(0);
    }
    
    FILE *fp = fopen(argv[1],"rb");
    
    if (!fp) {
        fprintf(stderr, "Cannot open %s\n", argv[1]);
        exit(0);
    }
    
    user_table_entry_t user_table_entry;
    
    read_user(fp, &user_table_entry);
    
    print_user(&user_table_entry);
    
    fclose(fp);
    
    return 0;
}
