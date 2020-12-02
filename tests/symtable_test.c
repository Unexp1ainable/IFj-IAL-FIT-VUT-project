#include <check.h>
#include "../src/symtable.h"
#include <stdlib.h>

START_TEST(table_initialisation){
    Symtable * table;
    symtable_init(table);
    for (int i = 0;i<MAX_SIZE_OF_SYM_TABLE;i++){
        ck_assert_ptr_eq(table[i],NULL);
    }
}END_TEST

START_TEST(test_add_item_remove_item){
    Symtable * table;
    symtable_init(table);//tested in table initialisation already
    char mynameis[20]="Timotej";
    bool wasitwithouterror;
    Symtable_item * thefirstitemptr = symtable_add(table, mynameis, &wasitwithouterror);
    symtable_remove(table,mynameis);
    symtable_free(table);
}END_TEST



Suite *symtable_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("symtable");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, table_initialisation);
  suite_add_tcase(s, tc_core);
  return s;
}







//#########################################################################################################################
//#########################################################################################################################
//##########################    MAIN    ###################################################################################
//#########################################################################################################################
//#########################################################################################################################
int main(void) {
  int no_failed = 0;                   
  Suite *s;                            
  SRunner *runner;                     

  s = symtable_suite();                   
  runner = srunner_create(s);          

  srunner_run_all(runner, CK_NORMAL);  
  no_failed = srunner_ntests_failed(runner); 
  srunner_free(runner);                      
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;  
}
