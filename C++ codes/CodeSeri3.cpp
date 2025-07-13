///////////////////////////////////////////////////////////////////////////////////
// Implementation code for seri 3 corresponding to 0.001 < eta < 0.01 with d_eta = 0.001       
//////////////////////////////////////////////////////////////////////////////////
using namespace std;

#include <iostream>
#include <math.h>
#include <fstream>
#include <cstdlib>  // defines rand
#include <ctime>    // defines time() function

#define linkState(x)  ( (x==0) ? 0 : 1)  // inline function

#define N    2000   // Number of nodes
#define p    0.1    // probability of connections in random network
#define TE   5      // activation time of Exc. links
#define TI   7      // activation time of Inh. links
#define D    4     // thereshold value for firing
#define tmax 20000    // maximum time
#define Ecurrent   1.0  // corresponding to excitatory synaptic weight, i.e., W_E

#define dEta     0.001   // eta step
#define etaFinal 0.01  // end of eta space: dEta < eta < etaFinal 

#define E    0.8    // percentage of Excitatory neurons
#define Icurrent   4  //corresponding to inhibitory synaptic weight, i.e., W_I


int  A[N][N] = {0};               // Adjacency Matrix
int node_state[N] = {0};          // state of neurons, including: 0=inactive , 1=active
double node_input[N] = {0};          // neuron input
int Link_counter[N][N]         = {0}; // including 0, 1, 2, ...,TE for 0<row<n_E and 0<col<N
int Link_counter_updated[N][N] = {0}; // and 0, 1, 2, ...,TI for n_E<row<N and 0<col<N

