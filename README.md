To run each C++ code (in folder C++ codes) , set the values of parameters and change the name of output files based on them.

Data for reproducing Fig.1 are obtained while E=0.8 (percentage of Excitatory neurons) and Icurrent=4 (inhibitory synaptic weight)

Data for reproducing Figs. 2 and 3 are obtained while E and Icurrent are set for balanced structures, such that
          (E,Icurrent)=  (10,0.11), (20,0.25), (30,0.43), (40,0.67), (50,1), (60,1.5), (70,2.33), (80,4), (90,9)

To reproduce other text files needed for plotting results in Figs 2 and 3 (such as mean_acivity_matrix_eta1.txt, mean_synaptic_current_matrix_eta1.txt, mean_synaptic_energy_matrix_eta1.txt, ...), run the C++ codes for the desired parameters and find the variables (such as mean values of energy and current) using Python codes. As an example, data for the case of Icurrent=4 and E=10,...,90 and Python codes could be found in folder "Data_for_WI=1.5_and_all_NE_values". 
          
