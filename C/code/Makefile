all:
	g++ -o exec0_generate_location step0_generate_location_table.c tables.c
	g++ -o exec1_unique_location step1_unique_location_table.c tables.c
	g++ -o exec2_generate_senddatetime step2_generate_senddatetime_table.c tables.c
	g++ -o exec3_unique_senddate step3_unique_senddate_table.c tables.c
	g++ -o exec3h_unique_sendtime step3h_unique_sendtime_table.c tables.c
	g++ -o exec4_generate_user step4_generate_user_table.c tables.c
	g++ -o exec5_unique_user step5_unique_user_table.c tables.c
	g++ -o exec6_generate_message step6_generate_message_table.c tables.c
	g++ -o exec7_sort_message step7_sort_message_table.c tables.c
	g++ -o exec9_sort_user step9_sort_user_table.c tables.c
	g++ -o exec_print_record print_record.c record.c
	g++ -o exec_print_user print_user.c tables.c
	g++ -o exec_print_location print_location.c tables.c
	g++ -o exec_print_message print_message.c tables.c
	g++ -o exec_print_senddate print_senddate.c tables.c
	g++ -o exec_print_sendtime print_sendtime.c tables.c

clean:
	rm exec0_generate_location exec1_unique_location exec2_generate_senddatetime exec3_unique_senddatetime \
	exec4_generate_user exec5_unique_user exec6_generate_message exec7_sort_message exec8_sort_senddatetime \
	exec9_sort_user exec_print_record exec_print_user exec_print_location exec_print_message exec_print_senddatetime \
	user_tree location_tree time_tree message_tree

#new queries
queries:
	g++ -o query_a query_a.c tables.c query_helper.c
	g++ -o query_b query_b.c tables.c query_helper.c
	g++ -o query_c query_c.c tables.c query_helper.c
	g++ -o query_d query_d.c tables.c query_helper.c

bplustrees:
	g++ -o user_tree user_tree.c query_helper.c tables.c
	g++ -o location_tree location_tree.c query_helper.c tables.c
	g++ -o time_tree time_tree.c query_helper.c tables.c
	g++ -o message_tree message_tree.c query_helper.c tables.c