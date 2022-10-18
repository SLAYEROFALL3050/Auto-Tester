# Auto-Tester
Auto Tester for C code
To Use:

1. Place the compiled code in any directory
2. Make an alias/shortcut that calls the program from wherever you stored it, you can name it whatever you want.
3. When you want to test your code type the following:
'name_of_alias' 'name_of_file_to_test.c' '.c test or .in' 'number_of_tests'

replace 'name_of_alias' with the name of the alias you have chosen to call the auto tester.
replace 'name_of_file_to_test.c' with the name of the file you want to test.
replace '.c test or .in' with either 'c' or 'in' for declaring the type of test.
replace 'number_of_tests' with the number of tests to be run.

Example:
test example.c c 5
test = alias/shortcut to auto tester
example.c = file to test
c = specified test type
5 = number of tests to run

Remember this program must be called in the directory containing the file to test and any tests that must be run.

Important Notes:
-   Tests must be called 'public00.in'. Where 00 represents the test number. The file containing expected 
    results must be named 'public00.output'.
-   The maximum number of tests is 99.
-   Make sure any code can be compiled with a simple 'gcc file.c' command.
    If extra specifications are needed then you must have a Makefile if 'c' is specified or compile the code yourself.
-   If 'in' is specified as the test type then you must compile the code yourself before calling the tester.
    The compiled code must be called 'a.out'.
-   If 'c' is specified as the test type and a Makefile exists in the same directory, then the compiled code must be 
    named 'public00' where 00 represents the test number.