/********************************************/
// Main
/********************************************/
int main()
{
    ofstream outputNE("./Seri3EnodeN2000K200D4We1Wi4Ne80.txt"); //output for active E-neurons
    ofstream outputNI("./Seri3InodeN2000K200D4We1Wi4Ne80.txt"); //output for active I-neurons
    ofstream outputLE("./Seri3ElinkN2000K200D4We1Wi4Ne80.txt"); //output for active E-synapses
    ofstream outputLI("./Seri3IlinkN2000K200D4We1Wi4Ne80.txt"); //output for active I-synapses
    
    outputNE << "#N,	<k>, Epercentage = " << N << "\t" << N*p << "\t" << E << endl;
    outputNE << "#delta_E,	delta_I,	W_I = " << TE << "\t" << TI << "\t" << Icurrent << endl;
	outputNE << "#hor:time,	ver:Eta" << endl;
	outputNE << "#dEta = " << dEta << endl;
	
	outputNI << "#N,	<k>, Epercentage = " << N << "\t" << N*p << "\t" << E << endl;
    outputNI << "#delta_E,	delta_I,	W_I = " << TE << "\t" << TI << "\t" << Icurrent << endl;
	outputNI << "#hor:time,	ver:Eta" << endl;
	outputNI << "#dEta = " << dEta << endl;
	
	outputLE << "#N,	<k>, Epercentage = " << N << "\t" << N*p << "\t" << E << endl;
    outputLE << "#delta_E,	delta_I,	W_I = " << TE << "\t" << TI << "\t" << Icurrent << endl;
	outputLE << "#hor:time,	ver:Eta" << endl;
	outputLE << "#dEta = " << dEta << endl;
	
	outputLI << "#N,	<k>, Epercentage = " << N << "\t" << N*p << "\t" << E << endl;
    outputLI << "#delta_E,	delta_I,	W_I = " << TE << "\t" << TI << "\t" << Icurrent << endl;
	outputLI << "#hor:time,	ver:Eta" << endl;
	outputLI << "#dEta = " << dEta << endl;
      
    
    unsigned seed = time(NULL);
    srand(seed);
    double r;       // double random number between (0,1)
    int n_E = N*E;  // number of Excitatory neurons
    int NL = 0;   // number of links
    
    //-----------------------------------------------
    // making Adjacency matrix of ER network (Directed)
    for ( int i=0 ; i<N; i++ ){
        for ( int j=0 ; j<N ; j++ ){
            if (j==i) continue;
            double r = rand()/double(RAND_MAX);
            if (r<p){
                A[i][j]=1; 
                NL++;
            }
        }
    }
    //------------------------------------------------
    // Loop for sweeping Eta
    double eta;
    for (eta=dEta ; eta<etaFinal ; eta += dEta){
    	int t=0;        
    	// initial values 
    	for (int i=0 ; i<N ; i++){
    		node_state[i]=0;
    		for (int j=0 ; j<N ; j++){
    			Link_counter[i][j] = 0;
    			Link_counter_updated[i][j] = 0;
			}
		}    
	
	// neurons
	double act_E_density=0; // fraction of excitatory active neurons
    double act_I_density=0; // fraction of inhibitory active neurons
	
	// synapses
	double act_LE_den=0; // fraction of excitatory active synapses
    double act_LI_den=0; // fraction of inhibitory active synapses
	
    // Noise on nodes at t=0
    int N_E_0=n_E*eta;      // initial number of active excitatory neurons
	int N_I_0=(N-n_E)*eta;  // initial number of active inhibitory neurons
    
    
	// becomming active
	// Excitatory
    for (int i=0 ; i<N_E_0 ; i++){
        node_state[i]=1;
        act_E_density++;
    }
    // Inhibitory
    for (int i=0 ; i<N_I_0 ; i++){
        node_state[N-i-1]=1;
        act_I_density++;
    }
     
	    
    //output at t=0
    // neurons
    act_E_density /= N;
    act_I_density /= N;
    outputNE << eta << "\t" << act_E_density << "\t";
    outputNI << eta << "\t" << act_I_density << "\t";
    // synapses
	outputLE << eta << "\t" << act_LE_den << "\t";
	outputLI << eta << "\t" << act_LI_den << "\t";
    
    
    // Starting Dynamics
    t++;
    while ( t<=tmax ){
            
        // updating link-counters for Excitatory links
        for (int i=0 ; i<n_E ; i++){
            for (int j=0 ; j<N ; j++){
              	if ( A[i][j]!=0 ){
                	
					// links that remain inactive	
              		if ( (node_state[i]==0) && (Link_counter[i][j]==0)  )
                       	Link_counter_updated[i][j] = 0;
                     
					// links that become active as a result of activation of thier pre-synaptic neuron  	
                    else if ( (node_state[i]==1) && (Link_counter[i][j]==0)  )
                       	Link_counter_updated[i][j] = 1;	
                    
					// active links whose time counter is less than TE   
                    else if ( (Link_counter[i][j] >= 1) && (Link_counter[i][j] < TE) )
                       	Link_counter_updated[i][j] = Link_counter[i][j] + 1;
                        
                    // active links whose time counter is TE --> become inactive
					else if ( Link_counter[i][j] == TE )
                       	Link_counter_updated[i][j] = 0;	
				}
        		act_LE_den += linkState(Link_counter_updated[i][j]);
			}
    	}
        
		// updating link-counters for Inhibitory links
        for (int i=n_E ; i<N ; i++){
            for (int j=0 ; j<N ; j++){
              	if ( A[i][j]!=0 ){
                	
					// links that remain inactive	
               		if ( (node_state[i]==0) && (Link_counter[i][j]==0)  )
                       	Link_counter_updated[i][j] = 0;
                    
					// links that become active as a result of activation of thier pre-synaptic neuron    	
                    else if ( (node_state[i]==1) && (Link_counter[i][j]==0)  )
                       	Link_counter_updated[i][j] = 1;	
                    
					// active links whose time counter is less than TI    
                    else if ( (Link_counter[i][j] >= 1) && (Link_counter[i][j] < TI) )
                       	Link_counter_updated[i][j] = Link_counter[i][j] + 1;
                    
					// active links whose time counter is TI --> become inactive    
					else if ( Link_counter[i][j] == TI )
                       	Link_counter_updated[i][j] = 0;	
				}	
				act_LI_den += linkState(Link_counter_updated[i][j]);
			}       
        } 
		
		    
        // updating node states
        // First: node inputs
        for (int i=0 ; i<N ; i++){
            node_input[i] = 0;
            // positive
			int positive_input=0;
            for (int j=0 ; j<n_E ; j++){
            	positive_input += ( A[j][i] *  linkState(Link_counter_updated[j][i]) );
			}
            // negative
            int negative_input=0;
            for (int j=n_E ; j<N ; j++){
            	negative_input += ( A[j][i] *  linkState(Link_counter_updated[j][i]) );
			}
            // total input
            node_input[i] = Ecurrent*positive_input - Icurrent*negative_input;
    	}   
        // Then: node-state
        for (int i=0 ; i<N ; i++){
            node_state[i]=0;
            if ( node_input[i] >= D )
                node_state[i]=1;
        }
        
		    
        // external input on nodes at t
        for (int i=0 ; i<N ; i++){
            r = rand()/double(RAND_MAX);
            if (r<eta) node_state[i]=1;
        }
         
		   
        // counting active nodes
        act_E_density=0;
        act_I_density=0;
        for (int i=0 ; i<n_E ; i++)
            act_E_density += node_state[i];
        for (int i=n_E ; i<N ; i++)
            act_I_density += node_state[i];  
          
		   
        // output at t
        // active neurons
        act_E_density /= N;
        act_I_density /= N;
        outputNE << act_E_density << "\t";
        outputNI << act_I_density << "\t";
        // active synapses
        act_LE_den /= NL;
        act_LI_den /= NL;
        outputLE << act_LE_den << "\t";
        outputLI << act_LI_den << "\t";
        
		    
        //updating for the next time step
        t++;
        for (int i=0 ; i<N ; i++){
        	for (int j=0; j<N ; j++){
                Link_counter[i][j] = Link_counter_updated[i][j];
            }
		}
		// resetting density of active synapses
		act_LE_den = 0;
        act_LI_den = 0;
                     
    } // end of while
    
    outputNE << endl;
    outputNI << endl;
    outputLE << endl;
    outputLI << endl;
    
 } // end of Eta loop
        
    return 0;
}



