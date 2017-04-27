

To fix C++11 syntax highlighting go to:


few things - first 

c++ general processor incude paths

go to providers tab

select two - 

CDT USERT Settings & 
CDT built in GCC settings - resize the window and look at 
	command settings - add -std=c++11
	
	
	
or 

 C++ -> build -> discovery settings GCC++ compiler and
 	add the flag there
	
Then look for Paths & Symbols 
	GNU C++ and define __cplusplus to be 201403
	
	
	
