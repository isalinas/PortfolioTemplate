If the makefile and main.cpp are in the same directory, then just type make and the file will compile and make exe part2.

To run the program enter ./part2 -b # -p # -c # -i #
where # is the number 

if you miss a flag or number the program will notify you and terminate 


#Questions

For my monitor it implements a signal and contiue design. The reason why i chose this is because its to make sure there is no race condition for the buffer. My monitor makes sure there is only one thread by using a signal and wait on condition variables. Outside of the monitor i included a mutex and conditions after the monitor to include Mutual Exclusivity. 
