#include "record.h"

int main(int argc, char **argv)
{
    /* print usage if needed */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s record_table_file\n", argv[0]);
        exit(0);
    }
    
    FILE *fp = fopen(argv[1],"rb");
    
    if (!fp) {
        fprintf(stderr, "Cannot open %s\n", argv[1]);
        exit(0);
    }
    
    record_t *record = NULL;
    
    record = read_record(fp);
    
    print_record(record);
    
    free_record(record);
    
    fclose(fp);
    
    return 0;
}
